#include "state.h"
#include "player.h"
#include "mapa.h"
#ifndef MOBS_H
#define MOBS_H

bool hitbox(STATE s, int n);
void criar(STATE* s,int n);
void mexer(STATE* s,int x,int y,int m);
void draw_inimigos(STATE* st,int n);
void mexe_inimigos(STATE* st,int n);

#endif
