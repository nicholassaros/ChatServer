CC = g++
FLAGS = -std=c++20

TARGET = server

SRC = ./src/SimpleServer.cpp ./src/ThreadPool.cpp ./src/main.cpp

all: $(TARGET)


$(TARGET) :
	$(CC) $(FLAGS) -o $(TARGET) $(SRC)