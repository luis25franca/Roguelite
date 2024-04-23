#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

#include "state.h"
#define MAX_DIST 21

void distancia (int R, int C, int value, STATE *s) {
    int DR, DC;
    if(value > MAX_DIST) return;

    if(s->mapa[R][C].terreno == '#') return;

    if(s->mapa[R][C].dist <= value) return;

    s->mapa[R][C].dist = value;

    //FOR_DELTA(DR,DC,1); 
    for (DR = -1; DR <= 1; DR++) {
        for (DC = -1; DC <= 1; DC++) {
            distancia(R + DR, C + DC, value +1,s);
        }
    }
        
}

int count_walls (int row, int column, int span, Mapa map[MAP_WIDTH][MAP_HEIGHT]) {
    int count = 0;
    int span_x, span_y;

    for (span_x = row - span / 2; span_x <= row + span / 2; span_x++) {
        for (span_y = column - span / 2; span_y <= column + span / 2; span_y++) {
            if (span_x >= 0 && span_x < MAP_WIDTH && span_y >= 0 && span_y < MAP_HEIGHT) {
                if (map[span_x][span_y].terreno == '#') {
                    count++;
                }
            }
        }
    }

    return count;
}

void replace_map (Mapa map[MAP_WIDTH][MAP_HEIGHT], char new_map[MAP_WIDTH][MAP_HEIGHT]) {
    int row, column;

    for (row = 0; row < MAP_WIDTH; row++) {
        for (column = 0; column < MAP_HEIGHT; column++) {
            map[row][column].terreno = new_map[row][column];
        }
    }
}

void map_algorithm1 (Mapa map[MAP_WIDTH][MAP_HEIGHT]) {
    char new_map[MAP_WIDTH][MAP_HEIGHT];  // para transpor

    for (int row = 0; row < MAP_WIDTH; row++) {
        for (int column = 0; column < MAP_HEIGHT; column++) {
            int count_walls_3x3 = count_walls(row, column, 3, map);
            int count_walls_5x5 = count_walls(row, column, 5, map);

            if (count_walls_3x3 >= 5 || count_walls_5x5 <= 2) {
                new_map[row][column] = '#';
            } else {
                new_map[row][column] = '.';
            }
        }
    }

    replace_map(map, new_map);
}

void map_algorithm2 (Mapa map[MAP_WIDTH][MAP_HEIGHT]) {
    char new_map[MAP_WIDTH][MAP_HEIGHT]; // para transpor  
    int row, column; 

    for (row = 0; row < MAP_WIDTH; row++) {
        for (column = 0; column < MAP_HEIGHT; column++) {
            if (count_walls(row, column, 5, map) == 0) new_map[row][column] = '.';
        }        
    }

    replace_map(map, new_map);
}

void create_lake(int center_row, int center_column, int radius, char map[MAP_WIDTH][MAP_HEIGHT]) {
    for (int row = center_row - radius; row <= center_row + radius; row++) {
        for (int column = center_column - radius; column <= center_column + radius; column++) {
            if (row >= 0 && row < MAP_WIDTH && column >= 0 && column < MAP_HEIGHT) {
                if (pow(row - center_row, 2) + pow(column - center_column, 2) <= pow(radius, 2)) {
                    map[row][column] = '+';
                }
            }
        }
    }
}

void map_lakes_algorithm (Mapa map[MAP_WIDTH][MAP_HEIGHT]) {
    char new_map[MAP_WIDTH][MAP_HEIGHT]; //para transpor
    int row, column;

    srand(time(NULL));

    //lagos na primeira metade do mapa (raio 7)
    for (row = 0; row < MAP_WIDTH / 2; row++) {
        for (column = 0; column < MAP_HEIGHT; column++) {
            if (count_walls(row, column, 5, map) == 0) {
                new_map[row][column] = '+';
            }
        }
    }

    int random_row = rand() % (MAP_WIDTH / 2);
    int random_column = rand() % MAP_HEIGHT;
    create_lake(random_row, random_column, 7, new_map);

    //lagos na segunda metade do mapa (raio 5)
    for (row = MAP_WIDTH / 2; row < MAP_WIDTH; row++) {
        for (column = 0; column < MAP_HEIGHT; column++) {
            if (count_walls(row, column, 5, map) == 0) {
                new_map[row][column] = '+';
            }
        }
    }

    random_row = rand() % (MAP_WIDTH / 2) + (MAP_WIDTH / 2);
    random_column = rand() % MAP_HEIGHT;
    create_lake(random_row, random_column, 5, new_map);

    replace_map(map, new_map);
}

