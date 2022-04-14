#This is my first makefile

CC=gcc

all: #specified target
	$(CC) Physics3d.c -lm -o Physics3d
	$(CC) ParseOutput.c -o OutputDecoder