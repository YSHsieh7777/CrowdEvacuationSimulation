
CXX = g++
CXXFLAGS = -std=c++11 -Wall -lSDL2 -lSDL2_image

EXEC = simulation
OBJ = main.o person.o window_map.o
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $^ -o $@	$(CXXFLAGS) 

%.o: %.c
	$(CXX) -c $< -o $@

.PHONY: run
run: $(EXEC)
	./$(EXEC)

.PHONY: clean
clean:
	rm -f *.o $(EXEC)