
CXX				= g++
CXXFLAGS		= -std=c++11 -Wall -g 
LIBS			= -lSDL2
TESTLIBS		= -lSDL2 -lgtest -lpthread -lm
INCLUDES		= -Iinclude

BUILDDIR		= build
UTILDIR			= src/utils
UTIL			= $(wildcard $(UTILDIR)/*.cpp)
UTILOBJS		= $(patsubst $(UTILDIR)/%.cpp,$(BUILDDIR)/%.o,$(UTIL))
MAPDIR			= src/map
MAP				= $(wildcard $(MAPDIR)/*.cpp)
MAPOBJS			= $(patsubst $(MAPDIR)/%.cpp,$(BUILDDIR)/%.o,$(MAP))
OBJCETDIR		= src/object
OBJECT			= $(wildcard $(OBJCETDIR)/*.cpp)
OBJECTOBJS		= $(patsubst $(OBJCETDIR)/%.cpp,$(BUILDDIR)/%.o,$(OBJECT))
SIMULATORDIR	= src/simulator
SIMULATOR		= $(wildcard $(SIMULATORDIR)/*.cpp)
SIMULATOROBJS	= $(patsubst $(SIMULATORDIR)/%.cpp,$(BUILDDIR)/%.o,$(SIMULATOR))
UNITTESTDIR		= unittest
UNITTEST		= $(wildcard $(UNITTESTDIR)/*.cpp)
UNITTESTOBJS	= $(patsubst $(UNITTESTDIR)/%.cpp,$(BUILDDIR)/%.o,$(UNITTEST))

OBJS			= $(UTILOBJS) $(MAPOBJS) $(OBJECTOBJS) $(SIMULATOROBJS)
TESTOBJS		= $(UTILOBJS) $(MAPOBJS) $(OBJECTOBJS) $(UNITTESTOBJS)

EXEC			= simulation
TESTEXEC		= unit_test
VPATH			= $(SIMULATORDIR) $(UTILDIR) $(MAPDIR) $(OBJCETDIR) $(UNITTESTDIR)


all: make_build_dir $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $^ -o $@	$(CXXFLAGS) $(LIBS)

$(TESTEXEC): $(TESTOBJS)
	$(CXX) $^ -o $@	$(CXXFLAGS) $(TESTLIBS)

$(BUILDDIR)/%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCLUDES)


.PHONY: run test clean make_build_dir

test: make_build_dir $(TESTEXEC)
	./$(TESTEXEC)

run: make_build_dir $(EXEC)
	./$(EXEC)

make_build_dir:
	@mkdir -p $(BUILDDIR)

clean:
	@rm -rf $(BUILDDIR) $(EXEC)