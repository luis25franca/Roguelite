#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#include "state.h"
#include "color.c"
#include "visao.h"
#include "player.h"
#include "mobs.h"
#include "mobs.c"
#include "mapa.h"
#include "visao.c"
#include "player.c"
#include "mapa.c"
#include "save_load.h"
#include "save_load.c"
#include "hud.c"
#include "hud.h"

int main() {
	STATE st;
    initscr(); //inicializar o ncurses
	init_all_colors(); //inicializar as cores (color.c)
    cbreak(); //para que os inputs não esperem por um Enter 
    noecho(); //para que os inputs não sejam duplicados no display
    nonl(); //Enter não é lido como newline
    intrflush(stdscr, false); //descarta qualquer input se o programa for interrompido
    keypad(stdscr, true); //ativa o reconhecimento de teclas especiais (setas, fn)
	mousemask(ALL_MOUSE_EVENTS, NULL); //ativa o rato no terminal
	
    int choice, start;
    while (1) {
        clear();
        attron(COLOR_PAIR(50));
        mvprintw((38/2)-5, (150/2)-65, "'########::'##::::'##:'##::: ##::'######:::'########::'#######::'##::: ##:::::'######::'##::::::::::'###::::'##:::'##:'########:'########::");
        mvprintw((38/2)-4, (150/2)-65, " ##.... ##: ##:::: ##: ###:: ##:'##... ##:: ##.....::'##.... ##: ###:: ##::::'##... ##: ##:::::::::'## ##:::. ##:'##:: ##.....:: ##.... ##:");
        mvprintw((38/2)-3, (150/2)-65, " ##:::: ##: ##:::: ##: ####: ##: ##:::..::: ##::::::: ##:::: ##: ####: ##:::: ##:::..:: ##::::::::'##:. ##:::. ####::: ##::::::: ##:::: ##:");
        mvprintw((38/2)-2, (150/2)-65, " ##:::: ##: ##:::: ##: ## ## ##: ##::'####: ######::: ##:::: ##: ## ## ##::::. ######:: ##:::::::'##:::. ##:::. ##:::: ######::: ########::");
        mvprintw((38/2)-1, (150/2)-65, " ##:::: ##: ##:::: ##: ##. ####: ##::: ##:: ##...:::: ##:::: ##: ##. ####:::::..... ##: ##::::::: #########:::: ##:::: ##...:::: ##.. ##:::");
        mvprintw((38/2),   (150/2)-65, " ##:::: ##: ##:::: ##: ##:. ###: ##::: ##:: ##::::::: ##:::: ##: ##:. ###::::'##::: ##: ##::::::: ##.... ##:::: ##:::: ##::::::: ##::. ##::");
        mvprintw((38/2)+1, (150/2)-65, " ########::. #######:: ##::. ##:. ######::: ########:. #######:: ##::. ##::::. ######:: ########: ##:::: ##:::: ##:::: ########: ##:::. ##:");
        mvprintw((38/2)+2, (150/2)-65, "........::::.......:::..::::..:::......::::........:::.......:::..::::..::::::......:::........::..:::::..:::::..:::::........::..:::::..::");
        mvprintw((38/2)+4, (150/2)-25, "Edgar Pinto, João Brito, Luís França, Pedro Matos, 2022/2023");
        attroff(COLOR_PAIR(50));
        mvprintw((38/2)+7, (150/2), "------Menu------");
        attron(COLOR_PAIR(COLOR_GREEN));
        mvprintw((38/2)+8, (150/2), "1. Iniciar");
        attroff(COLOR_PAIR(COLOR_GREEN));
        attron(COLOR_PAIR(COLOR_YELLOW));
        mvprintw((38/2)+9, (150/2), "2. Continuar");
        attroff(COLOR_PAIR(COLOR_YELLOW));
        attron(COLOR_PAIR(COLOR_RED));
        mvprintw((38/2)+10, (150/2), "3. Sair\n");
        attroff(COLOR_PAIR(COLOR_RED));
        attron(COLOR_PAIR(COLOR_YELLOW));
        mvprintw((38/2)+12, (150/2)-20, "Pressiona a tecla 'X' para guardar o seu progresso");
        attroff(COLOR_PAIR(COLOR_YELLOW));
        refresh();

        choice = getch();
        clear();
        switch (choice) {
            case '1':
                mvprintw((38/2)-5, (150/2)-65, "Bem-vindo ao DungeonSlayer, neste jogo tens o objetivo de conseguir a maior pontuação possível derrotando monstros, enquanto exploras um");
                mvprintw((38/2)-4, (150/2)-65, "complexo sistema de cavernas. Podes alterar o piso onde estás, subindo escadas que vais encontrando ao longo do mapa. De tempo a tempo, começa ");
                mvprintw((38/2)-3, (150/2)-65, "uma nova wave, aparecendo ainda mais inimigos. ");                           
                mvprintw((38/2)-1, (150/2)-65, "Os controlos são simples: usas as setas, WASD, ou o teclado numérico para te movimentares, e o botão esquerdo do rato para atacar. Selecionas o ataque");
                mvprintw((38/2), (150/2)-65, "que pretendes utilizar no teclado, correspondendo cada tecla a um movimento específico.");
                attron(COLOR_PAIR(COLOR_CYAN));
                mvprintw((38/2)+2, (150/2)-65, "'J' : Ataque básico, não gasta mana e ataca em 3 espaços à frente do jogador numa das 4 direções cardinais.");
                attroff(COLOR_PAIR(COLOR_CYAN));
                attron(COLOR_PAIR(COLOR_RED));
                mvprintw((38/2)+4, (150/2)-65, "'K': Bomba, gasta 10 pontos de mana e ataca num quadrado de 3 por 3 baseado na posição do rato, num limite de 5 espaços de distância do jogador. ");
                mvprintw((38/2)+5, (150/2)-65, "ATENÇÃO: se o jogador estiver no alcance da explosão, também toma dano.");
                attroff(COLOR_PAIR(COLOR_RED));
                attron(COLOR_PAIR(COLOR_MAGENTA));
                mvprintw((38/2)+7, (150/2)-65, "'L': Relâmpago, gasta 5 pontos do mana, e ataca num vetor em direção à posição do rato.");
                attroff(COLOR_PAIR(COLOR_MAGENTA));
                attron(COLOR_PAIR(COLOR_YELLOW));
                mvprintw((38/2)+9, (150/2)-65, "'I': Terramoto, gasta 15 pontos de mana, e ataca numa área de 6 por 6 à volta do jogador.");
                attroff(COLOR_PAIR(COLOR_YELLOW));
                attron(COLOR_PAIR(COLOR_GREEN));
                mvprintw((38/2)+11, (150/2)-65, "'O': Cura, gasta 10 pontos de mana e regenera 5 HP ao jogador.");
                attroff(COLOR_PAIR(COLOR_GREEN));
                mvprintw((38/2)+13, (150/2)-65, "A cada 500 pontos sobes de nível e a tua HP e mana máximos aumentam em 5 unidades.");
                mvprintw((38/2)+15, (150/2)-65, "Estás pronto para o desafio? Pressiona qualquer botão para começar.");
                
                start = getch();
                switch(start){
                    default:
                clear();
                
                st.key_presses = 0;
                st.wave = 1;
                st.turns = 1;
                st.p1.hp = 20;
                st.p1.hp_max = 20;
                st.p1.playerX = 20;
                st.p1.playerY = 20;
                st.mapa[20][20].dist = 8;
                st.p1.mana = 50;
                st.p1.mana_max = 50;
                st.p1.atk='j';
                st.mobnum = 10;
                st.p1.p_level = 0;

                generate_map(st.mapa);
                criar(&st,40);

                while (1) {
                    calculate_fov(&st);
                    draw_visible_map(st.mapa);
					//draw_map(st.mapa);
                    draw_hud(&st);
                    draw_inimigos(&st,st.mobnum);
                    if (hitbox(st, st.mobnum)) {st.p1.hp--;}
                    draw_player(&st);
                    if (hitbox(st, st.mobnum)) {st.p1.hp--;}
                    if(st.p1.hp < 0) {st.p1.hp = 0;}
                }

                break;
                }
                break;
            case '2':
                load_game(&st);
                while (1) {
                    calculate_fov(&st);
                    draw_visible_map(st.mapa);
					//draw_map(st.mapa);
                    draw_hud(&st);
                    draw_inimigos(&st,st.mobnum);
                    if (hitbox(st, st.mobnum)) {st.p1.hp--;}
                    draw_player(&st);
                    if (hitbox(st, st.mobnum)) {st.p1.hp--;}
                    if(st.p1.hp < 0) {st.p1.hp = 0;}
                    
                }
                break;

            case '3':
                endwin();
                return 0;

            default:
                mvprintw((38/2), (150/2), "Tecla inválida. Tente novamente");
                getch();
                break;
        }
    }

    endwin();
    return 0;
}
