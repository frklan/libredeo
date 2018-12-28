target = libredeo
src = $(wildcard *.cpp) \
			$(wildcard src/*.cpp)
hdr = $(wildcard *.h) \
			$(wildcard src/*.h)
obj = $(src:.cpp=.o) 
dep = $(obj:.o=.d)

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
	ar -r $(target).a $(obj)

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
	cp $(target).a $(DESTDIR)$(PREFIX)/lib/$(target).a
	
	mkdir -p $(DESTDIR)$(PREFIX)/include/$(target)
	cp $(hdr) $(DESTDIR)$(PREFIX)/include/$(target)/

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/$(target).a
	rm -rf $(DESTDIR)$(PREFIX)/include/$(target)/