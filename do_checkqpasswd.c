#include "qpasswd.h"
#include "hextab.h"
#include "crypto.h"
#include "case.h"

#undef DEBUG

#define METHOD_PLAIN        1
#define METHOD_APOP         2
#define METHOD_CRAM_MD5     4
#define METHOD_CRAM_SHA1    8
#define METHOD_CRAM_RIPEMD 16
#define METHOD_DIGEST_MD5  32
#define METHOD_ALL         63

/* be aware off:
 *
 * - plain is the generel type for clear text passwords via
 *   -> pop3, USER+PASS
 *   -> smtp, AUTH PLAIN
 *   -> smtp, AUTH LOGIN
 * - we read the $SASL evironment variable, nothing else!
 */
const char *sasl[] = {
 "plain",
 "apop",
 "cram-md5",
 "cram-sha1",
 "cram-ripemd",
 "digest-md5", 0
};

/* list of null terminated passwords */
char qpwds[QPASSWD_MAX];

int do_checkqpasswd(char *r_user, char *r_pass, char *r_greet)
{
	unsigned char digest[20], encrypted[41];
	char homebuf[BUFSIZE], *l_pass=qpwds;
	struct passwd *pw;
	int uid, gid, i, count;
	int method=0;
	int tocheck=0;
#if WANT_ENVIRON_CHECK
	char *e_sasl=getenv("SASL");

	if (e_sasl) {
		int len=strlen(e_sasl);
		for (i=0;sasl[i];i++)
			if (case_diffb(e_sasl, len, sasl[i]) == 0)
				tocheck |= 1<<i;
	} else
#endif
	/* default: check all! */
	tocheck=METHOD_ALL;

	/*
	 * 1. find the user in /etc/qpasswd
	 * 2. fill: home, uid, gid
	 * 3. if user is not found, die!
	 */
	home=homebuf;
	if (qpasswd_gethome(r_user, &uid, &gid, home) == -1)
		return -1;

	if ((count=qpasswd_getpass()) == 0)
		return -1;

	if ((pw=getpwuid(uid)) == 0)
		die_msg(111, "getpwuid() == 0");

	debug("from fd3: (%s) (%s) (%s)\n", r_user, r_pass, r_greet);

	/* check against all aviable pw's from .qpasswd */
	for (i=0; i != count; i++) {
		int j;
		char *e;


/* 1. plaintext checking (USER+PASS / LOGIN / PLAIN) */
		if (tocheck & METHOD_PLAIN) {
			method=1;
			if (!strcmp(r_pass,l_pass)) goto okay;
		}

/* 2. md5 checking (APOP) */
		if (tocheck & METHOD_APOP) {
			MD5_CTX md5;
			MD5_Init(&md5);
			MD5_Update(&md5, r_greet, strlen(r_greet));
			MD5_Update(&md5, l_pass, strlen(l_pass));
			MD5_Final(digest, &md5);
			method=2; e=encrypted;
			for (j=0; j<16; j++) {
				*e = hextab[digest[j]/16]; ++e;
				*e = hextab[digest[j]%16]; ++e;
			}
			*e=0;
			if (!strcmp(r_pass,encrypted)) goto okay;
		}

#if WANT_CRAM_MD5
/* 3. hmac_md5 checking (CRAM-MD5) */
		if (tocheck & METHOD_CRAM_MD5) {
			method=3; e=encrypted;
			hmac_md5(r_pass, l_pass, digest);
			for (j=0; j<16; j++) {
				*e = hextab[digest[j]/16]; ++e;
				*e = hextab[digest[j]%16]; ++e;
			}
			*e=0;
			if (!strcmp(r_greet,encrypted)) goto okay;
		}
#endif

#if WANT_CRAM_SHA1
/* 4. hmac_sha1 checking (CRAM-SHA1) */
		if (tocheck & METHOD_CRAM_SHA1) {
			method=4; e=encrypted;
			hmac_sha1(r_pass, l_pass, digest);
			for (j=0; j<20; j++) {
				*e = hextab[digest[j]/16]; ++e;
				*e = hextab[digest[j]%16]; ++e;
			}
			*e=0;
			if (!strcmp(r_greet,encrypted)) goto okay;
		}
#endif

#if WANT_CRAM_RIPEMD
/* 5. hmac_rmd160 checking (CRAM-RIPEMD) */
		if (tocheck & METHOD_CRAM_RIPEMD) {
			method=5; e=encrypted;
			hmac_rmd160(r_pass, l_pass, digest);
			for (j=0; j<20; j++) {
				*e = hextab[digest[j]/16]; ++e;
				*e = hextab[digest[j]%16]; ++e;
			}
			*e=0;
			if (!strcmp(r_greet,encrypted)) goto okay;
		}
#endif

/*****************************************************************************
 * 6. digest-md5 checking
 *
 * r_user=username
 * r_pass=response  (md5 hash = 32)
 * r_greet=authfile (nc,qop,realm,nonce,cnonce,digesturi)
 *
 *   a1 = md5(user:realm:pass) : nonce : cnonce
 *   a2 = ...
 * resp = hex(a1) + nonce + nc + conce + qop + hex(a2)
 */
#if WANT_DIGEST_MD5
		if (tocheck & METHOD_DIGEST_MD5) {
			char *nc=0, *qop=0, *realm=0, *nonce=0, *cnonce=0,
				*authzid=0, *digesturi=0;
			unsigned char ea1[33], ea2[33];
			char authfile[DIGEST_MAX];
			int fd, len;
			MD5_CTX md5;
		
			method=6;
			fd=open_ro(r_greet);
			if (fd == -1) goto next;
			len=read(fd, authfile, DIGEST_MAX-1);
			if (len == -1) goto next;
			close(fd);
			authfile[len]=0;
		
			for (j=0;j<len;j++) /* terminate strings */
				if (authfile[j] == '\n') authfile[j]=0;
		
			for (j=0;j<len;j+=strlen(authfile+j)+1) {
				char *x=authfile+j;
				if (!strncmp(x,"nc", 2)) nc=x+3;
				if (!strncmp(x,"qop", 3)) qop=x+4;
				if (!strncmp(x,"realm", 5)) realm=x+6;
				if (!strncmp(x,"nonce", 5)) nonce=x+6;
				if (!strncmp(x,"cnonce", 6)) cnonce=x+7;
				if (!strncmp(x,"authzid", 7)) authzid=x+8;
				if (!strncmp(x,"digest-uri", 10)) digesturi=x+11;
			}
			if (!nc || !qop || !realm || !nonce ||
				!cnonce || !digesturi) goto next;
		
			MD5_Init(&md5);
			MD5_Update(&md5, r_user, strlen(r_user));
			MD5_Update(&md5, ":", 1);
			MD5_Update(&md5, realm, strlen(realm));
			MD5_Update(&md5, ":", 1);
			MD5_Update(&md5, l_pass, strlen(l_pass));
			MD5_Final(digest, &md5);
		
			MD5_Init(&md5);
			MD5_Update(&md5, digest, 16); /* md5(user+realm+pass) */
			MD5_Update(&md5, ":", 1);
			MD5_Update(&md5, nonce, strlen(nonce));
			MD5_Update(&md5, ":", 1);
			MD5_Update(&md5, cnonce, strlen(cnonce));
			if (authzid) { /* warning: not tested with authzid! */
				MD5_Update(&md5, ":", 1);
				MD5_Update(&md5, authzid, strlen(authzid));
			}
			MD5_Final(digest, &md5);
			for (e=ea1,j=0; j<16; j++) {
				*e = hextab[digest[j]/16]; ++e;
				*e = hextab[digest[j]%16]; ++e;
			} *e=0; /* ea1 = ready */
		
			MD5_Init(&md5);
			MD5_Update(&md5, "AUTHENTICATE:", 13);
			MD5_Update(&md5, digesturi, strlen(digesturi));
			if (strlen(qop) > 4) /* for auth-* type */
				MD5_Update(&md5, ":00000000000000000000000000000000", 32);
			MD5_Final(digest, &md5);
			for (e=ea2,j=0; j<16; j++) {
				*e = hextab[digest[j]/16]; ++e;
				*e = hextab[digest[j]%16]; ++e;
			} *e=0; /* ea2 = ready */
		
		        /* resp = hex(a1) + nonce + nc + conce + qop + hex(a2) */
			MD5_Init(&md5);
			MD5_Update(&md5, ea1, 32);
			MD5_Update(&md5, ":", 1);
			MD5_Update(&md5, nonce, strlen(nonce));
			MD5_Update(&md5, ":", 1);
			MD5_Update(&md5, nc, strlen(nc));
			MD5_Update(&md5, ":", 1);
			MD5_Update(&md5, cnonce, strlen(cnonce));
			MD5_Update(&md5, ":", 1);
			MD5_Update(&md5, qop, strlen(qop));
			MD5_Update(&md5, ":", 1);
			MD5_Update(&md5, ea2, 32);
			MD5_Final(digest, &md5);
			for (e=encrypted,j=0; j<16; j++) {
				*e = hextab[digest[j]/16]; ++e;
				*e = hextab[digest[j]%16]; ++e;
			} *e=0;
		
			if (!strcmp(r_pass,encrypted)) {
				MD5_Init(&md5);
				MD5_Update(&md5, ":", 1);
				MD5_Update(&md5, digesturi, strlen(digesturi));
				MD5_Final(digest, &md5);
				for (e=ea2,j=0; j<16; j++) {
					*e = hextab[digest[j]/16]; ++e;
					*e = hextab[digest[j]%16]; ++e;
				} *e=0; /* ea2 = ready */
		
		                /* rspauth = hex(a1) + nonce + nc + conce + qop + hex(a2) */
				MD5_Init(&md5);
				MD5_Update(&md5, ea1, 32);
				MD5_Update(&md5, ":", 1);
				MD5_Update(&md5, nonce, strlen(nonce));
				MD5_Update(&md5, ":", 1);
				MD5_Update(&md5, nc, strlen(nc));
				MD5_Update(&md5, ":", 1);
				MD5_Update(&md5, cnonce, strlen(cnonce));
				MD5_Update(&md5, ":", 1);
				MD5_Update(&md5, qop, strlen(qop));
				MD5_Update(&md5, ":", 1);
				MD5_Update(&md5, ea2, 32);
				MD5_Final(digest, &md5);
				for (e=encrypted,j=0; j<16; j++) {
					*e = hextab[digest[j]/16]; ++e;
					*e = hextab[digest[j]%16]; ++e;
				} *e=0;
		
				fd=open_trunc(r_greet);
				if (fd == -1) goto next;
				len=write(fd, encrypted, 32);
				if (len != 32) goto next;
				close(fd);
		
				/* qmail-smtpd, reads the known file */
				/* qmail-pop3d, reads the environment for the filename */
				e=authfile;
				e+=fmt_str(e, "DIGEST_RESPONSE=");
				e+=fmt_str(e, r_greet);
				*e=0;
				if (putenv(authfile) == -1) die_env();
		
				goto okay; /* wow! */
			} /* if correct password */
		} /* 6. digest-md5 checking */
		next:
#endif
		/* switch to the next possibility */
		l_pass+=strlen(l_pass)+1;
	}
	return -1;

	okay:
#if LOG_CHECKQPASSWD_METHOD
	doname(); log1("method was: ");
	switch (method) {
	case 1: lognl("plain"); break;
	case 2: lognl("apop"); break;
#if WANT_CRAM_MD5
	case 3: lognl("cram-md5"); break;
#endif
#if WANT_CRAM_SHA1
	case 4: lognl("cram-sha1"); break;
#endif
#if WANT_CRAM_RIPEMD
	case 5: lognl("cram-ripemd"); break;
#endif
#if WANT_DIGEST_MD5
	case 6: lognl("digest-md5"); break;
#endif
	}
	flush();
#endif

#if WANT_VALID_SHELL
	if (!pw->pw_shell) die_shell();
#endif
	if (pw->pw_shell) shell=pw->pw_shell;
	else shell=default_shell;

	/* now we switch to the alias user */
	switch2user(uid, gid, pw->pw_name);

	return 0;
}
