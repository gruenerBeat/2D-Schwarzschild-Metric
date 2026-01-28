CXX := g++
CXXFLAGS := -Wno-narrowing
LDFLAGS :=-w -lSDL2 

target := Renderer
source := $(wildcard *.cpp)
object := $(patsubst %.cpp,%.o,$(source))

$(info $(source))
$(info $(object))

.PHONY: all
all: $(target)

%:%.cpp
%:%.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -g -c $<

$(target): $(object)
	echo Hallo
	$(CXX) $(LDFLAGS) -o $@ $^

.PHONY: build_run
build_run: $(target)
	$(target)

.PHONY: clean
clean:
	rm -rf $(target) $(object)
