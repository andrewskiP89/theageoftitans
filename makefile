CC = g++
SRC_DIR = ./src
INC_DIR = ./headers
SFML_LIB = legendofzedda -lsfml-graphics -lsfml-window -lsfml-system

main: $(SRC_DIR)/main.o
	$(CC) $(SRC_DIR)/main.o -o $(SFML_LIB)

main.o: $(SRC_DIR)/main.cpp
	$(CC)  -c $(SRC_DIR)/main.cpp -I $(INC_DIR)/Menu.h
