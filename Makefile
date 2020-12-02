COMPILER = g++
SRC = ./src/
OUTPUT = game

INCLUDE = -I $(SRC)headers
FLAGS = -Wall -Wextra -g -fsanitize=address -lSDL2_image -lm `sdl2-config --libs --cflags`

SRC_RULES = $(shell find src -name "*.cpp" | sed "s/.cpp/.o/g")

all: $(SRC_RULES)
	$(COMPILER) $(SRC_RULES) $(INCLUDE) $(FLAGS) -o $(OUTPUT)
	make clean

run:
	./$(OUTPUT)

.c.o:
	$(COMPILER) $(SRC)$<.cpp -c -o $@.o $(INCLUDE) $(FLAGS)

clean:
	rm -f $(shell find . -name "*.o")
