SHELL       := /bin/sh

# Support out-of-tree builds
VPATH       := .

prefix      := /usr/local
exec_prefix := ${prefix}
datarootdir := ${prefix}/share
mandir      := ${datarootdir}/man

INSTALL      := /usr/bin/install -c
INSTALL_DATA := ${INSTALL} -m 644

MAN1        := znc.1.gz znc-buildmod.1.gz znc-config.1.gz

ifneq "$(V)" ""
VERBOSE=1
endif
ifeq "$(VERBOSE)" ""
Q=@
E=@echo
else
Q=
E=@\#
endif

all: $(MAN1)

%.1.gz: %.1 Makefile
	$(E) Packing man page $@...
	$(Q)gzip -9 <$< >$@

clean:
	-rm -f $(MAN1)

install: $(MAN1)
	mkdir -p $(DESTDIR)$(mandir)/man1
	$(INSTALL_DATA) $(MAN1) $(DESTDIR)$(mandir)/man1

uninstall:
	for file in $(MAN1) ; do \
		rm $(DESTDIR)$(mandir)/man1/$$file || exit 1 ; \
	done
	rmdir $(DESTDIR)$(mandir)/man1
