// Este arquivo é o CÉREBRO do jogo. Ele decide como criar as cartas, como os inimigos atacam, e como o combate avança.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "deck.h"

static int sorteia_numero(int menor, int maior) { 
    // Sorteia um numero
    if (menor > maior) return menor;
    return menor + rand() % (maior - menor + 1); 
}

static Carta criar_carta_aleatoria(int tipo_acao, int custo_fixo) {
    // Cria uma carta com custo e poder aleatorios mas validos
    Carta nova_carta;
    nova_carta.tipo_de_acao = tipo_acao;
    nova_carta.custo_energia = custo_fixo;
    
    if (tipo_acao == CARTA_ATAQUE) strcpy(nova_carta.nome_carta, "ATK");
    else if (tipo_acao == CARTA_DEFESA) strcpy(nova_carta.nome_carta, "DEF");
    else if (tipo_acao == CARTA_ESPECIAL) strcpy(nova_carta.nome_carta, "ESP");

    int efeito_minimo = 0, efeito_maximo = 0;
    
    if (tipo_acao == CARTA_ESPECIAL) {
        nova_carta.custo_energia = 0;
        nova_carta.poder_efeito = 0; 
    } else {
        if (custo_fixo == 0)      { efeito_minimo = 1;  efeito_maximo = 5; }
        else if (custo_fixo == 1) { efeito_minimo = 5;  efeito_maximo = 10; }
        else if (custo_fixo == 2) { efeito_minimo = 10; efeito_maximo = 15; }
        else if (custo_fixo == 3) { efeito_minimo = 15; efeito_maximo = 30; }
        nova_carta.poder_efeito = sorteia_numero(efeito_minimo, efeito_maximo);
    }
    return nova_carta;
}

void aplicar_dano_a_pessoa(SerVivo *alvo, int dano) {
    // Aplica dano, tirando primeiro do escudo e depois da vida
    if (dano <= 0) return;
    
    if (alvo->escudo > 0) {
        if (dano >= alvo->escudo) {
            dano -= alvo->escudo;
            alvo->escudo = 0;
        } else {
            alvo->escudo -= dano;
            dano = 0;
        }
    }
    
    alvo->vida_atual -= dano;
    if (alvo->vida_atual < 0) alvo->vida_atual = 0;
}

void criar_baralho_inicial(EstadoDoJogo *jogo) {
    // Monta o baralho de 20 cartas do jogador
    jogo->baralho_completo.quantas_tem = 0;
    
    adicionar_carta(&jogo->baralho_completo, criar_carta_aleatoria(CARTA_ATAQUE, 0));
    for (int i = 0; i < 3; i++) adicionar_carta(&jogo->baralho_completo, criar_carta_aleatoria(CARTA_ATAQUE, 1));
    adicionar_carta(&jogo->baralho_completo, criar_carta_aleatoria(CARTA_ATAQUE, 2));
    adicionar_carta(&jogo->baralho_completo, criar_carta_aleatoria(CARTA_ATAQUE, 3));
    for (int i = 0; i < 4; i++) adicionar_carta(&jogo->baralho_completo, criar_carta_aleatoria(CARTA_ATAQUE, sorteia_numero(0, 3)));
    
    adicionar_carta(&jogo->baralho_completo, criar_carta_aleatoria(CARTA_DEFESA, 0));
    for (int i = 0; i < 3; i++) adicionar_carta(&jogo->baralho_completo, criar_carta_aleatoria(CARTA_DEFESA, 1));
    adicionar_carta(&jogo->baralho_completo, criar_carta_aleatoria(CARTA_DEFESA, 2));
    adicionar_carta(&jogo->baralho_completo, criar_carta_aleatoria(CARTA_DEFESA, 3));
    for (int i = 0; i < 2; i++) adicionar_carta(&jogo->baralho_completo, criar_carta_aleatoria(CARTA_DEFESA, sorteia_numero(0, 2)));
    
    // Carta Especial original (Descarta e Compra 5)
    adicionar_carta(&jogo->baralho_completo, criar_carta_aleatoria(CARTA_ESPECIAL, 0));
    
    // NOVA CARTA: POÇÃO DE CURA (Custo 2, Efeito +10 HP, +5 Escudo)
    Carta cura_nova = criar_carta_aleatoria(CARTA_ESPECIAL, 0); 
    strcpy(cura_nova.nome_carta, "POÇÃO DE CURA");
    cura_nova.custo_energia = 2; 
    cura_nova.poder_efeito = 10; 
    adicionar_carta(&jogo->baralho_completo, cura_nova);

    // NOVA CARTA: ENERGIA (Custo 0, Efeito +1 a +2 Energia)
    Carta mana_nova = criar_carta_aleatoria(CARTA_ESPECIAL, 0); 
    strcpy(mana_nova.nome_carta, "ENERGIA");
    mana_nova.custo_energia = 0; 
    mana_nova.poder_efeito = 0;
    adicionar_carta(&jogo->baralho_completo, mana_nova);
    
    // NOVA CARTA: ESCUDO PESADO (Custo 3, Defesa 30, Bloqueio 2 Turnos)
    Carta escudo_pesado = criar_carta_aleatoria(CARTA_DEFESA, 0); 
    strcpy(escudo_pesado.nome_carta, "ESCUDO PESADO");
    escudo_pesado.custo_energia = 3;
    escudo_pesado.poder_efeito = 30;
    adicionar_carta(&jogo->baralho_completo, escudo_pesado);

    misturar_cartas(&jogo->baralho_completo);
}

