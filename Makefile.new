# Tino Reichardt <der@mcmilk.de>
#
# - change it to fit your needs
# - see also params.h

MANDIR	= $(DESTDIR)/usr/share/man
DEST	= $(DESTDIR)/command
GROUP	= nogroup

ARCH	= $(shell uname -m | sed -e 's/i[4-9]86/i386/' -e 's/armv[3-6][lb]/arm/')
DIET	= $(shell basename `which diet 2>/dev/null` 2>/dev/null)

# compiler
CC	= $(DIET) gcc
CFLAGS	= -Os -Wall -pipe -fomit-frame-pointer
LDFLAGS	= -s -lcrypt

# extraflags
CFLAGS += -mpreferred-stack-boundary=2 -march=$(ARCH)
#CFLAGS += -W -Wchar-subscripts -Wmissing-prototypes -Wmissing-declarations -Wswitch -Wredundant-decls -Wunused
#CFLAGS += -Werror
#CFLAGS += -funroll-loops

# strip
STRIP	= strip -R .note -R .comment
STRIPx	= strip -x -R .note -R .comment

OBJS	= buffer.o buffer_flush.o buffer_put.o buffer_write.o \
die1.o die111.o die2.o die_env.o die_msg.o die_shell.o \
do_checkpasswd.o do_checkqpasswd.o doexit.o doname.o douser.o dozero.o \
fd_size.o fd_write.o fmt_str.o fmt_strn.o fmt_u32.o hextab.o case_diffb.o \
hmac_md5.o hmac_rmd160.o hmac_sha1.o log.o lognl.o m_checkpassword.o \
m_multicheckpw.o m_qpasswd.o maildirmake.o main.o md5.o mmap_file.o debug.o \
open_file.o qpasswd_gethome.o qpasswd_getpass.o qpasswd_lastpop_check.o \
qpasswd_lastpop_update.o read3.o rmd160.o scan_i32.o scan_ip4.o scan_plusminus.o \
scan_u32.o sha1.o sig_catch.o sig_pipedefault.o switch2user.o usage.o zeroall.o

OBJS2	= dozero.o fmt_str.o fmt_strn.o hextab.o m_tests.o test_cases.o
PRGS	= multicheckpw test-cases

all:	it

it:	$(PRGS)
setup:  it install
again:	clean setup

multicheckpw: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)
	$(STRIP) $@
	for i in checkpasswd checkqpasswd qpasswd; do \
	 ( test -f $$i || ln -s multicheckpw $$i ) \
	done

test-cases: $(OBJS2)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS2)
	$(STRIP) $@

check:	test-cases
	test-cases > test-cases.computed
	@echo If the following test fails, send a mail to crypto@mcmilk.de!
	diff test-cases.wanted test-cases.computed
	@echo The test was OKAY!

clean:
	rm -f $(OBJS) $(OBJS2) $(OBJS3) $(PRGS) checkpasswd checkqpasswd qpasswd test-cases.computed

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
	$(STRIPx) $@

install: uninstall
	test -d $(MANDIR)/man5 || mkdir -p $(MANDIR)/man5
	test -d $(MANDIR)/man8 || mkdir -p $(MANDIR)/man8
	man/install-pages.sh $(MANDIR)
	cp -f multicheckpw $(DEST)/multicheckpw
	chown root.$(GROUP) $(DEST)/multicheckpw
	chmod 4511 $(DEST)/multicheckpw
	ln -s multicheckpw $(DEST)/checkpasswd
	ln -s multicheckpw $(DEST)/checkqpasswd
	ln -s multicheckpw $(DEST)/qpasswd

uninstall:
	rm -f \
	$(DEST)/qpasswd \
	$(DEST)/checkpasswd \
	$(DEST)/checkqpasswd \
	$(DEST)/multicheckpw \
	$(MANDIR)/man5/qpasswd.5 \
	$(MANDIR)/man8/checkpasswd.8 \
	$(MANDIR)/man8/checkqpasswd.8 \
	$(MANDIR)/man8/multicheckpw.8
