SHELL       := /bin/sh

# Support out-of-tree builds
srcdir      := .
VPATH       := .

prefix      := /usr/local
exec_prefix := ${prefix}
datarootdir := ${prefix}/share
bindir      := ${exec_prefix}/bin
datadir     := ${datarootdir}
sysconfdir  := ${prefix}/etc
libdir      := ${exec_prefix}/lib
includedir  := ${prefix}/include
sbindir     := ${exec_prefix}/sbin
localstatedir := ${prefix}/var
CXX      := g++
CXXFLAGS :=    -D_FORTIFY_SOURCE=2 -O2 -DHAVE_IPV6 -Wall -W -Wno-unused-parameter -Woverloaded-virtual -Wshadow -DCSOCK_USE_POLL -DHAVE_LSTAT -I/Users/syco/Programs/include   -DHAVE_LIBSSL -D_MODDIR_=\"${exec_prefix}/lib/znc\" -D_DATADIR_=\"${datarootdir}/znc\"
LDFLAGS  := 
LIBS     := -ldl  -L/Users/syco/Programs/lib -lssl -lcrypto -lz  
LIBZNC   := 
LIBZNCDIR:= 
MODDIR   := ${exec_prefix}/lib/znc
DATADIR  := ${datarootdir}/znc
PKGCONFIGDIR := $(libdir)/pkgconfig
INSTALL         := /usr/bin/install -c
INSTALL_PROGRAM := ${INSTALL}
INSTALL_SCRIPT  := ${INSTALL}
INSTALL_DATA    := ${INSTALL} -m 644

LIB_SRCS  := ZNCString.cpp Csocket.cpp znc.cpp User.cpp IRCSock.cpp Client.cpp DCCBounce.cpp \
	DCCSock.cpp Chan.cpp Nick.cpp Server.cpp Modules.cpp MD5.cpp Buffer.cpp Utils.cpp \
	FileUtils.cpp HTTPSock.cpp Template.cpp ClientCommand.cpp Socket.cpp SHA256.cpp \
	WebModules.cpp Listener.cpp
BIN_SRCS  := main.cpp
LIB_OBJS  := $(patsubst %cpp,%o,$(LIB_SRCS))
BIN_OBJS  := $(patsubst %cpp,%o,$(BIN_SRCS))
CLEAN     := znc *.o core core.*
DISTCLEAN := Makefile config.log config.status znc-config znc-buildmod .depend \
	modules/.depend modules/Makefile man/Makefile znc.pc znc-uninstalled.pc

ifneq "$(V)" ""
VERBOSE=1
endif
ifeq "$(VERBOSE)" ""
Q=@
E=@echo
C=-s
else
Q=
E=@\#
C=
endif

.PHONY: all man modules clean distclean install

all: znc man modules $(LIBZNC)
	@echo ""
	@echo " ZNC was successfully compiled. You may use"
	@echo " '$(MAKE) install' to install ZNC to '$(prefix)'."
	@echo " You can then use '$(bindir)/znc --makeconf'"
	@echo " to generate a config file."
	@echo ""
	@echo " If you need help with using ZNC, please visit our wiki at:"
	@echo "   http://znc.in"

ifeq "$(LIBZNC)" ""
OBJS := $(BIN_OBJS) $(LIB_OBJS)

znc: $(OBJS)
	$(E) Linking znc...
	$(Q)$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
else
znc: $(BIN_OBJS) $(LIBZNC)
	$(E) Linking znc...
	$(Q)$(CXX) $(LDFLAGS) -o $@ $(BIN_OBJS) -L. -lznc -Wl,-rpath,$(LIBZNCDIR) $(LIBS)

$(LIBZNC): $(LIB_OBJS)
	$(E) Linking $(LIBZNC)...
	$(Q)$(CXX) $(LDFLAGS) -shared -o $@ $(LIB_OBJS) $(LIBS)
endif

man:
	@$(MAKE) -C man $(C)

modules: $(LIBZNC)
	@$(MAKE) -C modules $(C)

clean:
	rm -rf $(CLEAN)
	@$(MAKE) -C modules clean;
	@$(MAKE) -C man clean

distclean: clean
	rm -rf $(DISTCLEAN)

%.o: %.cpp Makefile
	@mkdir -p .depend
	$(E) Building $@...
	$(Q)$(CXX) $(CXXFLAGS) -c -o $@ $< -MMD -MF .depend/$@.dep

install: znc $(LIBZNC)
	mkdir -p $(DESTDIR)$(bindir)
	mkdir -p $(DESTDIR)$(includedir)/znc
	mkdir -p $(DESTDIR)$(PKGCONFIGDIR)
	mkdir -p $(DESTDIR)$(MODDIR)
	mkdir -p $(DESTDIR)$(DATADIR)
	cp -Rp $(srcdir)/webskins $(DESTDIR)$(DATADIR)
	$(INSTALL_PROGRAM) znc $(DESTDIR)$(bindir)
	$(INSTALL_SCRIPT) znc-config $(DESTDIR)$(bindir)
	$(INSTALL_SCRIPT) znc-buildmod $(DESTDIR)$(bindir)
	$(INSTALL_DATA) $(srcdir)/*.h $(DESTDIR)$(includedir)/znc
	$(INSTALL_DATA) znc.pc $(DESTDIR)$(PKGCONFIGDIR)
	@$(MAKE) -C modules install DESTDIR=$(DESTDIR);
	if test -n "$(LIBZNC)"; then \
		mkdir -p $(DESTDIR)$(LIBZNCDIR) || exit 1 ; \
		$(INSTALL_PROGRAM) $(LIBZNC) $(DESTDIR)$(LIBZNCDIR) || exit 1 ; \
	fi
	@$(MAKE) -C man install DESTDIR=$(DESTDIR)

uninstall:
	rm $(DESTDIR)$(bindir)/znc
	rm $(DESTDIR)$(bindir)/znc-config
	rm $(DESTDIR)$(bindir)/znc-buildmod
	rm $(DESTDIR)$(includedir)/znc/*.h
	rm $(DESTDIR)$(PKGCONFIGDIR)/znc.pc
	rm -rf $(DESTDIR)$(MODDIR)/webskins
	if test -n "$(LIBZNC)"; then \
		rm $(DESTDIR)$(LIBZNCDIR)/$(LIBZNC) || exit 1 ; \
		rmdir $(DESTDIR)$(LIBZNCDIR) || exit 1 ; \
	fi
	@$(MAKE) -C man uninstall DESTDIR=$(DESTDIR)
	@if test -n "modules"; then \
		$(MAKE) -C modules uninstall DESTDIR=$(DESTDIR); \
	fi
	rmdir $(DESTDIR)$(bindir)
	rmdir $(DESTDIR)$(includedir)/znc
	rmdir $(DESTDIR)$(PKGCONFIGDIR)
	@echo "Successfully uninstalled, but empty directories were left behind"

-include $(wildcard .depend/*.dep)
