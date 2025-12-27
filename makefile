all:Build

Build:
	g++ -g -c GeometryHelper.cpp
	g++ -g -c RenderingUtils.cpp
	g++ -g -c Metric.cpp
	g++ -g Renderer.cpp -w -lSDL2 -o Renderer GeometryHelper.o RenderingUtils.o Metric.o

build_run:Build
	Renderer

clean:
	rm -rf Renderer
