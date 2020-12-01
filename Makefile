COMPILER = g++
SRC = ./src/
OUTPUT = game

INCLUDE = -I $(SRC)headers
FLAGS = -Wall -Wextra -g -fsanitize=address

SRC_RULES = RobotsManagement VillageStats Task

all: $(SRC_RULES)
	$(COMPILER) $(SRC)main.cpp *.o $(INCLUDE) $(FLAGS) -o $(OUTPUT)
	make clean

run:
	./$(OUTPUT)

$(SRC_RULES):
	$(COMPILER) $(SRC)$@.cpp -c $(INCLUDE) $(FLAGS)

clean:
	rm -f *.o
