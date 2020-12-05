COMPILER = g++
SRC = ./src/
OUTPUT = game

INCLUDE = -I$(SRC)header -I$(SRC)header/game_components
FLAGS = -Wall -Wextra -g -fsanitize=address -lSDL2_image -lm `sdl2-config --libs --cflags`

OBJS = $(shell find src -name "*.cpp" | sed "s/.cpp/.o/g")

all: $(OBJS)
	$(COMPILER) $(OBJS) $(INCLUDE) $(FLAGS) -o $(OUTPUT) && printf "\n\nSuccess!\n\n" || printf "\n\nFailure :(\n\n"
	@make clean &>/dev/null

run:
	./$(OUTPUT)

.cpp.o:
	$(COMPILER) -c $< -o $@ $(INCLUDE) $(FLAGS)

clean:
	echo "Removing all .o files recursively"
	rm -f $(shell find . -name "*.o")
