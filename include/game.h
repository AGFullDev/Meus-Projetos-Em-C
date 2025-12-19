// Este arquivo guarda o estado de TUDO no jogo: quem é o Herói, quem são os Inimigos, onde estão as cartas e em que tela estamos.
#ifndef JOGO_GUARDADO
#define JOGO_GUARDADO

#include "deck.h"

#define CARTA_ATAQUE 0
#define CARTA_DEFESA 1
#define CARTA_ESPECIAL 2

#define PERSONAGEM_JOGADOR 0
#define INIMIGO_FRACO 1
#define INIMIGO_FORTE 2

#define ESTADO_MENU 0
#define ESTADO_DIFICULDADE 1
#define ESTADO_ARENA 2
#define ESTADO_VITORIA 3
#define ESTADO_DERROTA 4
#define ESTADO_FIM 5

typedef struct {
    int tipo_acao;
    int poder_efeito;
} AcaoDoInimigo;

typedef struct {
    char nome_personagem[32];
    int vida_atual;
    int vida_maxima;
    int escudo;
    int tipo_de_ser;

    AcaoDoInimigo acoes_ciclicas[3];
    int numero_de_acoes;
    int proxima_acao;
} SerVivo;

typedef struct {
    Baralho baralho_completo;
    Baralho pilha_compras;
    Baralho pilha_descarte;
    Baralho mao_jogador;

    SerVivo jogador;
    
    SerVivo inimigos[2]; 
    int total_inimigos;
    int alvo_selecionado;

    int energia_atual;
    int turno_vez;
    int carta_selecionada;

    int vitorias_consecutivas;
    int rodada_atual;
    int estado_do_jogo;
    int nivel_dificuldade;
    
    // NOVO CAMPO para a carta ESCUDO PESADO
    int turnos_escudo_pesado; 

} EstadoDoJogo;

void iniciar_jogo(EstadoDoJogo *jogo);
void comecar_batalha(EstadoDoJogo *jogo);
void comecar_turno_jogador(EstadoDoJogo *jogo);
void finalizar_logica_turno_jogador(EstadoDoJogo *jogo);

void criar_baralho_inicial(EstadoDoJogo *jogo);
void comprar_uma_carta(EstadoDoJogo *jogo);
void comprar_mao_completa(EstadoDoJogo *jogo);
void reembaralhar_descarte(EstadoDoJogo *jogo);

int usar_carta(EstadoDoJogo *jogo, int indice);

void acao_dos_inimigos(EstadoDoJogo *jogo);
void aplicar_dano_a_pessoa(SerVivo *alvo, int dano);

void proxima_luta_apos_vitoria(EstadoDoJogo *jogo);

#endif