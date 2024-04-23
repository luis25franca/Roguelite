#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

#include "state.h"


#define MAX_DIST 21

bool hitbox(STATE s, int n){
    for(int i=0;i<n;i++){
        if (s.p1.playerX == s.mob[i].mobX && s.p1.playerY == s.mob[i].mobY &&s.mob[i].kind!='D') {return true;}
    }
    return false; 
}

void criar (STATE *s,int n) {
    srand(time(NULL));
    for(int i=0;i<n;i++){ //numero de inimigos
        int x,y;
        do{
            x=(rand() % 36) + 1;
            y=(rand() % 148) + 1;
        } while(s->mapa[x][y].terreno == '#');

        int k = (rand() % 4); //««

        if(k==0) {
        s->mob[i].mobX = x;
        s->mob[i].mobY = y;
        s->mob[i].kind = 'G'; //goblin
        s->mob[i].hp = 1;
        s->mapa[x][y].tem_mob = i;
        s->mapa[x][y].foi_iluminado = 1; 
        }
        if(k==1){
        s->mob[i].mobX = x;
        s->mob[i].mobY = y;
        s->mob[i].kind = 'O'; //ork
        s->mob[i].hp = 2;
        s->mapa[x][y].tem_mob = i;
        s->mapa[x][y].foi_iluminado = 1; 
        }
        if(k==2){
        s->mob[i].mobX = x;
        s->mob[i].mobY = y;
        s->mob[i].kind = 'W'; //wolf
        s->mob[i].hp = 1;
        s->mapa[x][y].tem_mob = i; 
        s->mapa[x][y].foi_iluminado = 1;
        }
        if(k==3){
        s->mob[i].mobX = x;
        s->mob[i].mobY = y;
        s->mob[i].kind = 'T'; //TROLL
        s->mob[i].hp = 3;
        s->mapa[x][y].tem_mob = i; 
        s->mapa[x][y].foi_iluminado = 1;
        }

    }
}

void mexer (STATE *s ,int x, int y,int m){
    int dx=x,dy=y;
    int k = 1 + (rand()%2);
    
    if (s->mob[m].kind=='G' || s->mob[m].kind=='W' || s->mob[m].kind=='T'|| (s->mob[m].kind == 'O' && k%2==0)){
        for (int i = x-1; i < x+1; i++) {
            for (int j = y-1; j < y+1; j++) {
                if (s->mapa[i][j].dist < s->mapa[x][y].dist && s->mob[m].kind !='T') {
                    dx = i; dy = j;
                } 
            }
        } 
        if (dx==x && dy==y){
            do{
                dx=x-1 + (rand() % 3);
                dy=y-1 + (rand() % 3);
            } while(s->mapa[dx][dy].terreno == '#');
        }
        
    }
    s->mob[m].mobX = dx; 
    s->mob[m].mobY = dy;
    s->mapa[x][y].tem_mob = -1; 
    s->mapa[dx][dy].tem_mob = m; 
}
   

void mexe_inimigos(STATE* st,int n){
    int m=0;
    distancia(st->p1.playerX,st->p1.playerY,0,st);
    while(m<n){
        if (st->mob[m].hp <= 0) {
            st->mob[m].kind='D';
        }
        mexer(st,st->mob[m].mobX,st->mob[m].mobY,m); 
        m++;   
    }
    for(int x1=0;x1<149;x1++){
        for(int x2=0;x2<37;x2++){
            st->mapa[x2][x1].dist=120;
        }
    } 
}

