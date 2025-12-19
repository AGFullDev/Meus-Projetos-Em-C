// Este arquivo é o Desenho da Tela, ele mostra o menu, as cartas e os heróis.
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h> 
#include <stdio.h>
#include "renderer.h"
#include "game.h"

#define LARGURA_TELA 1280
#define ALTURA_TELA 720

#define COR_BRILHANTE al_map_rgb(255, 255, 255) 
#define COR_SELECAO al_map_rgb(255, 0, 0)
#define COR_OURO al_map_rgb(255, 255, 0) 
#define COR_PADRAO al_map_rgb(200, 200, 200)

static void desenhar_barra_vida(int x, int y, int largura, int altura, int atual, int maximo, ALLEGRO_FONT *fonte) {
    float porcentagem = (float)atual / maximo;
    if (porcentagem < 0) porcentagem = 0;
    int preenchido = porcentagem * largura;

    al_draw_filled_rectangle(x, y, x + largura, y + altura, al_map_rgb(40, 40, 40));
    al_draw_filled_rectangle(x, y, x + preenchido, y + altura, al_map_rgb(255, 60, 60));
    al_draw_rectangle(x, y, x + largura, y + altura, al_map_rgb(255,255,255), 2);
    
    char texto_vida[16];
    sprintf(texto_vida, "%d/%d", atual, maximo);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), x + largura / 2, y + altura / 2 - al_get_font_ascent(fonte) / 2, ALLEGRO_ALIGN_CENTER, texto_vida);
}

void desenhar_menu(ALLEGRO_FONT *fonte, int escolha, ALLEGRO_BITMAP *fundo_imagem) {
    int centro_x = LARGURA_TELA / 2;

    if (fundo_imagem) {
        al_draw_scaled_bitmap(fundo_imagem, 
                              0, 0, al_get_bitmap_width(fundo_imagem), al_get_bitmap_height(fundo_imagem),
                              0, 0, LARGURA_TELA, ALTURA_TELA, 0);
    } else {
        al_clear_to_color(al_map_rgb(10, 80, 160)); 
    }

    al_draw_text(fonte, COR_BRILHANTE, centro_x, 150, ALLEGRO_ALIGN_CENTER, "SLAY THE TOWER");
    al_draw_text(fonte, COR_PADRAO, centro_x, 200, ALLEGRO_ALIGN_CENTER, "TRABALHO PRATICO");
    
    int botao_largura = 250;
    int botao_altura = 70;
    int botao_x = centro_x - botao_largura / 2;
    int botao_y = 400;

    ALLEGRO_COLOR cor_preenchimento_0 = (escolha == 0) ? COR_SELECAO : al_map_rgb(40, 40, 40); 
    ALLEGRO_COLOR cor_borda_0 = (escolha == 0) ? COR_SELECAO : COR_PADRAO;
    ALLEGRO_COLOR cor_texto_0 = (escolha == 0) ? al_map_rgb(0, 0, 0) : COR_BRILHANTE;
    
    al_draw_filled_rectangle(botao_x, botao_y, botao_x + botao_largura, botao_y + botao_altura, cor_preenchimento_0);
    al_draw_rectangle(botao_x, botao_y, botao_x + botao_largura, botao_y + botao_altura, cor_borda_0, 4);
    
    al_draw_text(fonte, cor_texto_0, centro_x, botao_y + botao_altura / 2 - al_get_font_ascent(fonte)/2, ALLEGRO_ALIGN_CENTER, "INICIAR BATALHA");

    int botao_y_sair = botao_y + botao_altura + 30;
    ALLEGRO_COLOR cor_preenchimento_1 = (escolha == 1) ? COR_SELECAO : al_map_rgb(40, 40, 40);
    ALLEGRO_COLOR cor_borda_1 = (escolha == 1) ? COR_SELECAO : COR_PADRAO;
    ALLEGRO_COLOR cor_texto_1 = (escolha == 1) ? al_map_rgb(0, 0, 0) : COR_BRILHANTE;

    al_draw_filled_rectangle(botao_x, botao_y_sair, botao_x + botao_largura, botao_y_sair + botao_altura, cor_preenchimento_1);
    al_draw_rectangle(botao_x, botao_y_sair, botao_x + botao_largura, botao_y_sair + botao_altura, cor_borda_1, 4);

    al_draw_text(fonte, cor_texto_1, centro_x, botao_y_sair + botao_altura / 2 - al_get_font_ascent(fonte)/2, ALLEGRO_ALIGN_CENTER, "SAIR");
}

