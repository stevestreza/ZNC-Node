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
sbindir     := ${exec_prefix}/sbin
localstatedir := ${prefix}/var
CXX      := g++
# CXXFLAGS are for the main binary, so don't use them here, use MODFLAGS instead
MODFLAGS :=    -D_FORTIFY_SOURCE=2 -O2 -DHAVE_IPV6 -Wall -W -Wno-unused-parameter -Woverloaded-virtual -Wshadow -DCSOCK_USE_POLL -DHAVE_LSTAT -I/Users/syco/Programs/include   -DHAVE_LIBSSL -fPIC -I$(srcdir)/..
MODLINK  := -bundle -flat_namespace -undefined suppress
LDFLAGS  := 
# LIBS are not and should not be used in here.
# The znc binary links already against those.
# LIBS     := -ldl  -L/Users/syco/Programs/lib -lssl -lcrypto -lz  
PERL     := 
SWIG     := 
MODDIR   := ${exec_prefix}/lib/znc
DATADIR  := ${datarootdir}/znc
LIBZNC   := 
LIBZNCDIR:= 
INSTALL         := /usr/bin/install -c
INSTALL_PROGRAM := ${INSTALL}
INSTALL_SCRIPT  := ${INSTALL}
INSTALL_DATA    := ${INSTALL} -m 644

TCL_FLAGS:= 

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

ifneq "$(LIBZNC)" ""
LDFLAGS += -L.. -lznc -Wl,-rpath,$(LIBZNCDIR)
endif

FILES    := $(notdir $(wildcard $(srcdir)/*.cpp))
# If extra is enabled
ifeq "no" "yes"
FILES    += $(addprefix extra/, $(notdir $(wildcard $(srcdir)/extra/*.cpp)))
endif

FILES    := $(basename $(FILES))

ifeq "" "1"
FILES    := $(foreach file, $(FILES), \
	$(if $(shell grep REQUIRESSL $(srcdir)/$(file).cpp), \
		, \
		$(basename $(file)) \
	))
endif

ifneq "$(PERL)" ""
# We execute this now so that we see the 'beauty' of these flags in make's output
PERLCOMMON   := $(shell $(PERL) -MExtUtils::Embed -e perl_inc -e ldopts)
# Perl API is ugly, casting string literals to char* and redeclaring functions :(
PERLCOMMON   += -Wno-write-strings -Wno-redundant-decls -Wno-missing-declarations
# This is for SWIG
PERLCOMMON   += -DSWIG_TYPE_TABLE=znc
modperlFLAGS := $(PERLCOMMON)
# Find additional headers for out-of-tree build
modperlFLAGS += -I.
PERLHOOK     := modperl_install
else
FILES    := $(shell echo $(FILES) | sed -e "s/modperl//")
endif

ifeq "" ""
FILES    := $(shell echo $(FILES) | sed -e "s:extra/saslauth::")
endif
saslauthFLAGS := -lsasl2

ifeq "" ""
FILES    := $(shell echo $(FILES) | sed -e "s:extra/charset::")
endif
charsetFLAGS := 

ifeq "$(TCL_FLAGS)" ""
FILES       := $(shell echo $(FILES) | sed -e "s:extra/modtcl::")
else
TCLHOOK     := modtcl_install
endif
modtclFLAGS := $(TCL_FLAGS)


TARGETS  := $(addsuffix .so, $(FILES))

CLEAN    := *.so extra/*.so modperl/ZNC.so modperl/modperl_wrap.cxx modperl/ZNC.pm

.PHONY: all clean install modperl_install

all: $(TARGETS)

install: all create_install_dir install_metadirs $(PERLHOOK) $(TCLHOOK)
	$(INSTALL_PROGRAM) $(TARGETS) $(DESTDIR)$(MODDIR)

create_install_dir:
	mkdir -p $(DESTDIR)$(MODDIR)
	mkdir -p $(DESTDIR)$(DATADIR)
	rm -rf $(DESTDIR)$(MODDIR)/*.so

install_metadirs: create_install_dir
	for a in $(srcdir)/*; do \
		d=$$(echo $$a | sed -e "s:$(srcdir)/::g"); \
		if [ -d $$a ] && [ -f $${d}.so ]; then \
			cp -Rp $$a $(DESTDIR)$(DATADIR); \
		fi \
	done

clean:
	rm -rf $(CLEAN)

%.so: %.cpp Makefile
	@mkdir -p .depend
	@mkdir -p extra
	$(E) Building $(if $(filter %extra/,$(dir $<)),extra )module $(notdir $(basename $@))...
	$(Q)$(CXX) $(MODFLAGS) $(LDFLAGS) $(MODLINK) -o $@ $< $($(notdir $(basename $@))FLAGS) -MMD -MF .depend/$(notdir $@).dep

modperl.so: modperl/ZNC.so modperl/swigperlrun.h
modperl/ZNC.so: modperl/ZNC.cpp Makefile
	$(E) Building ZNC perl bindings library...
	$(Q)$(CXX) $(MODFLAGS) $(LDFLAGS) $(MODLINK) -I$(srcdir) $(PERLCOMMON) -o $@ $<
ifneq "$(SWIG)" ""
modperl/swigperlrun.h:
	@mkdir -p modperl
	$(Q)$(SWIG) -perl5 -c++ -shadow -external-runtime $@
modperl/ZNC.cpp: modperl/modperl.i Makefile modperl/module.h modperl/CString.i
	$(E) Generating ZNC API for perl...
	@mkdir -p modperl
	$(Q)$(SWIG) -perl5 -c++ -shadow -outdir modperl -I$(srcdir) -w362,315,401,402 -o $@.warn $<
	$(Q)$(PERL) -pe '$$x==2 and print "items+=0;" and $$x=3;/^XS\(SWIG_init\)/ and $$x=1;$$x==1 and /dXSARGS/ and $$x=2' $@.warn > $@
	$(Q)rm -rf $@.warn
endif

modperl_install: create_install_dir install_metadirs
	for i in $(srcdir)/*.pm; do \
		test -r $$i && $(INSTALL_DATA) $$i $(DESTDIR)$(MODDIR); true; \
	done
	for a in $(srcdir)/*; do \
		if [ -d $$a ] && [ -f $${a}.pm ]; then \
			cp -Rp $$a $(DESTDIR)$(DATADIR); \
		fi \
	done
	rm -rf $(DESTDIR)$(DATADIR)/modperl
	mkdir -p $(DESTDIR)$(MODDIR)/modperl
	$(INSTALL_PROGRAM) $(srcdir)/modperl/ZNC.so $(DESTDIR)$(MODDIR)/modperl
	$(INSTALL_DATA) $(srcdir)/modperl/ZNC.pm $(DESTDIR)$(MODDIR)/modperl
	$(INSTALL_DATA) $(srcdir)/modperl/startup.pl $(DESTDIR)$(MODDIR)/modperl

modtcl_install:
	mkdir -p $(DESTDIR)$(DATADIR)/modtcl/
	$(INSTALL_DATA) $(srcdir)/extra/modtcl.tcl $(srcdir)/extra/binds.tcl $(DESTDIR)$(DATADIR)/modtcl/

uninstall:
	# Yes, we are lazy, just remove everything in there
	rm -rf $(DESTDIR)$(MODDIR)/*
	rm -rf $(DESTDIR)$(DATADIR)/*
	rmdir $(DESTDIR)$(MODDIR)
	rmdir $(DESTDIR)$(DATADIR)

-include $(wildcard .depend/*.dep)