void create_cave (int center_row, int center_column, int radius, char new_map[MAP_WIDTH][MAP_HEIGHT]) {
    for (int row = center_row - radius; row <= center_row + radius; row++) {
        for (int column = center_column - radius; column <= center_column + radius; column++) {
            if (row > 0 && row < MAP_WIDTH - 1 && column > 0 && column < MAP_HEIGHT - 1) {
                if (pow(row - center_row, 2) + pow(column - center_column, 2) <= pow(radius, 2)) {
                    new_map[row][column] = '%';
                }
            }
        }
    }
}

void add_stairs (Mapa map[MAP_WIDTH][MAP_HEIGHT], int stairs_number) {
    char new_map[MAP_WIDTH][MAP_HEIGHT];

    for (int count = 0; count < stairs_number; count++) {
        int rand_row, rand_column;

        rand_row = rand() % (MAP_WIDTH - 2) + 1;
        rand_column = rand() % (MAP_HEIGHT - 2) + 1;

        if (map[rand_row][rand_column].terreno == '.') {
            new_map[rand_row][rand_column] = '<';
        } else {
            int search_distance = 1;
            int found = 0;

            while (!found && search_distance < MAP_WIDTH && search_distance < MAP_HEIGHT) {
                for (int row = rand_row - search_distance; row <= rand_row + search_distance; row++) {
                    for (int column = rand_column - search_distance; column <= rand_column + search_distance; column++) {
                        if (row >= 0 && row < MAP_WIDTH && column >= 0 && column < MAP_HEIGHT && map[row][column].terreno == '.') {
                            new_map[row][column] = '<';
                            found = 1;
                            break;
                        }
                    }
                    if (found) {
                        break;
                    }
                }
                search_distance++;
            }
        }
    }

    replace_map(map, new_map);
}

void generate_map (Mapa map[MAP_WIDTH][MAP_HEIGHT]) {
    int row, column;
    srand(time(NULL)); //inicializar a seed random do ncurses

    //preenche o mapa com a probabilidade de parede
    for (row = 0; row < MAP_WIDTH; row++) { 
        for (column = 0; column < MAP_HEIGHT; column++) {
            if (rand() % 100 < 45) {
                map[row][column].terreno = '#';
            } else {
                map[row][column].terreno = '.';
            }
            map[row][column].tem_mob = -1;
        }
    }

    //aplica os algoritmos de geração
    for (int count = 0; count < 4; count++) map_algorithm1(map);
    for (int count = 0; count < 3; count++) map_algorithm2(map);
    map_lakes_algorithm(map);

    //adiciona escadas
    add_stairs(map, 4);

   //garante espaço no spawn do player
    for (row = 15; row <= 25; row++) {
        for (int column = 15; column <= 25; column++) {
            if (map[row][column].terreno != '+') map[row][column].terreno = '.';
        }
    }

    //adiciona paredes externas
    for (row = 0; row < MAP_WIDTH; row++) {
        map[row][0].terreno = '#';
        map[row][MAP_HEIGHT - 1].terreno = '#';
    }
    for (column = 0; column < MAP_HEIGHT; column++) {
        map[0][column].terreno = '#';
        map[MAP_WIDTH - 1][column].terreno = '#';
    }

}

