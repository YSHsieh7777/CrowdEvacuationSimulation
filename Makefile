
CXX			= g++
CXXFLAGS	= -std=c++11 -Wall 
LIBS		= -lSDL2 -lSDL2_image
INCLUDES		= -Iinclude

BUILDDIR	= build
SRCDIR		= src
SRC			= $(wildcard $(SRCDIR)/*.cpp)
OBJ			= $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRC))

EXEC		= simulation
VPATH		= src


all: make_build_dir $(EXEC)

$(EXEC): $(OBJ)
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