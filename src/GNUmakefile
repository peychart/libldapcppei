VERSION	:= $$(cat ../.version)
LIBRARY ?= libldapcppei
FILES	?= ldapByteArea ldapBerValue ldapAttribute ldapEntry ldapSet ldapEasyInterface

CC		?= gcc
CXX		?= g++
MAKE_CXXFLAGS	?= -std=c++0x
AR		?= ar
SOURCES 	:= $$(for I in $(FILES) ; do echo $${I}.cpp ; done)
OBJECTS		:= $$(for I in $(FILES) ; do echo $${I}.o ; done)

all: $(LIBRARY).a $(LIBRARY)-dbg.a $(LIBRARY).so $(LIBRARY)-dbg.so

$(LIBRARY).so:
	$(CXX) -fPIC $(MAKE_CXXFLAGS) -I ./libldapcppei -x c++ -shared -I$$PWD -o $(LIBRARY).so.$(VERSION) $(SOURCES) ; chmod 644 $(LIBRARY).so.$(VERSION) ; rm -f $(OBJECTS)
	L=$(LIBRARY).so.$(VERSION) ; while [ $$L != $(LIBRARY).so ] ; do L=$$(echo $$L|sed -e 's/\.[0-9]*$$//') ; ln -sf $(LIBRARY).so.$(VERSION) $$L ; done ; ln -sf $(LIBRARY).so.$(VERSION) $(LIBRARY).so

$(LIBRARY)-dbg.so:
	$(CXX) -fPIC $(MAKE_CXXFLAGS) -I ./libldapcppei -x c++ -shared -g -I$$PWD -o $(LIBRARY)-dbg.so.$(VERSION) $(SOURCES) ; chmod 644 $(LIBRARY)-dbg.so.$(VERSION) ; rm -f $(OBJECTS)
	L=$(LIBRARY)-dbg.so.$(VERSION) ; while [ $$L != $(LIBRARY)-dbg.so ] ; do L=$$(echo $$L|sed -e 's/\.[0-9]*$$//') ; ln -sf $(LIBRARY)-dbg.so.$(VERSION) $$L ; done ; ln -sf $(LIBRARY)-dbg.so.$(VERSION) $(LIBRARY)-dbg.so

$(LIBRARY).a:
	$(CXX) -fPIC $(CFLAGS) $(MAKE_CXXFLAGS) -I ./libldapcppei -x c++ -I$$PWD -c $(SOURCES) ; chmod 644 $(OBJECTS)
	$(AR) qs $(LIBRARY).a $(OBJECTS) ; chmod 644 $(LIBRARY).a ; rm -f $(OBJECTS)
	ln -sf $(LIBRARY).a $(LIBRARY)_static.a

$(LIBRARY)-dbg.a:
	$(CXX) -fPIC $(CFLAGS) $(MAKE_CXXFLAGS) -I ./libldapcppei -x c++ -g -I$$PWD -c $(SOURCES) ; chmod 644 $(OBJECTS)
	$(AR) qs $(LIBRARY)-dbg.a $(OBJECTS) ; chmod 644 $(LIBRARY)-dbg.a ; rm -f $(OBJECTS)
	ln -sf $(LIBRARY)-dbg.a $(LIBRARY)_static-dbg.a

clean:
	@rm -f $(OBJECTS)
	@rm -f $(LIBRARY)-dbg.so* $(LIBRARY)-dbg.a $(LIBRARY)_static-dbg.a
	@rm -f $(LIBRARY).so* $(LIBRARY).a $(LIBRARY)_static.a

