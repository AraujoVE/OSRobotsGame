COMPILER = g++
SRC = ./src/
OUTPUT = game

INCLUDE = -I $(SRC)header
FLAGS = -Wall -Wextra -g -fsanitize=address -lSDL2_image -lm `sdl2-config --libs --cflags`

OBJS = $(shell find src -name "*.cpp" | sed "s/.cpp/.o/g")

all: $(OBJS)
	$(COMPILER) $(OBJS) $(INCLUDE) $(FLAGS) -o $(OUTPUT)

#make clean

run:
	./$(OUTPUT)

.cpp.o:
	$(COMPILER) -c $< -o $@ $(INCLUDE) $(FLAGS)

clean:
	rm -f $(shell find . -name "*.o")
