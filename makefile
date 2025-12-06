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
	$(CC) $(S_FILES) -w $(CC_SDL) -o $(EXEC)

build_run:Build
	$(EXEC)

clean:
	rm -rf simul
