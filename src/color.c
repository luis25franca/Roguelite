#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

void init_all_colors() {
    start_color();

	init_pair(COLOR_BLACK,COLOR_BLACK,COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_RED,COLOR_RED,COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN,COLOR_BLACK);

    init_color(9, 500, 500, 500); //valores RGB para o cinzento
    init_color(10, 1000, 450, 0); //valores RGB para o laranja
    init_pair(20, 9, 9); //index 20 para o cinzento
    init_pair(50, 10, COLOR_BLACK); //index 50 para o laranja em fundo preto
    
    //para os fundos amarelos
    init_pair(31, COLOR_RED, 3);
    init_pair(32, COLOR_GREEN, 3);
    init_pair(33, COLOR_YELLOW, 3);
    init_pair(34, COLOR_BLUE, 3);
    init_pair(35, COLOR_MAGENTA, 3);

    //para os mobs terem o fundo cinzento do chão
    init_pair(21, COLOR_RED, 9);
    init_pair(22, COLOR_GREEN, 9);
    init_pair(23, COLOR_YELLOW, 9);
    init_pair(24, COLOR_BLUE, 9);
    init_pair(25, COLOR_MAGENTA, 9);

    //para o fundo azul da água
    init_pair (40, COLOR_WHITE, COLOR_BLUE);
    init_pair(41, COLOR_RED, COLOR_BLUE);
    init_pair(42, COLOR_GREEN, COLOR_BLUE);
    init_pair(43, COLOR_YELLOW, COLOR_BLUE);
    init_pair(44, COLOR_BLUE, COLOR_BLUE);
    init_pair(45, COLOR_MAGENTA, COLOR_BLUE);
    
    //para os ataques
    init_pair(9, COLOR_WHITE, COLOR_CYAN);//slash
	init_pair(10,COLOR_WHITE, COLOR_RED);//explosão
	init_pair(11,COLOR_WHITE, COLOR_MAGENTA);//relampago
	init_pair(12,COLOR_WHITE, COLOR_GREEN);//cura
    init_pair(13,COLOR_WHITE, COLOR_YELLOW);//terramoto
}