void find_space (STATE *s, int row, int column) {
    int nearest_row = -1;
    int nearest_column = -1;
    int min_distance = 100000;

    for (int search_row = 0; search_row < MAP_WIDTH; search_row++) {
        for (int search_col = 0; search_col < MAP_HEIGHT; search_col++) {
            if (s->mapa[search_row][search_col].terreno == '.' || s->mapa[search_row][search_col].terreno == '+') {
                int distance = abs(search_row - row) + abs(search_col - column);
                if (distance < min_distance) {
                    min_distance = distance;
                    nearest_row = search_row;
                    nearest_column = search_col;
                }
            }
        }
    }

    if (nearest_row != -1 && nearest_column != -1) {
        s->p1.playerX = nearest_row;
        s->p1.playerY = nearest_column;
    }
}

void cave_gen (STATE *s) {
    if (s->mapa[s->p1.playerX][s->p1.playerY].terreno == '<' && s->level == 1) {
        for (int row = 0; row < MAP_WIDTH; row++) {
            for (int column = 0; column < MAP_HEIGHT; column++) {
                s->save_map[row][column] = s->mapa[row][column];
            }
        }

        char new_map[MAP_WIDTH][MAP_HEIGHT];
        for (int row = 0; row < MAP_WIDTH; row++) {
            for (int column = 0; column < MAP_HEIGHT; column++) {
                new_map[row][column] = '#';
            }
        }

        replace_map(s->mapa, new_map);
        srand(time(NULL));

        int center_row = s->p1.playerX + 1; //linha do centro do raio
        if (center_row >= MAP_WIDTH) { 
            center_row = MAP_WIDTH - 1 - rand() % 5; //ajustes, se o centro sair fora do mapa
        } else if (center_row < 0) {
            center_row = rand() % 5;
        }
        int center_column = s->p1.playerY; //coluna do centro do raio
        if (center_column >= MAP_HEIGHT) {
            center_column = MAP_HEIGHT - 1 - rand() % 5; //ajustes, se o centro sair fora do mapa
        } else if (center_column < 0) {
            center_column = rand() % 5;
        }
        int radius = rand() % 6 + 5; //raio aleatório para a caverna, entre 5 e 10

        create_cave(center_row, center_column, radius, new_map); //cria o círculo para a caverna
        new_map[center_row][center_column] = '<'; //escada para voltar
        replace_map(s->mapa, new_map);
        criar(s, 40); //respawnar os mobs
        calculate_fov(s); //recalcular o fov
    }

    if (s->mapa[s->p1.playerX][s->p1.playerY].terreno == '<' && s->level == 2) {
        for (int row = 0; row < MAP_WIDTH; row++) {
            for (int column = 0; column < MAP_HEIGHT; column++) {
                s->mapa[row][column] = s->save_map[row][column];
                criar(s, 40);
            }
        }
        find_space(s, s->p1.playerX, s->p1.playerY);
    }
}

void draw_visible_map (Mapa map[MAP_WIDTH][MAP_HEIGHT]) {
    int row, column;

    for (row = 0; row < MAP_WIDTH; row++) {
        for (column = 0; column < MAP_HEIGHT; column++) {
            char tile = map[row][column].terreno;
            int color = 0; //par foreground/background das cores

            if (map[row][column].e_iluminado) {
                switch (tile) {
                    case '#': color = COLOR_BLACK; break;
                    case '.': color = 20; break;
                    case '+': color = 44; break;
                    case '<': color = 31; break;
                    case '%': color = 33; break;
                    default: color = 0; break;
                }
            } else {
                tile = ' ';
            }
            attron(COLOR_PAIR(color));
            mvaddch(row, column, tile);
            attroff(COLOR_PAIR(color)); 
        }
    }
}

void draw_map (Mapa map[MAP_WIDTH][MAP_HEIGHT]) {
    int row, column;

    for (row = 0; row < MAP_WIDTH; row++) {
        for (column = 0; column < MAP_HEIGHT; column++) {
            char tile = map[row][column].terreno;
            int color = 0;
            switch (tile) {
                case '#': color = COLOR_WHITE; break;
                case '.': color = 20; break;
                case '+': color = 44; break;
                case '<': color = 31; break;
                case '%': color = 33; break;
                default: color = 0; break;
            }
            attron(COLOR_PAIR(color));
            mvaddch(row, column, tile);
            attroff(COLOR_PAIR(color)); 
        }
    }
}
