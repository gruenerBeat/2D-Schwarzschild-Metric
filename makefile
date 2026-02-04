all:Build

Build:
	g++ -g -c GeometryHelper.cpp
	g++ -g -c Screen.cpp
	g++ -g Renderer.cpp -w -lSDL2 -o Renderer GeometryHelper.o Screen.o

build_run:Build
	Renderer

clean:
	rm -rf Renderer
