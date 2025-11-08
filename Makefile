# Simple Makefile for a C project (src + include folders)
CC = gcc
CFLAGS = -Iinclude -Wall -Werror

SRC = $(wildcard src/*.c)
TARGET = app

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