static SerVivo criar_inimigo(int tipo_de_ser, int nivel_dificuldade) {
    // Monta um inimigo Fraco ou Forte baseado na dificuldade
    SerVivo inimigo = {0};
    inimigo.tipo_de_ser = tipo_de_ser;
    inimigo.escudo = 0;
    
    float multiplicador_vida = 1.0 + (float)nivel_dificuldade * 0.25;
    if (nivel_dificuldade == 0) multiplicador_vida = 1.0;

    if (tipo_de_ser == INIMIGO_FRACO) {
        strcpy(inimigo.nome_personagem, "Fraco");
        inimigo.vida_maxima = (int)((float)sorteia_numero(10, 30) * multiplicador_vida); 
        
        inimigo.numero_de_acoes = sorteia_numero(1, 2);
        
        for (int i = 0; i < inimigo.numero_de_acoes; i++) {
            int tipo_acao = (i == 0) ? CARTA_ATAQUE : sorteia_numero(CARTA_ATAQUE, CARTA_DEFESA);
            int nivel_custo = sorteia_numero(0, 1);
            Carta temporaria = criar_carta_aleatoria(tipo_acao, nivel_custo);
            inimigo.acoes_ciclicas[i].tipo_acao = temporaria.tipo_de_acao;
            inimigo.acoes_ciclicas[i].poder_efeito = temporaria.poder_efeito;
        }

    } else if (tipo_de_ser == INIMIGO_FORTE) {
        strcpy(inimigo.nome_personagem, "Forte");
        inimigo.vida_maxima = (int)((float)sorteia_numero(40, 100) * multiplicador_vida);

        inimigo.numero_de_acoes = sorteia_numero(2, 3);
        
        int contador_nivel_1 = 0;
        for (int i = 0; i < inimigo.numero_de_acoes; i++) {
            int tipo_acao = (i == 0) ? CARTA_ATAQUE : sorteia_numero(CARTA_ATAQUE, CARTA_DEFESA);
            int nivel_custo = sorteia_numero(1, 3);
            
            if (nivel_custo == 1 && contador_nivel_1 >= 1) {
                 nivel_custo = sorteia_numero(2, 3);
            }
            if (nivel_custo == 1) contador_nivel_1++;
            
            Carta temporaria = criar_carta_aleatoria(tipo_acao, nivel_custo);
            inimigo.acoes_ciclicas[i].tipo_acao = temporaria.tipo_de_acao;
            inimigo.acoes_ciclicas[i].poder_efeito = temporaria.poder_efeito;
        }
    }
    
    inimigo.vida_atual = inimigo.vida_maxima;
    inimigo.proxima_acao = 0;
    return inimigo;
}

