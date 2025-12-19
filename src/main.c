#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h> 
#include <stdio.h>
#include "game.h"
#include "renderer.h"

// Este arquivo é o principal, ele inicia o Allegro, carrega as imagens, cuida do loop do jogo e do teclado.
static int verificar_fim_da_luta(EstadoDoJogo *jogo_atual) {
    if (jogo_atual->jogador.vida_atual <= 0) {
        jogo_atual->estado_do_jogo = ESTADO_DERROTA;
        return 1;
    }

    int todos_mortos = 1;
    for (int i = 0; i < jogo_atual->total_inimigos; i++) {
        if (jogo_atual->inimigos[i].vida_atual > 0) {
            todos_mortos = 0;
            break;
        }
    }
    
    if (todos_mortos) {
        proxima_luta_apos_vitoria(jogo_atual);
        return 1;
    }
    return 0;
}

int main() {
    
    // 1. Preparativos
    if (!al_init()) { fprintf(stderr, "ERRO: Nao deu para comecar o Allegro.\n"); return -1; }
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon(); 

    if (!al_install_keyboard()) { fprintf(stderr, "ERRO: Nao deu para usar o teclado.\n"); return -1; }

    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR); 

    ALLEGRO_DISPLAY *tela = al_create_display(1280, 720);
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_display_event_source(tela));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    ALLEGRO_FONT *fonte = al_load_ttf_font("arial.ttf", 24, 0);
    if (!fonte) { fprintf(stderr, "ERRO: Nao achei a fonte arial.ttf.\n"); return -1; }

    // Carregar Imagens
    ALLEGRO_BITMAP *fundo_menu = al_load_bitmap("download (1).jpg");
    if (!fundo_menu) { fprintf(stderr, "AVISO: Fundo do menu falhou. Usando cor.\n"); }
    
    ALLEGRO_BITMAP *fundo_arena = al_load_bitmap("backarena.jpg");
    if (!fundo_arena) { fprintf(stderr, "AVISO: Fundo da arena falhou. Usando cor.\n"); }

    ALLEGRO_BITMAP *imagem_jogador = al_load_bitmap("heroi.jpg");
    if (!imagem_jogador) { fprintf(stderr, "AVISO: Imagem do heroi falhou. Usando quadrado preto.\n"); }
    
    ALLEGRO_BITMAP *imagem_inimigo_fraco = al_load_bitmap("goblin.png");
    if (imagem_inimigo_fraco) { al_convert_mask_to_alpha(imagem_inimigo_fraco, al_map_rgb(255, 255, 255)); } 
    else { fprintf(stderr, "AVISO: Imagem do goblin falhou. Usando quadrado.\n"); }
    
    ALLEGRO_BITMAP *imagem_inimigo_forte = al_load_bitmap("wizard.png");
    if (imagem_inimigo_forte) { al_convert_mask_to_alpha(imagem_inimigo_forte, al_map_rgb(255, 255, 255)); } 
    else { fprintf(stderr, "AVISO: Imagem do mago falhou. Usando quadrado.\n"); }

    ALLEGRO_BITMAP *imagem_chefe = al_load_bitmap("MegaKnight.jpg");
    if (!imagem_chefe) { fprintf(stderr, "AVISO: Imagem do chefe falhou. Usando quadrado.\n"); }


    EstadoDoJogo jogo;
    iniciar_jogo(&jogo);

    int opcao_menu = 0;
    int opcao_dificuldade = 1;

    // 2. Loop do Jogo
    while (jogo.estado_do_jogo != ESTADO_FIM) {
        ALLEGRO_EVENT evento;

        // Desenho da Tela
        if (jogo.estado_do_jogo == ESTADO_MENU) { desenhar_menu(fonte, opcao_menu, fundo_menu); }
        else if (jogo.estado_do_jogo == ESTADO_DIFICULDADE) { desenhar_dificuldade(fonte, opcao_dificuldade, fundo_menu); }
        else if (jogo.estado_do_jogo == ESTADO_ARENA) { 
            // Chamada com todos os 7 argumentos de imagem/fundo
            desenhar_arena(fonte, &jogo, fundo_arena, imagem_jogador, imagem_inimigo_fraco, imagem_inimigo_forte, imagem_chefe); 
            desenhar_mao(fonte, &jogo);
            verificar_fim_da_luta(&jogo); 
        }
        else if (jogo.estado_do_jogo == ESTADO_VITORIA) {
            al_clear_to_color(al_map_rgb(0, 50, 0));
            al_draw_text(fonte, al_map_rgb(0, 255, 0), 640, 300, ALLEGRO_ALIGN_CENTER, "VITORIA! Voce derrotou o CHEFAO!");
        }
        else if (jogo.estado_do_jogo == ESTADO_DERROTA) {
            al_clear_to_color(al_map_rgb(40, 0, 0));
            al_draw_text(fonte, al_map_rgb(255, 0, 0), 640, 300, ALLEGRO_ALIGN_CENTER, "DERROTA! Recomece tudo de novo.");
        }
        
        al_flip_display();

        al_wait_for_event(fila_eventos, &evento);

        // Saída e Encerramento (Teclas)
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
        
        // Tecla Q para sair do jogo (Regra 1.4)
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_Q) {
            jogo.estado_do_jogo = ESTADO_FIM;
            break;
        }
        
        // TRATAMENTO PÓS-FIM DE JOGO: Pressionar ENTER ou ESC após VITORIA/DERROTA encerra o jogo
        if ((jogo.estado_do_jogo == ESTADO_VITORIA || jogo.estado_do_jogo == ESTADO_DERROTA) && evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER || evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                jogo.estado_do_jogo = ESTADO_FIM;
            }
        }


        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {

            if (jogo.estado_do_jogo == ESTADO_MENU) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_UP) opcao_menu--;
                if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN) opcao_menu++;
                if (opcao_menu < 0) opcao_menu = 1;
                if (opcao_menu > 1) opcao_menu = 0;
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    if (opcao_menu == 0) jogo.estado_do_jogo = ESTADO_DIFICULDADE;
                    else if (opcao_menu == 1) jogo.estado_do_jogo = ESTADO_FIM;
                }
            }
            else if (jogo.estado_do_jogo == ESTADO_DIFICULDADE) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_UP) opcao_dificuldade--;
                if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN) opcao_dificuldade++;
                if (opcao_dificuldade < 0) opcao_dificuldade = 2;
                if (opcao_dificuldade > 2) opcao_dificuldade = 0;
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    jogo.nivel_dificuldade = opcao_dificuldade;
                    comecar_batalha(&jogo);
                    jogo.estado_do_jogo = ESTADO_ARENA;
                }
            }

            else if (jogo.estado_do_jogo == ESTADO_ARENA) {
                
                // TRUQUES: FIM DE COMBATE
                if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    for (int i = 0; i < jogo.total_inimigos; i++) { jogo.inimigos[i].vida_atual = 0; }
                    verificar_fim_da_luta(&jogo);
                }
                
                // TRUQUES: VIDA BAIXA
                if (evento.keyboard.keycode == ALLEGRO_KEY_X) {
                    jogo.jogador.vida_atual = 1;
                    verificar_fim_da_luta(&jogo);
                }

                // Navegação
                if (evento.keyboard.keycode == ALLEGRO_KEY_LEFT) jogo.carta_selecionada = (jogo.carta_selecionada - 1 + jogo.mao_jogador.quantas_tem) % jogo.mao_jogador.quantas_tem;
                if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) jogo.carta_selecionada = (jogo.carta_selecionada + 1) % jogo.mao_jogador.quantas_tem;
                
                // Navegação de Alvo
                if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                    if (jogo.total_inimigos > 0) {
                        jogo.alvo_selecionado = (jogo.alvo_selecionado + 1) % jogo.total_inimigos;
                        if (jogo.inimigos[jogo.alvo_selecionado].vida_atual <= 0 && jogo.total_inimigos > 1) {
                             jogo.alvo_selecionado = (jogo.alvo_selecionado + 1) % jogo.total_inimigos;
                        }
                    }
                }

                // Jogar Carta
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    if (usar_carta(&jogo, jogo.carta_selecionada)) { 
                        if (verificar_fim_da_luta(&jogo)) continue; 
                        
                        if (jogo.energia_atual == 0) { 
                            finalizar_logica_turno_jogador(&jogo);
                            acao_dos_inimigos(&jogo);
                            verificar_fim_da_luta(&jogo);
                        }
                    }
                }

                // Finalizar Turno Manual
                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    finalizar_logica_turno_jogador(&jogo);
                    acao_dos_inimigos(&jogo);
                    verificar_fim_da_luta(&jogo);
                }
            }
        }
    }

    // 3. Limpeza
    if (fundo_menu) { al_destroy_bitmap(fundo_menu); }
    if (fundo_arena) { al_destroy_bitmap(fundo_arena); }
    if (imagem_jogador) { al_destroy_bitmap(imagem_jogador); }
    if (imagem_inimigo_fraco) { al_destroy_bitmap(imagem_inimigo_fraco); }
    if (imagem_inimigo_forte) { al_destroy_bitmap(imagem_inimigo_forte); }
    if (imagem_chefe) { al_destroy_bitmap(imagem_chefe); }

    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(tela);
    return 0;
}