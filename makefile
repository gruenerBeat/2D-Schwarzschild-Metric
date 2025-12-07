# Files
S_FILES=Simulation.cpp

# Output
EXEC=simul

# Build settings
CC=g++
# SDL options
CC_SDL=-lSDL2

all:Build

Build:
	g++ -c GeometryHelper.cpp
	g++ -c ScreenHelper.cpp
	$(CC) $(S_FILES) -w $(CC_SDL) -o $(EXEC) GeometryHelper.o ScreenHelper.o

build_run:Build
	$(EXEC)

clean:
	rm -rf simul