void draw_inimigos(STATE* st,int n) {
    mexe_inimigos(st,n);
    for (int i = 0; i < n; i++) {
        int mobX = st->mob[i].mobX;
        int mobY = st->mob[i].mobY;
        if(st->mob[i].hp>0){
           if (mobX >= 0 && mobX < MAP_WIDTH && mobY >= 0 && mobY < MAP_HEIGHT) {
            if (st->mapa[mobX][mobY].foi_iluminado && st->mapa[mobX][mobY].tem_mob == i) {
                //mob visível no fov
                if(st->mapa[mobX][mobY].terreno == '+'){
                    if (st->mob[i].kind == 'G') {
                    attron(COLOR_PAIR(42));
                    mvaddch(mobX, mobY, 'G');
                    attroff(COLOR_PAIR(42));
                } else if (st->mob[i].kind == 'O') {
                    attron(COLOR_PAIR(41));
                    mvaddch(mobX, mobY, 'O');
                    attroff(COLOR_PAIR(41));
                } else if (st->mob[i].kind == 'W') {
                    attron(COLOR_PAIR(45));
                    mvaddch(mobX, mobY, 'W');
                    attroff(COLOR_PAIR(45));
                } else if (st->mob[i].kind == 'T') {
                    attron(COLOR_PAIR(43));
                    mvaddch(mobX, mobY, 'T');
                    attroff(COLOR_PAIR(43));
                }
                }
                if(st->mapa[mobX][mobY].terreno == '.'){
                    if (st->mob[i].kind == 'G') {
                    attron(COLOR_PAIR(22));
                    mvaddch(mobX, mobY, 'G');
                    attroff(COLOR_PAIR(22));
                } else if (st->mob[i].kind == 'O') {
                    attron(COLOR_PAIR(21));
                    mvaddch(mobX, mobY, 'O');
                    attroff(COLOR_PAIR(21));
                } else if (st->mob[i].kind == 'W') {
                    attron(COLOR_PAIR(25));
                    mvaddch(mobX, mobY, 'W');
                    attroff(COLOR_PAIR(25));
                } else if (st->mob[i].kind == 'T') {
                    attron(COLOR_PAIR(23));
                    mvaddch(mobX, mobY, 'T');
                    attroff(COLOR_PAIR(23));
                }
                }
                if(st->mapa[mobX][mobY].terreno == '%'){
                    if (st->mob[i].kind == 'G') {
                    attron(COLOR_PAIR(32));
                    mvaddch(mobX, mobY, 'G');
                    attroff(COLOR_PAIR(32));
                } else if (st->mob[i].kind == 'O') {
                    attron(COLOR_PAIR(31));
                    mvaddch(mobX, mobY, 'O');
                    attroff(COLOR_PAIR(31));
                } else if (st->mob[i].kind == 'W') {
                    attron(COLOR_PAIR(35));
                    mvaddch(mobX, mobY, 'W');
                    attroff(COLOR_PAIR(35));
                } else if (st->mob[i].kind == 'T') {
                    attron(COLOR_PAIR(34));
                    mvaddch(mobX, mobY, 'T');
                    attroff(COLOR_PAIR(34));
                }
                }
            }
        } 
        }
        
    }
}

void increaseMobs(STATE* st, int numMobsToAdd) {
    // Verifica se já passou tempo suficiente para adicionar mais mobs
    if (st->turns % 2 == 0) {
        // Adiciona os mobs
        for (int i = st->mobnum+1; i < (st->mobnum + numMobsToAdd); i++) {
                // Reinicia o mob
                int x, y;
                do {
                    x = (rand() % 36) + 1;
                    y = (rand() % 148) + 1;
                } while (st->mapa[x][y].terreno == '#');

                int k = (rand() % 4);

                if (k == 0) {
                    st->mob[i].mobX = x;
                    st->mob[i].mobY = y;
                    st->mob[i].kind = 'G';  // goblin
                    st->mob[i].hp = 1;
                    st->mapa[x][y].tem_mob = i;
                    st->mapa[x][y].foi_iluminado = 1;
                } else if (k == 1) {
                    st->mob[i].mobX = x;
                    st->mob[i].mobY = y;
                    st->mob[i].kind = 'O';  // ork
                    st->mob[i].hp = 2;
                    st->mapa[x][y].tem_mob = i;
                    st->mapa[x][y].foi_iluminado = 1;
                } else if (k == 2) {
                    st->mob[i].mobX = x;
                    st->mob[i].mobY = y;
                    st->mob[i].kind = 'W';  // wolf
                    st->mob[i].hp = 1;
                    st->mapa[x][y].tem_mob = i;
                    st->mapa[x][y].foi_iluminado = 1;
                } else if (k == 3) {
                    st->mob[i].mobX = x;
                    st->mob[i].mobY = y;
                    st->mob[i].kind = 'T';  // troll
                    st->mob[i].hp = 3;
                    st->mapa[x][y].tem_mob = i;
                    st->mapa[x][y].foi_iluminado = 1;
                }
        }
    }
}
