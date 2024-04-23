#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#include "state.h"

void calculate_fov(STATE* st) {
    double angle;

    // Reinicializa as propriedades de visibilidade do mapa
    for (int row = 0; row < MAP_WIDTH; row++) {
        for (int column = 0; column < MAP_HEIGHT; column++) {
            st->mapa[row][column].e_iluminado = 0;
            st->mapa[row][column].foi_iluminado = 0;
            st->mapa[row][column].tem_mob = -1; // reseta a visibilidade dos mobs
        }
    }

    // Calcula o campo de visão (Field of View)
    for (angle = 0.0; angle < 2 * M_PI; angle += 0.01) {
        double ray_x = cos(angle);
        double ray_y = sin(angle);
        double posX = st->p1.playerX;
        double posY = st->p1.playerY;

        // Lança um raio de visão a partir da posição do jogador
        while (posX >= 0 && posX < MAP_WIDTH && posY >= 0 && posY < MAP_HEIGHT) {
            int mapX = (int)posX;
            int mapY = (int)posY;

            // Se encontrar uma parede ('#'), interrompe o raio
            if (st->mapa[mapX][mapY].terreno == '#') {
                break;
            }

            // Marca a célula do mapa como iluminada
            st->mapa[mapX][mapY].e_iluminado = 1;
            st->mapa[mapX][mapY].foi_iluminado = 1; // marca como iluminado
            posX += ray_x;
            posY += ray_y;
        }
    }

    // Atualiza a visibilidade dos mobs com base no campo de visão (fov)
    for (int i = 0; i < st->mobnum; i++) {
        int mobX = st->mob[i].mobX;
        int mobY = st->mob[i].mobY;

        if (mobX >= 0 && mobX < MAP_WIDTH && mobY >= 0 && mobY < MAP_HEIGHT) {
            if (st->mapa[mobX][mobY].foi_iluminado && st->mapa[mobX][mobY].terreno != '#') {
                st->mapa[mobX][mobY].tem_mob = i; // mob visível
            } else {
                st->mapa[mobX][mobY].tem_mob = -1; // mob não é visível
            }
        }
    }
}
