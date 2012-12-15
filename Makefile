#! /usr/bin/make

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
TARGETS = $(basename $(SOURCES))

INIT_SHADER = Common/InitShader.o
INIT_MODEL =  SpelchkCommon/Model.o
INIT_CAMERA = SpelchkCommon/Camera.o
KINECT = SpelchkCommon/KinectInator.o
INIT_SCENEGRAPH = SpelchkCommon/SceneGraph.o

CXXOPTS = -O2 -g -Wall -fmessage-length=0 -msse3 
CXXDEFS = -DFREEGLUT_STATIC -DGLEW_STATIC
CXXINCS = -Iinclude -I~/kinect/OpenNI/Include -I/usr/include/ni

CXXFLAGS = $(CXXOPTS) $(CXXDEFS) $(CXXINCS)

LDLIBS = -L/usr/local/lib -lboost -lboost_threads -lboost_functions -lGLEW -lglut -lGL -lXmu -lX11 -lm -lSOIL -lOpenNI

LDFLAGS = $(LDOPTS) $(LDDIRS) $(LDLIBS)

DIRT = $(wildcard *.o *.i *~ */*~ *.log)

#-----------------------------------------------------------------------------

.PHONY: Makefile

default all: $(TARGETS)

$(TARGETS): $(INIT_SHADER) $(INIT_CAMERA) $(INIT_MODEL) $(INIT_SCENEGRAPH) $(KINECT)

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

#-----------------------------------------------------------------------------

%.i: %.cpp
	$(CXX) -E $(CXXFLAGS) $< | uniq > $@

#-----------------------------------------------------------------------------

clean:
	$(RM) $(DIRT)

rmtargets:
	$(RM) $(TARGETS)

clobber: clean rmtargets