void desenhar_dificuldade(ALLEGRO_FONT *fonte, int escolha, ALLEGRO_BITMAP *fundo_imagem) {
    int centro_x = LARGURA_TELA / 2;
    
    if (fundo_imagem) {
        al_draw_scaled_bitmap(fundo_imagem, 
                              0, 0, al_get_bitmap_width(fundo_imagem), al_get_bitmap_height(fundo_imagem),
                              0, 0, LARGURA_TELA, ALTURA_TELA, 0);
    } else {
        al_clear_to_color(al_map_rgb(10, 80, 160)); 
    }
    
    al_draw_text(fonte, COR_BRILHANTE, centro_x, 200, ALLEGRO_ALIGN_CENTER, "ESCOLHA A DIFICULDADE");
    
    const char *nomes[] = {"FACIL", "NORMAL", "DIFICIL"};
    
    int botao_largura = 350;
    int botao_altura = 60;
    int botao_x = centro_x - botao_largura / 2;

    for (int i = 0; i < 3; i++) {
        
        ALLEGRO_COLOR cor_preenchimento = (escolha == i) ? COR_SELECAO : al_map_rgb(40, 40, 40);
        ALLEGRO_COLOR cor_borda = (escolha == i) ? COR_SELECAO : COR_PADRAO;
        ALLEGRO_COLOR cor_texto = (escolha == i) ? al_map_rgb(0, 0, 0) : COR_BRILHANTE;
        
        int caixa_y = 350 + i * 80;
        
        al_draw_filled_rectangle(botao_x, caixa_y, botao_x + botao_largura, caixa_y + botao_altura, cor_preenchimento);
        al_draw_rectangle(botao_x, caixa_y, botao_x + botao_largura, caixa_y + botao_altura, cor_borda, 4);

        al_draw_text(fonte, cor_texto, centro_x, caixa_y + botao_altura / 2 - al_get_font_ascent(fonte)/2, ALLEGRO_ALIGN_CENTER, nomes[i]);
    }
}

