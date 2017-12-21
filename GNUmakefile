#
# Generating the DEBIAN package:
#   make debian
# or, direct installation:
#   sudo make install
#
VERSION		?= $$(cat .version)
FILES		:= ldapByteArea ldapBerValue ldapAttribute ldapEntry ldapSet ldapEasyInterface
ARCHITECTURE	:= $$(if uname -m|grep -qs 64;then echo amd64;else echo i386;fi)
PREFIX		:= /usr
LIBPREFIX	:= 
CONTROLFILE	?= control
PKGNAME		?= $$(basename $$PWD)

headersDest	?= /$(PREFIX)/include/$(PKGNAME)
srcDest		?= /$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/srv

CC		:= gcc
CXX		:= g++
MAKE_CXXFLAGS	:= -std=c++0x
AR		:= ar
LDCONFIG	:= ldconfig
HEADERS		:= $$(for I in $(FILES) ; do echo $${I}.h ; done)
SOURCES		:= $$(for I in $(FILES) ; do echo $${I}.cpp ; done)

all:
	@(cd src && make)

help:
	@echo "	make [all] (-> compile libs)"
	@echo "	make install"
	@echo "	make uinstall"
	@echo "	make debian (-> $(PKGNAME).deb)"
	@echo "	make dist   (-> \"source\".tgz)"
	@echo "	make doc    (-> \"doc\".tgz)"
	@echo "	make clean"
	@echo "	make info"

clean:
	@(cd src && make clean >/dev/null)
	@rm -rf ./$(PKGNAME) ./$(PKGNAME)-$(VERSION)_$(ARCHITECTURE).deb

install: installHeaders installLibs
	[ -d /etc/ld.so.conf.d ] && echo "$(LIBPREFIX)/lib/" >/etc/ld.so.conf.d/$(PKGNAME).conf && $$LDCONFIG

uninstall:
	rm -f $(LIBPREFIX)/lib/$(PKGNAME)-dbg.so* $(LIBPREFIX)/lib/$(PKGNAME).so*
	rm -f $(LIBPREFIX)/lib/$(PKGNAME)_static-dbg.a $(LIBPREFIX)/lib/$(PKGNAME)_static.a
	rm -f $(LIBPREFIX)/lib/$(PKGNAME)-dbg.a $(LIBPREFIX)/lib/$(PKGNAME).a
	rm -f /etc/ld.so.conf.d/$(PKGNAME).conf
	for I in $(HEADERS) ; do rm -f $(headersDest)/$$I ; done ; if [ -d "$(headersDest)" ] ; then rmdir $(headersDest) ; fi
	for I in $(SOURCES) $(HEADERS) ; do rm -f $(srcDest)/$$I ; done ; if [ -d "$(srcDest)" ] ; then rmdir $(srcDest) ; fi

installHeaders:
	mkdir -p $(headersDest) ; cp -fr src/$(PKGNAME)/* $(headersDest)

installsrc: installHeaders
	mkdir -p $(srcDest) ; for I in $(SOURCES) ; do cp -f src/$$I $(srcDest) ; done
	for I in $(HEADERS) ; do ln -sf $(headersDest)/$$I $(srcDest) ; done

installLibs: all
	mkdir -p $(LIBPREFIX)/lib ; for I in $(PKGNAME).so* $(PKGNAME)-dbg.so* $(PKGNAME).a $(PKGNAME)-dbg.a ; do cp -f src/$$I $(LIBPREFIX)/lib ; done

dist: clean
	@(P=$(PKGNAME) ; V=$(VERSION) ; cd .. ; tar --exclude $$P/webdoc --exclude $$P/.git -chzf $$P-$$V.tgz $$P ; chmod -x $$P-$$V.tgz)

doc:
	@(V=$(VERSION) ; N=$(PKGNAME) ; cd .. ; tar -chzf $${N}_webdoc-$$V.tgz $$N/COPYRIGHT $$N/LICENCE $$N/README $$N/INSTALL $$N/NEW $$N/TODO $$N/*akefile $$N/webdoc ; chmod -x $$N-$$V.tgz)

tmpdebian: all
	rm -rf ./$(PKGNAME) ./$(PKGNAME)-$(VERSION)_$(ARCHITECTURE).deb
	mkdir -p ./$(PKGNAME)/DEBIAN
	mkdir -p ./$(PKGNAME)/$(LIBPREFIX)/lib
	mkdir -p ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/
	mkdir -p ./$(PKGNAME)/$(PREFIX)/share/man/man3/
	mkdir -p ./$(PKGNAME)/$(PREFIX)/include/$(PKGNAME)
	mkdir -p ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/src/

	cp -f NEW ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/changelog.Debian
	gzip -f ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/changelog.Debian
	cp -f COPYRIGHT ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/copyright
	gzip -f ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/copyright
	cp -f LICENCE ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/
	cp -f README ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/README.Debian
	cp -f TODO ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/TODO
	cp -fr man/man3/*.3.gz ./$(PKGNAME)/$(PREFIX)/share/man/man3/
	cp -fr src/$(PKGNAME) ./$(PKGNAME)/$(PREFIX)/include/
	cp -fP src/$(PKGNAME).so* ./$(PKGNAME)/$(LIBPREFIX)/lib/
	cp -fP src/$(PKGNAME).a src/$(PKGNAME)_static.a ./$(PKGNAME)/$(LIBPREFIX)/lib/
	cp -fP src/$(PKGNAME)-dbg.so* ./$(PKGNAME)/$(LIBPREFIX)/lib/
	cp -fP src/$(PKGNAME)-dbg.a src/$(PKGNAME)_static-dbg.a ./$(PKGNAME)/$(LIBPREFIX)/lib/
	(cd src && make clean)
	find . -name $(PKGNAME) -prune -o \( \! -name *~ -print0 \)| cpio -pmd0 ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/src/$(PKGNAME)/
	cp -fr src/$(PKGNAME) ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/src/$(PKGNAME)/src/
	rm -rf ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/src/$(PKGNAME)/webdoc
	rm -rf ./$(PKGNAME)/$(PREFIX)/share/doc/$(PKGNAME)-$(VERSION)/src/$(PKGNAME)/.git
	(cd ./$(PKGNAME) && >DEBIAN/md5sums && find ./$(PREFIX) ./$(LIBPREFIX)/lib -type f -exec md5sum {} \; >>DEBIAN/md5sums)

controlfile: tmpdebian
	@mkdir -p ./$(PKGNAME)/DEBIAN
	@(cat $(CONTROLFILE) | sed -e "s/^Package:.*$$/Package: $(PKGNAME)/" -e "s/^Version:.*$$/Version: $(VERSION)/" -e "s/^Architecture:.*$$/Architecture: $(ARCHITECTURE)/" -e "s/^Filename:.*$$/Filename: $(PKGNAME)-$(VERSION)_$(ARCHITECTURE).deb/" -e "s/^Installed-Size:.*$$/Installed-Size: $$(du -ks .|cut -f1)/" >./$(PKGNAME)/DEBIAN/$(CONTROLFILE))

debian: controlfile
	fakeroot dpkg --build $(PKGNAME) && mv $(PKGNAME).deb $(PKGNAME)-$(VERSION)_$(ARCHITECTURE).deb
	@(cd src && make clean)
	@rm -rf ./$(PKGNAME)

info: controlfile
	@cat ./$(PKGNAME)/DEBIAN/$(CONTROLFILE)
	
