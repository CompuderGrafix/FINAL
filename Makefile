#! /usr/bin/make

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
TARGETS = $(basename $(SOURCES))

INIT_SHADER = Common/InitShader.o
INIT_MODEL =  SpelchkCommon/Model.o
INIT_CAMERA = SpelchkCommon/Camera.o
KINECTLIB = SpelchkCommon/KinectInator.so
INIT_SCENEGRAPH = SpelchkCommon/SceneGraph.o

CXXOPTS = -O2 -g -Wall -fmessage-length=0
CXXDEFS = -DFREEGLUT_STATIC -DGLEW_STATIC
CXXINCS = -Iinclude -ISpelchkCommon -Ikinect/OpenNI/Include -I/usr/include/ni  

CXXFLAGS = $(CXXOPTS) $(CXXDEFS) $(CXXINCS)

LDLIBS = -L/usr/local/lib -LSpelchkCommon -lGLEW -lglut -lGL -lXmu -lX11 -lm -lSOIL 

LDFLAGS = $(LDOPTS) $(LDDIRS) $(LDLIBS)

DIRT = $(wildcard */*.o */*.so */*.d *.i *~ */*~ *.log)

#-----------------------------------------------------------------------------

.PHONY: Makefile

default all: $(KINECTLIB) $(TARGETS)

$(TARGETS): $(INIT_SHADER) $(INIT_CAMERA) $(INIT_MODEL) $(INIT_SCENEGRAPH) 

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

#-----------------------------------------------------------------------------

%.i: %.cpp
	$(CXX) -E $(CXXFLAGS) $< | uniq > $@

#-----------------------------------------------------------------------------

$(KINECTLIB): SpelchkCommon/KinectInator.cpp
	g++ -fPIC -MD -MP -MT "./SpelchkCommon/KinectInator.d SpelchkCommon/KinectInator.o" -c -msse3 -O2 -DNDEBUG -ISpelchkCommon -Ikinect/OpenNI/Include -I/usr/include/ni -o SpelchkCommon/KinectInator.o SpelchkCommon/KinectInator.cpp
	g++ -shared -o SpelchkCommon/KinectInator.so SpelchkCommon/KinectInator.o -lboost_system-mt -lboost_thread-mt -Lkinect/OpenNI/Lib -Lkinect/OpenNI/Samples/Bin/x64-Release -lOpenNI

clean:
	$(RM) $(DIRT)

rmtargets:
	$(RM) $(TARGETS)

clobber: clean rmtargets