void desenhar_arena(ALLEGRO_FONT *fonte, EstadoDoJogo *jogo, ALLEGRO_BITMAP *fundo_arena, ALLEGRO_BITMAP *imagem_jogador, ALLEGRO_BITMAP *imagem_fraco, ALLEGRO_BITMAP *imagem_forte, ALLEGRO_BITMAP *imagem_chefe) {
    char buffer[64];
    
    if (fundo_arena) {
        al_draw_scaled_bitmap(fundo_arena, 
                              0, 0, al_get_bitmap_width(fundo_arena), al_get_bitmap_height(fundo_arena),
                              0, 0, LARGURA_TELA, ALTURA_TELA, 0);
    } else {
        al_clear_to_color(al_map_rgb(20, 20, 40)); 
    }

    sprintf(buffer, "COMBATE: %d/10 | RODADA: %d", jogo->vitorias_consecutivas + 1, jogo->rodada_atual);
    al_draw_text(fonte, COR_BRILHANTE, LARGURA_TELA/2, 10, ALLEGRO_ALIGN_CENTER, buffer);


    int jogador_x = 200;
    int jogador_y = 200;
    float largura_alvo = 100.0f;
    float altura_alvo = 150.0f; 

    if (imagem_jogador) {
        al_draw_scaled_bitmap(imagem_jogador,
                              0, 0, al_get_bitmap_width(imagem_jogador), al_get_bitmap_height(imagem_jogador),
                              jogador_x, jogador_y, largura_alvo, altura_alvo, 0);

    } else {
        al_draw_filled_rectangle(jogador_x, jogador_y, jogador_x + 100, jogador_y + 150, al_map_rgb(0, 0, 0));
    }
    
    al_draw_text(fonte, COR_BRILHANTE, jogador_x + 50, jogador_y - 25, ALLEGRO_ALIGN_CENTER, "HEROI");
    
    desenhar_barra_vida(jogador_x - 25, jogador_y + 160, 150, 20, jogo->jogador.vida_atual, jogo->jogador.vida_maxima, fonte);

    sprintf(buffer, "ESCUDO: %d", jogo->jogador.escudo);
    al_draw_text(fonte, al_map_rgb(0, 255, 255), jogador_x + 50, jogador_y + 190, ALLEGRO_ALIGN_CENTER, buffer);

    sprintf(buffer, "ENERGIA: %d", jogo->energia_atual);
    al_draw_text(fonte, COR_OURO, 50, 50, ALLEGRO_ALIGN_LEFT, buffer);

    // MENSAGEM EXTRA DE ESCUDO PESADO AO LADO DO JOGADOR (Em destaque)
    if (jogo->turnos_escudo_pesado > 0) {
        sprintf(buffer, "SE DEFENDENDO!");
        al_draw_text(fonte, COR_OURO, jogador_x + 50, jogador_y + 220, ALLEGRO_ALIGN_CENTER, buffer);
    }

    int pilha_y = 600;
    
    al_draw_rectangle(50, pilha_y, 100, pilha_y + 50, COR_PADRAO, 2);
    sprintf(buffer, "C: %d", jogo->pilha_compras.quantas_tem);
    al_draw_text(fonte, COR_BRILHANTE, 75, pilha_y + 60, ALLEGRO_ALIGN_CENTER, buffer);
    
    al_draw_rectangle(150, pilha_y, 200, pilha_y + 50, COR_PADRAO, 2);
    sprintf(buffer, "D: %d", jogo->pilha_descarte.quantas_tem);
    al_draw_text(fonte, COR_BRILHANTE, 175, pilha_y + 60, ALLEGRO_ALIGN_CENTER, buffer);

    for (int i = 0; i < jogo->total_inimigos; i++) {
        SerVivo *inimigo = &jogo->inimigos[i];
        if (inimigo->vida_atual <= 0) continue;

        int inimigo_x = 800 + i * 180;
        int inimigo_y = 200;
        float largura_inimigo = 100.0f;
        float altura_inimigo = 150.0f;

        al_draw_text(fonte, COR_BRILHANTE, inimigo_x + 40, inimigo_y - 25, ALLEGRO_ALIGN_CENTER, inimigo->nome_personagem);
        
        ALLEGRO_COLOR cor_reserva = (inimigo->tipo_de_ser == INIMIGO_FORTE) ? al_map_rgb(255, 0, 0) : al_map_rgb(150, 150, 150);

        ALLEGRO_BITMAP *imagem_atual = NULL;
        
        if (jogo->vitorias_consecutivas == 10 && imagem_chefe) {
            imagem_atual = imagem_chefe;
        } else if (inimigo->tipo_de_ser == INIMIGO_FRACO && imagem_fraco) {
            imagem_atual = imagem_fraco;
        } else if (inimigo->tipo_de_ser == INIMIGO_FORTE && imagem_forte) {
            imagem_atual = imagem_forte;
        }
        
        if (imagem_atual) {
            al_draw_scaled_bitmap(imagem_atual,
                              0, 0, al_get_bitmap_width(imagem_atual), al_get_bitmap_height(imagem_atual),
                              inimigo_x, inimigo_y, largura_inimigo, altura_inimigo, 0);
        } else {
            al_draw_filled_rectangle(inimigo_x, inimigo_y, inimigo_x + 80, inimigo_y + 100, cor_reserva);
        }

        if (i == jogo->alvo_selecionado) {
            al_draw_circle(inimigo_x + 40, inimigo_y + 75, 60, COR_SELECAO, 3); 
        }

        desenhar_barra_vida(inimigo_x - 10 + 10, inimigo_y + 155, 100, 15, inimigo->vida_atual, inimigo->vida_maxima, fonte);

        sprintf(buffer, "ESC: %d", inimigo->escudo);
        al_draw_text(fonte, al_map_rgb(0, 255, 255), inimigo_x + 40 + 10, inimigo_y + 180, ALLEGRO_ALIGN_CENTER, buffer);

        AcaoDoInimigo proxima_acao = inimigo->acoes_ciclicas[inimigo->proxima_acao];
        char texto_acao[32];
        ALLEGRO_COLOR cor_acao = al_map_rgb(255, 255, 255);

        if (proxima_acao.tipo_acao == CARTA_ATAQUE) {
            sprintf(texto_acao, "ATK: %d", proxima_acao.poder_efeito);
            cor_acao = al_map_rgb(255, 0, 0); 
        } else if (proxima_acao.tipo_acao == CARTA_DEFESA) {
            sprintf(texto_acao, "DEF: %d", proxima_acao.poder_efeito);
            cor_acao = al_map_rgb(0, 0, 255); 
        }
        al_draw_text(fonte, cor_acao, inimigo_x + 40, inimigo_y - 45, ALLEGRO_ALIGN_CENTER, texto_acao);
    }
}

