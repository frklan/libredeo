target = libredeo
src = $(wildcard *.cpp) \
			$(wildcard src/*.cpp)
hdr = $(wildcard *.h) \
			$(wildcard src/*.h)
obj = $(src:.cpp=.o) 
dep = $(obj:.o=.d)
VERSION = $(shell git describe --tags --always --dirty)

ifndef PREFIX
	PREFIX = /usr/local
endif

ifndef CXX
  CXX = g++-7
endif

LDFLAGS = 
CXXFLAGS = -std=c++17 -g -Wall 

-include $(dep)

%.d: %.c
	@$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

$(target): $(obj)
	ar -r $(target)-$(VERSION).a $(obj)

examples: $(target) 
	$(MAKE) -C examples/

.PHONY: clean
clean:
	rm -rf $(obj) $(target).a
	$(MAKE) clean -C examples/ 

.PHONY: cleandep
cleandep:
	rm -f $(dep)

.PHONY: install
install: $(target)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	cp $(target)-$(VERSION).a $(DESTDIR)$(PREFIX)/lib/$(target)-$(VERSION).a
	rm -f $(DESTDIR)$(PREFIX)/lib/$(target).a
	ln -s $(DESTDIR)$(PREFIX)/lib/$(target)-$(VERSION).a $(DESTDIR)$(PREFIX)/lib/$(target).a
	
	mkdir -p $(DESTDIR)$(PREFIX)/include/$(target)
	cp $(hdr) $(DESTDIR)$(PREFIX)/include/$(target)/

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/$(target).a
	rm -f $(DESTDIR)$(PREFIX)/lib/$(target)-$(VERSION).a
	rm -rf $(DESTDIR)$(PREFIX)/include/$(target)/