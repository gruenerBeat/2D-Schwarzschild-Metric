# Files
S_FILES=Simulation.cpp

# Output
EXEC=simul

# Build settings
CC=g++
# SDL options
CC_SDL=-lSDL2
CC_TTF=-lSDL2_ttf

all:Build

Build:
	g++ -c GeometryHelper.cpp
	g++ -c ScreenHelper.cpp
	g++ -c Metric.cpp
	$(CC) $(S_FILES) -w $(CC_SDL) -w $(CC_TTF) -o $(EXEC) GeometryHelper.o ScreenHelper.o Metric.o

build_run:Build
	$(EXEC)

clean:
	rm -rf simul
