CC := gcc
SRC := ./src
INC := ./inc

TARGET := run.exe

$(TARGET): $(SRC)/main.c $(SRC)/file_tree.c $(SRC)/tree.c
	$(CC) $^ -I$(INC) -municode -o $@ -g

.PHONY: all

all : $(TARGET)