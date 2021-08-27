CC = g++
SRC_DIR = ./src
INC_DIR = ./headers/
SFML_LIB = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
MAIN = theageoftitans
OBJ = main.o EventManager.o MapManager.o
.PHONY: cleanobj

$(MAIN): $(OBJ)
	$(CC) -o $(MAIN) $(OBJ) $(SFML_LIB)

main.o: $(SRC_DIR)/main.cpp
	$(CC)  -c $(SRC_DIR)/main.cpp -I $(INC_DIR)

EventManager.o: $(SRC_DIR)/EventManager.cpp
	$(CC)  -c $(SRC_DIR)/EventManager.cpp -I $(INC_DIR)

MapManager.o: $(SRC_DIR)/MapManager.cpp
	$(CC)  -c $(SRC_DIR)/MapManager.cpp -I $(INC_DIR)

cleanobj:
	rm *.o
