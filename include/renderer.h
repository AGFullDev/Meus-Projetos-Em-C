// Este arquivo é o dicionário de todas as funções que desenham na tela.
#ifndef DESENHISTA_GUARDADO
#define DESENHISTA_GUARDADO

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> 
#include "game.h"

void desenhar_menu(ALLEGRO_FONT *fonte, int escolha, ALLEGRO_BITMAP *fundo_imagem); 
void desenhar_dificuldade(ALLEGRO_FONT *fonte, int escolha, ALLEGRO_BITMAP *fundo_imagem); 

void desenhar_arena(ALLEGRO_FONT *fonte, EstadoDoJogo *jogo, ALLEGRO_BITMAP *fundo_arena, ALLEGRO_BITMAP *imagem_jogador, ALLEGRO_BITMAP *imagem_fraco, ALLEGRO_BITMAP *imagem_forte, ALLEGRO_BITMAP *imagem_chefe);

void desenhar_mao(ALLEGRO_FONT *fonte, EstadoDoJogo *jogo);

#endif
