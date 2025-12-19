// Este arquivo define como é uma Carta (seu custo e poder) e como organizar grupos de Cartas (o Baralho).
#ifndef BARALHO_GUARDADO
#define BARALHO_GUARDADO

typedef struct {
    char nome_carta[32];
    int custo_energia;
    int poder_efeito;
    int tipo_de_acao;
} Carta;

typedef struct {
    Carta lista_cartas[128];
    int quantas_tem;
} Baralho;

void adicionar_carta(Baralho *lista, Carta carta_nova);
void misturar_cartas(Baralho *lista);
Carta pegar_carta_de_cima(Baralho *lista);

#endif