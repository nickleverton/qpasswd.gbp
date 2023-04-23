#include "qpasswd.h"
#include "hextab.h"
#include "test_cases.h"

#define NEED_ALL_CRYPTO_STUFF
#include "crypto.h"

extern void out(const char *msg);
extern void show_result(char *v1, char *v2, int len);

extern int fromhex(char c);
extern int scan_hex(const char *src, char *dst);
extern int scan_rfcdata(const char *src, char *dst, int len);

void out(const char *msg) { write(1,msg,strlen(msg)); }

int fromhex(char c)
{
	if (c>='0' && c<='9') return c-'0';
	if (c>='A' && c<='F') return c-'A'+10;
	if (c>='a' && c<='f') return c-'a'+10;

	return -1;
}

int scan_hex(const char *src, char *dst)
{
	const unsigned char *s=(const unsigned char *)src;
	unsigned written=0,i;

	for (i=0; s[i]; ++i) {
		int j;
		
		j=fromhex(s[i]); if (j<0) break;
		dst[written]=j<<4;

		j=fromhex(s[i+1]); if (j<0) break;
		dst[written]|=j;

		++i; ++written;
	}

	return i;
}

int scan_rfcdata(const char *src, char *dst, int len)
{
	char *x=(char*)src;
	int i=0;

	if (len == 0) {
		/* 0x0102030405060708090a0b0c0d0e0f10111213141516171819 */
		if (x[0] == '0' && x[1] == 'x') {
			i+=scan_hex(x+2, dst);
		} else {
			i+=fmt_str(dst, src);
		}
	} else {
		/* if 0x01, read sring as hexvalue */
		if (x[0] == '0' && x[1] == 'x') {
			if (scan_hex(x+2, dst) == 2) memset(dst+1, dst[0], len-1);
			i+=len;
		} else {
			i+=fmt_strn(dst, src, len);
		}
	}

	return i;
}

void show_result(char *v1, char *v2, int len)
{
	int x=strncmp(v1, v2, len);
	int t=isatty(1);

	if (t && x) out("\e[1m");
	out("0x");out(v1);
	if (x) { out(" != "); } else { out(" == "); }
	out("0x");out(v2);
	if (t && x) out("\e[0m");
	out("\n");

	return;
}

int main(void)
{
	unsigned char digest[20], encrypted[41];
	char key[BUFSIZE], data[BUFSIZE];
	int i,j;

	for (i=0;test_hmac_16[i].key;i++) {
		char *e;
		out(test_hmac_16[i].name);out(":\n");
		dozero(key);  scan_rfcdata(test_hmac_16[i].key,  key,  test_hmac_16[i].keylen);
		dozero(data); scan_rfcdata(test_hmac_16[i].data, data, test_hmac_16[i].datalen);

		e=encrypted; memset(encrypted, 0, 41);
		hmac_md5(data, key, digest);
		for (j=0; j<16; j++) {
			*e = hextab[digest[j]/16];e++;
			*e = hextab[digest[j]%16];e++;
		} *e=0;
		show_result(encrypted, test_hmac_16[i].test1+2, 32);
	}

	out("\n");

	for (i=0;test_hmac_20[i].key;i++) {
		char *e;
		out(test_hmac_20[i].name);out(":\n");
		dozero(key);  scan_rfcdata(test_hmac_20[i].key,  key,  test_hmac_20[i].keylen);
		dozero(data); scan_rfcdata(test_hmac_20[i].data, data, test_hmac_20[i].datalen);

		e=encrypted; memset(encrypted, 0, 41);
		hmac_sha1(data, key, digest);
		for (j=0; j<20; j++) {
			*e = hextab[digest[j]/16];e++;
			*e = hextab[digest[j]%16];e++;
		} *e=0;
		show_result(encrypted, test_hmac_20[i].test1+2, 40);

		e=encrypted; memset(encrypted, 0, 41);
		hmac_rmd160(data, key, digest);
		for (j=0; j<20; j++) {
			*e = hextab[digest[j]/16];e++;
			*e = hextab[digest[j]%16];e++;
		} *e=0;
		show_result(encrypted, test_hmac_20[i].test2+2, 40);
	}

	return 0;
}
