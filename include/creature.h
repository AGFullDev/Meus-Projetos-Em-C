// Este arquivo define como é uma Pessoa (Herói ou Inimigo), guardando a vida, escudo e estados ruins.
#ifndef PESSOA_GUARDADA
#define PESSOA_GUARDADA

typedef struct {
    int vida_atual;
    int vida_maxima;
    int escudo;
    int veneno_dano_por_turno;
    int turnos_parado;
} Pessoa;

#endif