#include "qpasswd.h"

#define SEP  ':'

#if WANT_LASTPOP
u32 lastpop;
#endif

/*
 * # OVERVIEW:
 *
 * [/some/path/to/put/in/here:uid1:gid1]
 * user1:home1  -> /some/path/to/put/in/here/home1/.qpasswd (uid1/gid1)
 * user2:/home  -> /home/.qpasswd (uid1/gid1)
 *
 * [/some/OTHER/path:uid2:gid2]
 * user1@lala.de:homeXY   -> /some/OTHER/path/homeXY/.qpasswd (uid2/gid2)
 * user2@lala.de:/home    -> /home/.qpasswd (uid2/gid2)
 *
 * xxx: should we check the user / the perms ?
 */

/* needs r_user, sets up: home+uid+gid */
int qpasswd_gethome(const char *r_user, int *uid, int *gid, char *homebuf)
{
	size_t tmp=0, nl=0, sep1=0, sep2=0, full;
	size_t userlen=strlen(r_user); /* len of the username we got */
	char *map=mmap_ro(QPASSWD_FILE, &full); /* out qpasswd via mmap! */
#if WANT_LASTPOP
	size_t sep3;
#endif

	if (map == 0)
		die_msg(111, "mmap() @ /etc/qpasswd");

	while (tmp<full) { /* for all bytes */

		/* find end of line */
		for (nl=tmp; nl<full && map[nl] != '\n'; ++nl);

		/* skip comments */
		if (map[tmp] == '#') goto next;

		/* remember the last '[user:uid:gid]' line */
		if (map[tmp] == '[' && map[nl-1] == ']') {

			/* find 1. seperator */
			for (sep1=tmp; sep1<nl; sep1++)
				if (map[sep1] == SEP) break;
			if (sep1 == tmp) goto next;

			/* find 2. seperator */
			for (sep2=sep1+1; sep2<nl; sep2++)
				if (map[sep2] == SEP) break;
			if (sep2 == sep1+1) goto next;

			/* get the home */
			if (sep1-tmp >= BUFSIZE) die111();
			homebuf[fmt_strn(homebuf, map+tmp+1, sep1-tmp-1)]=0;

			/* get uid + gid */
			if (scan_u32(map+sep1+1, uid) < 1) goto next;
			if (scan_u32(map+sep2+1, gid) < 1) goto next;
#if WANT_LASTPOP
			/* find optional 3. seperator */
			for (sep3=sep2+1; sep3<nl; sep3++)
				if (map[sep3] == SEP) break;
			if (sep3 != sep2+1)
				if (scan_u32(map+sep3+1, &lastpop) > 1)
					goto next;
			lastpop=LASTPOP_DEFAULT; /* take the default value */
#endif
			/* we are sure about the section */
			goto next;
		}

		/* maybe we have a username :) */
		if (userlen < nl-tmp) {
			if (!strncmp(r_user, map+tmp, userlen)) {
				if (map[tmp+userlen] != SEP) goto next;

				if (map[tmp+userlen+1] == '/') {
					/* user:/home */
					homebuf[fmt_strn(homebuf,
						map+tmp+userlen+1,
						nl-tmp-userlen-1)]=0;
				} else {
					/* user:home */
					char *s=homebuf+strlen(homebuf);
					s+=fmt_str(s, "/");
					s+=fmt_strn(s, map+tmp+userlen+1,
						nl-tmp-userlen-1);
					homebuf[s-homebuf]=0;
				}
#if LOG_VHOME_FOUND
				if (getenv("LOG_VHOME_FOUND")) {
					doname();
					log3("virtual home found: \"", homebuf, "\"\n");
				}
#endif
				munmap(map, full);
				return 0;
			}
		} /* have a username:userhome */

		next:
		tmp=nl+1; /* tmp is now beginning of the next line */
	}
	munmap(map, full);

	return -1;
}
