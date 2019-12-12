
CXX				= g++
CXXFLAGS		= -std=c++11 -Wall 
LIBS			= -lSDL2
INCLUDES		= -Iinclude

BUILDDIR		= build
MAPDIR			= src/map
MAP				= $(wildcard $(MAPDIR)/*.cpp)
MAPOBJS			= $(patsubst $(MAPDIR)/%.cpp,$(BUILDDIR)/%.o,$(MAP))
OBJCETDIR		= src/object
OBJECT			= $(wildcard $(OBJCETDIR)/*.cpp)
OBJECTOBJS		= $(patsubst $(OBJCETDIR)/%.cpp,$(BUILDDIR)/%.o,$(OBJECT))
SIMULATORDIR	= src/simulator
SIMULATOR		= $(wildcard $(SIMULATORDIR)/*.cpp)
SIMULATOROBJS	= $(patsubst $(SIMULATORDIR)/%.cpp,$(BUILDDIR)/%.o,$(SIMULATOR))

OBJS			= $(MAPOBJS) $(OBJECTOBJS) $(SIMULATOROBJS)

EXEC			= simulation
VPATH			= src/simulator src/map src/object


all: make_build_dir $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $^ -o $@	$(CXXFLAGS) $(LIBS)

$(BUILDDIR)/%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCLUDES)


.PHONY: run clean make_build_dir

run: make_build_dir $(EXEC)
	./$(EXEC)

make_build_dir:
	@mkdir -p $(BUILDDIR)

clean:
	@rm -rf $(BUILDDIR) $(EXEC)