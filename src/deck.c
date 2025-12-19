// Este arquivo guarda as regras de como manipular os baralhos: adicionar, misturar e puxar cartas.
#include <stdlib.h>
#include "deck.h"

void adicionar_carta(Baralho *lista, Carta carta_nova) {
    // Coloca a carta no final do baralho e aumenta a contagem
    lista->lista_cartas[lista->quantas_tem++] = carta_nova;
}

void misturar_cartas(Baralho *lista) {
    // Troca as cartas de lugar aleatoriamente
    for (int i = 0; i < lista->quantas_tem; i++) {
        int j = rand() % lista->quantas_tem;
        Carta temporaria = lista->lista_cartas[i];
        lista->lista_cartas[i] = lista->lista_cartas[j];
        lista->lista_cartas[j] = temporaria;
    }
}

Carta pegar_carta_de_cima(Baralho *lista) {
    // Pega a última carta do topo e diminui a contagem
    lista->quantas_tem--;
    return lista->lista_cartas[lista->quantas_tem];
}