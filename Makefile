target = ht2
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

.PHONY: clean
clean:
	rm -rf $(obj) $(target)*

.PHONY: cleandep
cleandep:
	rm -f $(dep)


.PHONY: install
install: $(target)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/$(target)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(target)