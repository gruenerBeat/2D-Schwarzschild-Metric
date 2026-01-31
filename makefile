CXX := g++
CXXFLAGS := -Wno-narrowing
LDFLAGS :=-w -lSDL2 

target := Renderer
main := Renderer.cpp
source := $(wildcard *.cpp)
source := $(filter-out $(main), $(source))
object := $(patsubst %.cpp,%.o,$(source))

$(info $(source))
$(info $(object))

.PHONY: all
all: $(target)

%:%.cpp
%:%.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -g -c $<

$(target): $(main) $(object)
	echo Hallo
	$(CXX) $(main) $(LDFLAGS) -o $@ $(object)

.PHONY: build_run
build_run: $(target)
	$(target)

.PHONY: clean
clean:
	rm -rf $(target) $(object)
