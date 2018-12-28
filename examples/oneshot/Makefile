target = oneshot

src = $(wildcard *.cpp) 
obj = $(src:.cpp=.o) 
dep = $(obj:.o=.d)

ifndef CXX
  CXX = g++-7
endif

LDFLAGS = -L../../ -lredeo
CXXFLAGS = -std=c++17 -g -Wall -I ../../
	
-include $(dep)

%.d: %.c
	@$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

$(target): $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS) $(CXXFLAGS)


.PHONY: clean
clean:
	rm -rf $(obj) $(target)