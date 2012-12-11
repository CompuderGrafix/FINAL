FLAGS = -g -Wall -std=c++11
LDLIBS = -lGL -lGLU -lGLEW -lglut -lXmu -lX11 -lSOIL -lm

test: src/test.cpp InitShader.o Camera.o Model.o
	g++ $(FLAGS) $(LDLIBS) -o $@ InitShader.o Camera.o Model.o src/$@.cpp
	
temp: src/temp.cpp InitShader.o Camera.o Model.o
	g++ $(FLAGS) $(LDLIBS) -o $@ InitShader.o Camera.o Model.o src/$@.cpp

sphere_with_light_sources: src/sphere_with_light_sources.cpp InitShader.o
	g++ $(FLAGS) $(LDLIBS) -o $@ InitShader.o src/$@.cpp

fly_through: src/fly_through.cpp InitShader.o
	g++ -$(FLAGS) $(LDLIBS) -o $@ InitShader.o src/$@.cpp 

rotating_cube: src/rotating_cube.cpp InitShader.o
	g++ $(FLAGS) $(LDLIBS) -o $@ InitShader.o src/$@.cpp 

triangle: src/triangle.cpp InitShader.o
	g++ $(FLAGS) $(LDLIBS) -o $@ InitShader.o src/$@.cpp 

InitShader.o: src/InitShader.cpp
	g++ $(FLAGS) $(LDLIBS) -c -o $@ src/InitShader.cpp
	
Camera.o: src/Camera.cpp
	g++ $(FLAGS) $(LDLIBS) -c -o $@ src/Camera.cpp
	
Model.o: src/Model.cpp
	g++ $(FLAGS) $(LDLIBS) -c -o $@ src/Model.cpp

clean:
	rm -f InitShader.o
	rm -f Camera.o
	rm -f test
	rm -f sphere_with_light_sources
	rm -f fly_through
	rm -f rotating_cube
	rm -f triangle
