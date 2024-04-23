#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>

#include "state.h"

int calcularPontuacao(STATE *st) {
    // Pontuação para cada mob derrotado
    int points = 0;
    for (int i = 0; i < st->mobnum; i++) {
        if (st->mob[i].hp <= 0)
            points += 50;
    }

    st->p1.p_level = points/500; //subir de nivel de 500 em 500 pontos
    st->p1.hp_max = 20 + st->p1.p_level *5; 
    st->p1.mana_max = 50 + st->p1.p_level *5;
    return points;}

void draw_hud(STATE* st){

    if (st->key_presses != 0 && st->key_presses % 60 == 0) st->turns++; //a cada 60 movimentos sobe um turno
    if (st->key_presses != 0 && st->key_presses % 120 == 0){ //a cada 120 movimentos aumenta quantidade de mobs e começa uma nova wave
        if(st->mobnum <= 30 && st->level == 1){ 
            increaseMobs(st,10);
            st->wave++;
            st->mobnum +=10;
        }
        else if (st->mobnum > 30 && st->mobnum < 40 && st->level == 1) {
            increaseMobs(st,40-st->mobnum);
            st->wave++;
            st->mobnum += 40-st->mobnum;
        }
        else st->wave++; 
    }

    mvprintw(2,MAP_HEIGHT+3,"Turno %d", st->turns);

    mvprintw(3,MAP_HEIGHT+3,"WAVE %d", st->wave);

    mvprintw(20,MAP_HEIGHT+3,"MOBS %d", st->mobnum);

    mvprintw(4,MAP_HEIGHT+3,"Pontos: %d", calcularPontuacao(st));
    mvprintw(5,MAP_HEIGHT+3,"Nivel: %d", (st->p1.p_level+1));

    attron(COLOR_PAIR(COLOR_RED));
    mvprintw(6,MAP_HEIGHT+3,"Health Points: %d", st->p1.hp);
    attroff(COLOR_PAIR(COLOR_RED));

    attron(COLOR_PAIR(COLOR_BLUE));
    mvprintw(7,MAP_HEIGHT+3,"Magic Points: %d", st->p1.mana);
    attroff(COLOR_PAIR(COLOR_BLUE));

    if(st->p1.atk=='j'){
        attron(COLOR_PAIR(COLOR_CYAN)); //desenhar icone da espada selecionada
        mvprintw(10, MAP_HEIGHT+2, "%c", '+');
        mvprintw(10, MAP_HEIGHT+4, "%c", '+');
        for (int i = 0; i < 7; i++) {
        if(i<2){
            mvprintw(9 + i, MAP_HEIGHT+3, "%c", '+');
        }
        else{
            mvprintw(9 + i, MAP_HEIGHT+3, "%c", '|');
        }
        
    }
        attroff (COLOR_PAIR(COLOR_CYAN));
    }
    else{ //desenhar icone da espada
        mvprintw(10, MAP_HEIGHT+2, "%c", '+');
        mvprintw(10, MAP_HEIGHT+4, "%c", '+');
        for (int i = 0; i < 7; i++) {
        if(i<2){
            mvprintw(10 + i, MAP_HEIGHT+3, "%c", '+');
        }
        else{
            mvprintw(10 + i, MAP_HEIGHT+3, "%c", '|');
        }
        
    }
    }
     attron (COLOR_PAIR(COLOR_BLUE));
     mvprintw(16, MAP_HEIGHT+2, "%s", "MP: 0");
     attroff (COLOR_PAIR(COLOR_BLUE));
     mvprintw(17, MAP_HEIGHT+2, "%s", "Key: J");


    char bow_sprite[][10] = { //icone do relampago
        "  | \\\\",
        "  |  \\\\",
        "  |   \\\\",
        ">-|---->\\",
        "  |    //",
        "  |   //",
        "  |  //",
        "  | //"
    };


    if(st->p1.atk=='l'){ //desenhar relapago selecionado
        attron(COLOR_PAIR(COLOR_MAGENTA));
        for (unsigned int i = 0; i < sizeof(bow_sprite) / sizeof(bow_sprite[0]); i++) {
        mvprintw(19 + i, MAP_HEIGHT+ 2, "%s", bow_sprite[i]);}
        attroff (COLOR_PAIR(COLOR_MAGENTA));
    }
    else{ //desenhar relampago
        for (unsigned int i = 0; i < sizeof(bow_sprite) / sizeof(bow_sprite[0]); i++) {
        mvprintw(19 + i, MAP_HEIGHT+ 2, "%s", bow_sprite[i]);
        }
    }
    
    attron (COLOR_PAIR(COLOR_BLUE));
     mvprintw(27, MAP_HEIGHT+2, "%s", "MP: 5");
     attroff (COLOR_PAIR(COLOR_BLUE));
     mvprintw(28, MAP_HEIGHT+2, "%s", "Key: L");


    char *bomb_sprite[] = { //icone da explosão
        "   **  ",
        "   ||   ",
        " __||__",
        "|      |",
        "|      |",
        "|______|"
    };

    if(st->p1.atk=='k'){ //desenhar icone da bomba selecionado
        attron(COLOR_PAIR(COLOR_RED));
        for (unsigned int i = 0; i < sizeof(bomb_sprite) / sizeof(bomb_sprite[0]); i++) {
        mvprintw(9 + i, MAP_HEIGHT + 11, "%s", bomb_sprite[i]);}
        attroff (COLOR_PAIR(COLOR_RED));
    }
    else{ //desenhar icone da bomba
        for (unsigned int i = 0; i < sizeof(bomb_sprite) / sizeof(bomb_sprite[0]); i++) {
        mvprintw(9 + i, MAP_HEIGHT + 11, "%s", bomb_sprite[i]);
        }
    }
    
    attron (COLOR_PAIR(COLOR_BLUE));
    mvprintw(16, MAP_HEIGHT+12, "%s", "MP: 10");
    attroff (COLOR_PAIR(COLOR_BLUE));
    mvprintw(17, MAP_HEIGHT+12, "%s", "Key: K");


     char *cross_sprite[] = { //icone da cura
        "     I  ",
        "     I   ",
        "     I   ",
        " ----+----",
        "     I   ",
        "     I   ",
        "     I   "

    };
    if(st->p1.atk=='o'){ //desenhar icone da cura selecionado
        attron(COLOR_PAIR(COLOR_GREEN));
        for (unsigned int i = 0; i < sizeof(cross_sprite) / sizeof(cross_sprite[0]); i++) {
        mvprintw(30 + i, MAP_HEIGHT + 1, "%s", cross_sprite[i]);}
        attroff (COLOR_PAIR(COLOR_GREEN));
    }
    else{ //desenhar icone da cura
        for (unsigned int i = 0; i < sizeof(cross_sprite) / sizeof(cross_sprite[0]); i++) {
        mvprintw(30 + i, MAP_HEIGHT + 1, "%s", cross_sprite[i]);
        }
    }

    attron (COLOR_PAIR(COLOR_BLUE));
    mvprintw(37, MAP_HEIGHT+2, "%s", "MP: 5");
    attroff (COLOR_PAIR(COLOR_BLUE));
    mvprintw(38, MAP_HEIGHT+2, "%s", "Key: O");


    /*mvprintw(24,MAP_HEIGHT+3,"%s","Wow é uma descrição qualquer que estou a meter ");
    mvprintw(25,MAP_HEIGHT+3,"%s", "porque n percebi oq e q era suposto meter aqui");*/


    char *hammer_sprite[] = { //icone do terramoto
        "  ####### ",
        "  ####### ",
        "  ####### ",
        "     I   ",
        "     I   ",
        "     I   ",
        "     I   "

    };
    if(st->p1.atk=='i'){ //desenhar icone do terramoto selecionado
        attron(COLOR_PAIR(COLOR_YELLOW));
        for (unsigned int i = 0; i < sizeof(hammer_sprite) / sizeof(hammer_sprite[0]); i++) {
        mvprintw(19 + i, MAP_HEIGHT + 11, "%s", hammer_sprite[i]);}
        attroff (COLOR_PAIR(COLOR_YELLOW));
    }
    else{ //desenhar icone do terramoto
        for (unsigned int i = 0; i < sizeof(hammer_sprite) / sizeof(hammer_sprite[0]); i++) {
        mvprintw(19 + i, MAP_HEIGHT + 11, "%s", hammer_sprite[i]);
        }
    }

    attron (COLOR_PAIR(COLOR_BLUE));
    mvprintw(27, MAP_HEIGHT+12, "%s", "MP: 15");
    attroff (COLOR_PAIR(COLOR_BLUE));
    mvprintw(28, MAP_HEIGHT+12, "%s", "Key: I");


}