static SerVivo criar_chefe() {
    // Monta o chefe final (Combate 11)
    SerVivo chefe = {0};
    chefe.tipo_de_ser = INIMIGO_FORTE;
    strcpy(chefe.nome_personagem, "MORTE SUBITA: FINAL BOSS"); 
    
    chefe.vida_maxima = sorteia_numero(300, 400); 
    chefe.vida_atual = chefe.vida_maxima;
    chefe.escudo = 0;
    
    chefe.numero_de_acoes = 3;
    
    chefe.acoes_ciclicas[0].tipo_acao = CARTA_ATAQUE;
    chefe.acoes_ciclicas[0].poder_efeito = sorteia_numero(20, 30); 
    
    chefe.acoes_ciclicas[1].tipo_acao = CARTA_DEFESA;
    chefe.acoes_ciclicas[1].poder_efeito = sorteia_numero(15, 25);
    
    chefe.acoes_ciclicas[2].tipo_acao = CARTA_ATAQUE;
    chefe.acoes_ciclicas[2].poder_efeito = sorteia_numero(10, 20); 

    chefe.proxima_acao = 0;
    return chefe;
}


void reembaralhar_descarte(EstadoDoJogo *jogo) {
    // Move as cartas do descarte para a pilha de compra e mistura
    while (jogo->pilha_descarte.quantas_tem > 0) {
        adicionar_carta(&jogo->pilha_compras, pegar_carta_de_cima(&jogo->pilha_descarte));
    }
    misturar_cartas(&jogo->pilha_compras);
}

void comprar_uma_carta(EstadoDoJogo *jogo) {
    // Tenta puxar 1 carta, reembaralha o descarte se necessário
    if (jogo->pilha_compras.quantas_tem == 0) {
        if (jogo->pilha_descarte.quantas_tem > 0) {
            reembaralhar_descarte(jogo);
        } else {
            return;
        }
    }
    
    if (jogo->pilha_compras.quantas_tem > 0)
        adicionar_carta(&jogo->mao_jogador, pegar_carta_de_cima(&jogo->pilha_compras));
}

void comprar_mao_completa(EstadoDoJogo *jogo) {
    // Compra 5 cartas para a mão do jogador
    jogo->mao_jogador.quantas_tem = 0;
    for (int i = 0; i < 5; i++) comprar_uma_carta(jogo);
}

void iniciar_jogo(EstadoDoJogo *jogo) {
    // Prepara o jogo no estado inicial (Menu)
    srand(time(NULL)); 
    
    jogo->estado_do_jogo = ESTADO_MENU;
    jogo->turno_vez = 0;
    jogo->nivel_dificuldade = 1;

    jogo->vitorias_consecutivas = 0;
    jogo->rodada_atual = 0; 

    jogo->jogador.nome_personagem[0] = 0;
    
    // Define a vida máxima fixa em 100
    jogo->jogador.vida_maxima = 100; 
    jogo->jogador.vida_atual = 100;
    jogo->jogador.escudo = 0;
    jogo->jogador.tipo_de_ser = PERSONAGEM_JOGADOR;
    
    // Inicializa o novo campo
    jogo->turnos_escudo_pesado = 0;

    criar_baralho_inicial(jogo);
}

void comecar_batalha(EstadoDoJogo *jogo) {
    // Prepara o jogo para um novo combate (combates 1 a 10)
    jogo->estado_do_jogo = ESTADO_ARENA;
    jogo->turno_vez = 0;
    jogo->energia_atual = 3;
    jogo->jogador.escudo = 0;
    jogo->alvo_selecionado = 0;
    jogo->total_inimigos = 0;
    jogo->rodada_atual = 1;
    
    // Garante que o estado de bloqueio é resetado
    jogo->turnos_escudo_pesado = 0;

    jogo->pilha_compras = jogo->baralho_completo;
    misturar_cartas(&jogo->pilha_compras);
    
    jogo->pilha_descarte.quantas_tem = 0;
    jogo->mao_jogador.quantas_tem = 0;

    int chance_forte = 5;
    if (jogo->nivel_dificuldade == 0) {
        chance_forte = 0;
    } else if (jogo->nivel_dificuldade == 2) {
        chance_forte = 20;
    }
    int tem_forte = (rand() % 100) < chance_forte;

    for (int i = 0; i < 2; i++) {
        int tipo_inimigo = INIMIGO_FRACO;
        
        if (i == 0 && tem_forte) {
            tipo_inimigo = INIMIGO_FORTE; 
            tem_forte = 0;
        } else if (i == 1 && tem_forte) {
             tipo_inimigo = INIMIGO_FRACO;
        }

        jogo->inimigos[jogo->total_inimigos++] = criar_inimigo(tipo_inimigo, jogo->nivel_dificuldade);
    }
    
    comprar_mao_completa(jogo);
}

