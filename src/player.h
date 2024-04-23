#include "state.h"
#ifndef PLAYER_H
#define PLAYER_H

int wall(Mapa m);
void do_movement_action(STATE *st, int dx, int dy);
void update(STATE *st);
void draw_player(STATE *st);
void slash (STATE *st, int dx, int dy);
void bomb(STATE*st, int mouse_x, int mouse_y, double angulo_rato, int distancia_rato);
void lightning(STATE *st, double angulo_rato);
void earthquake (STATE *st);
void cura (STATE *st);

#endif
