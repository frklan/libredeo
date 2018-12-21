target = ht2
tagetlib = libheliumtimer2.a
src = $(wildcard *.cpp) \
			$(wildcard src/*.cpp)
obj = $(src:.cpp=.o) 
dep = $(obj:.o=.d)
PREFIX = /usr/local

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
install: $(target) lib
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/$(target)

	mkdir -p $(DESTDIR)$(PREFIX)/lib
	cp $(tagetlib) $(DESTDIR)$(PREFIX)/lib/$(tagetlib)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(target)
	rm -f $(DESTDIR)$(PREFIX)/lib/$(tagetlib)