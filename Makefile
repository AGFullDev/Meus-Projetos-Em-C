CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude
# CORREÇÃO DEFINITIVA: Ordem das bibliotecas ajustada para linkagem correta.
# Adicionamos -lallegro_image e movemos as dependências para o início.
LIBS=-lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro

SRC=src/main.c src/deck.c src/game.c src/renderer.c
all:
	$(CC) $(CFLAGS) $(SRC) -o game.exe $(LIBS)