void desenhar_mao(ALLEGRO_FONT *fonte, EstadoDoJogo *jogo) {
    int base_y = 500; 
    char buffer[64];
    
    if (jogo->turnos_escudo_pesado > 0) {
        // Se ESCUDO PESADO estiver ativo, desenha a mensagem de bloqueio na área central das cartas
        int centro_x = LARGURA_TELA / 2;
        
        // Retângulo de fundo escuro para destacar o aviso
        al_draw_filled_rectangle(centro_x - 400, base_y, centro_x + 400, base_y + 160, al_map_rgb(20, 20, 20));
        
        int centro_y = base_y + 80;
        
        // Título principal em COR_SELECAO (vermelho)
        al_draw_text(fonte, COR_SELECAO, centro_x, centro_y - 40, ALLEGRO_ALIGN_CENTER, "!!! ESCUDO PESADO ATIVO !!!");
        
        // Mensagem de defesa em COR_OURO (amarelo/ouro)
        al_draw_text(fonte, COR_OURO, centro_x, centro_y, ALLEGRO_ALIGN_CENTER, "VOCÊ ESTÁ EM MODO DE DEFESA TOTAL");
        
        // Contagem de turnos restantes em COR_BRILHANTE (branco)
        sprintf(buffer, "NÃO É POSSÍVEL JOGAR CARTAS POR MAIS %d TURNO(S).", jogo->turnos_escudo_pesado);
        al_draw_text(fonte, COR_BRILHANTE, centro_x, centro_y + 40, ALLEGRO_ALIGN_CENTER, buffer);
        
        return; // Sai da função de desenho da mão
    }
    
    for (int i = 0; i < jogo->mao_jogador.quantas_tem; i++) {
        Carta carta = jogo->mao_jogador.lista_cartas[i];

        int largura_carta = 150;
        int altura_carta = 160;
        int x = 250 + i * 160; 
        int y = base_y;
        
        int deslocamento_y = (i == jogo->carta_selecionada) ? -20 : 0; 
        
        ALLEGRO_COLOR cor_borda = (i == jogo->carta_selecionada) ? COR_SELECAO : COR_BRILHANTE;
        ALLEGRO_COLOR cor_fundo_carta = (carta.tipo_de_acao == CARTA_ATAQUE) ? al_map_rgb(150, 40, 40) : 
                                   (carta.tipo_de_acao == CARTA_DEFESA) ? al_map_rgb(40, 40, 150) : 
                                   al_map_rgb(40, 150, 40);

        al_draw_filled_rectangle(x, y + deslocamento_y, x + largura_carta, y + altura_carta + deslocamento_y, cor_fundo_carta);
        al_draw_rectangle(x, y + deslocamento_y, x + largura_carta, y + altura_carta + deslocamento_y, cor_borda, 3);
        
        int texto_x_esquerda = x + 10; 
        
        int texto_x_valor_inicio = x + 110; 

        al_draw_text(fonte, COR_BRILHANTE, x + largura_carta/2, y + 10 + deslocamento_y, ALLEGRO_ALIGN_CENTER, carta.nome_carta);

        al_draw_text(fonte, COR_OURO, texto_x_esquerda, y + 40 + deslocamento_y, ALLEGRO_ALIGN_LEFT, "CUSTO:");
        sprintf(buffer, "%d", carta.custo_energia);
        al_draw_text(fonte, COR_BRILHANTE, texto_x_valor_inicio, y + 40 + deslocamento_y, ALLEGRO_ALIGN_LEFT, buffer);

        al_draw_text(fonte, COR_BRILHANTE, texto_x_esquerda, y + 70 + deslocamento_y, ALLEGRO_ALIGN_LEFT, "EFEITO:");
        sprintf(buffer, "%d", carta.poder_efeito);
        al_draw_text(fonte, COR_BRILHANTE, texto_x_valor_inicio, y + 70 + deslocamento_y, ALLEGRO_ALIGN_LEFT, buffer);
    }
}