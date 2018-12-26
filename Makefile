target = ht2
tagetlib = libheliumtimer2.a
src = $(wildcard *.cpp) \
			$(wildcard src/*.cpp)
hdr = $(wildcard *.h) \
			$(wildcard src/*.h)
obj = $(src:.cpp=.o) 
dep = $(obj:.o=.d)

ifeq ($(PREFIX), undefined)
	PREFIX = /usr/local
endif

ifeq ($(CXX), undefined)
  CXX = g++-7
endif

LDFLAGS = 

CXXFLAGS = -std=c++17 -g -Wall 

$(target): $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS) $(CXXFLAGS)

-include $(dep)

%.d: %.c
	@$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

lib: $(target)
	ar -rv $(tagetlib) $(obj) 
	ar -d $(tagetlib) main.o app.o

.PHONY: clean
clean:
	rm -rf $(obj) $(target)* $(tagetlib)

.PHONY: cleandep
cleandep:
	rm -f $(dep)


.PHONY: install
install: $(target)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/$(target)

.PHONY: lib-install
lib-install: $(target) lib
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	cp $(tagetlib) $(DESTDIR)$(PREFIX)/lib/$(tagetlib)
	
	mkdir -p $(DESTDIR)$(PREFIX)/include
	cp $(hdr) $(DESTDIR)$(PREFIX)/include

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(target)
	rm -f $(DESTDIR)$(PREFIX)/lib/$(tagetlib)