void start_boss_battle(EstadoDoJogo *jogo) {
    // Prepara o jogo para a luta contra o Chefe (Combate 11)
    jogo->estado_do_jogo = ESTADO_ARENA;
    jogo->turno_vez = 0;
    jogo->energia_atual = 3;
    jogo->jogador.escudo = 0;
    jogo->alvo_selecionado = 0;
    jogo->total_inimigos = 0;
    jogo->rodada_atual = 1;
    
    // Garante que o estado de bloqueio é resetado
    jogo->turnos_escudo_pesado = 0;

    printf("\n--- BOSS FIGHT: MORTE SUBITA ---\n");
    
    jogo->pilha_compras = jogo->baralho_completo;
    misturar_cartas(&jogo->pilha_compras);
    jogo->pilha_descarte.quantas_tem = 0;
    jogo->mao_jogador.quantas_tem = 0;

    jogo->inimigos[jogo->total_inimigos++] = criar_chefe();
    
    comprar_mao_completa(jogo);
}


void comecar_turno_jogador(EstadoDoJogo *jogo) {
    // Reseta energia e escudo do jogador, compra nova mão e avança a rodada
    jogo->turno_vez = 0;
    jogo->energia_atual = 3;
    jogo->jogador.escudo = 0;
    jogo->rodada_atual++; 
    
    // LOGICA DE ESCUDO PESADO (INÍCIO DO TURNO)
    if (jogo->turnos_escudo_pesado > 0) {
        printf("\n🛡️ SE DEFENDENDO! Você está imobilizado neste turno (ESCUDO PESADO ativo por mais %d turnos).\n", jogo->turnos_escudo_pesado);
        
        // REMOVIDO O DECREMENTO AQUI: O valor só será decrementado no fim do turno (em acao_dos_inimigos)
        
        // Aplica o bloqueio: Não compra cartas e consome toda a energia.
        jogo->energia_atual = 0; 
        
    } else {
        // Se não houver escudo pesado ativo, descarta a mão e compra novas cartas normalmente.
        while (jogo->mao_jogador.quantas_tem > 0) {
            adicionar_carta(&jogo->pilha_descarte, pegar_carta_de_cima(&jogo->mao_jogador));
        }
        comprar_mao_completa(jogo);
    }
}

void finalizar_logica_turno_jogador(EstadoDoJogo *jogo) {
    // Move a mão atual para o descarte e passa a vez para o inimigo
    while (jogo->mao_jogador.quantas_tem > 0) {
        adicionar_carta(&jogo->pilha_descarte, pegar_carta_de_cima(&jogo->mao_jogador));
    }
    jogo->turno_vez = 1;
}

void acao_dos_inimigos(EstadoDoJogo *jogo) {
    // Cada inimigo executa sua ação cíclica
    for (int i = 0; i < jogo->total_inimigos; i++) {
        SerVivo *inimigo = &jogo->inimigos[i];
        if (inimigo->vida_atual > 0) {
            inimigo->escudo = 0;
            
            AcaoDoInimigo acao = inimigo->acoes_ciclicas[inimigo->proxima_acao];
            
            if (acao.tipo_acao == CARTA_ATAQUE) {
                aplicar_dano_a_pessoa(&jogo->jogador, acao.poder_efeito);
            } else if (acao.tipo_acao == CARTA_DEFESA) {
                inimigo->escudo += acao.poder_efeito;
            }

            inimigo->proxima_acao = (inimigo->proxima_acao + 1) % inimigo->numero_de_acoes;
        }
    }
    
    // NOVO LOCAL PARA O DECREMENTO: O contador só é reduzido após o turno restrito ter sido COMPLETADO.
    if (jogo->turnos_escudo_pesado > 0) {
        jogo->turnos_escudo_pesado--;
    }
    
    comecar_turno_jogador(jogo);
}

int usar_carta(EstadoDoJogo *jogo, int indice) {
    // Tenta jogar a carta selecionada, paga o custo e aplica o efeito
    if (indice < 0 || indice >= jogo->mao_jogador.quantas_tem) return 0;
    
    // VERIFICAÇÃO: Não pode jogar cartas se estiver sob o efeito de Escudo Pesado
    // O valor visto aqui é o valor que o turno começou, antes do decremento de acao_dos_inimigos.
    if (jogo->turnos_escudo_pesado > 0) {
        return 0;
    }

    Carta carta_usada = jogo->mao_jogador.lista_cartas[indice];
    
    if (carta_usada.custo_energia > jogo->energia_atual) return 0;
    
    if (carta_usada.tipo_de_acao == CARTA_ATAQUE) {
        if (jogo->alvo_selecionado < jogo->total_inimigos && jogo->inimigos[jogo->alvo_selecionado].vida_atual > 0) {
             aplicar_dano_a_pessoa(&jogo->inimigos[jogo->alvo_selecionado], carta_usada.poder_efeito);
        } else {
            return 0;
        }
    }
    else if (carta_usada.tipo_de_acao == CARTA_DEFESA) {
        
        // EFEITO ESCUDO PESADO
        if (strcmp(carta_usada.nome_carta, "ESCUDO PESADO") == 0) {
            // Aplica grande escudo (poder_efeito = 30)
            jogo->jogador.escudo += carta_usada.poder_efeito; 
            // AJUSTADO: Ativa o estado de bloqueio para 2 turnos (T2 e T3)
            jogo->turnos_escudo_pesado = 2; // COBRE 2 TURNOS COMPLETOS DE RESTRIÇÃO
        } else {
            // Efeito normal de defesa
            jogo->jogador.escudo += carta_usada.poder_efeito;
        }
    }
    else if (carta_usada.tipo_de_acao == CARTA_ESPECIAL) {
        
        // Efeito da Carta POÇÃO DE CURA: Cura 10 HP e +5 Escudo
        if (strcmp(carta_usada.nome_carta, "POÇÃO DE CURA") == 0) {
            // Cura 10 HP (sem ultrapassar o máximo)
            jogo->jogador.vida_atual += 10;
            if (jogo->jogador.vida_atual > jogo->jogador.vida_maxima) {
                jogo->jogador.vida_atual = jogo->jogador.vida_maxima;
            }
            // Adiciona 5 Escudo
            jogo->jogador.escudo += 5;
        }
        // Efeito da Carta ENERGIA: Adiciona 1 ou 2 pontos de energia
        else if (strcmp(carta_usada.nome_carta, "ENERGIA") == 0) {
            int energia_ganha = sorteia_numero(1, 2);
            jogo->energia_atual += energia_ganha;
        }
        // Efeito da Carta Especial original (RECICLAR/ESP): Descarta mão e compra 5
        else {
            if (carta_usada.custo_energia != 0) return 0; 
            
            for (int i = 0; i < jogo->mao_jogador.quantas_tem; i++) {
                if (i != indice) { 
                    adicionar_carta(&jogo->pilha_descarte, jogo->mao_jogador.lista_cartas[i]);
                }
            }
            jogo->mao_jogador.quantas_tem = 0;
            comprar_mao_completa(jogo);
        }
    }

    jogo->energia_atual -= carta_usada.custo_energia;

    adicionar_carta(&jogo->pilha_descarte, carta_usada);

    for (int i = indice; i < jogo->mao_jogador.quantas_tem - 1; i++)
        jogo->mao_jogador.lista_cartas[i] = jogo->mao_jogador.lista_cartas[i+1];
    jogo->mao_jogador.quantas_tem--;
    
    if (jogo->carta_selecionada >= jogo->mao_jogador.quantas_tem && jogo->mao_jogador.quantas_tem > 0) {
        jogo->carta_selecionada = jogo->mao_jogador.quantas_tem - 1;
    }
    
    return 1;
}

void proxima_luta_apos_vitoria(EstadoDoJogo *jogo) {
    // Decide se o jogador inicia o próximo combate, luta contra o Chefão ou vence o jogo
    
    if (jogo->vitorias_consecutivas == 9) {
        jogo->vitorias_consecutivas++;
        start_boss_battle(jogo);
        return;
    }

    jogo->vitorias_consecutivas++;
    
    if (jogo->vitorias_consecutivas >= 11) {
        jogo->estado_do_jogo = ESTADO_VITORIA;
        return;
    }

    comecar_batalha(jogo);
}