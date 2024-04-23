#include "state.h"
#ifndef MAPA_H
#define MAPA_H

void coordenadas (Mapa mapa[MAP_WIDTH][MAP_HEIGHT]);
void distancia (int R, int C, int value, STATE *s);
void map_algorithm1 (Mapa map[MAP_WIDTH][MAP_HEIGHT]); //tile vira parede se em quadrado 3x3 houver 5 paredes ou em 5x5 houver 2 ou menos paredes
void map_algorithm2 (Mapa map[MAP_WIDTH][MAP_HEIGHT]); //tile vira parede se em quadrado 5x5 não houver paredes
void map_lake_algorithm (Mapa map[MAP_WIDTH][MAP_HEIGHT]); //cria dois lagos em locais aleatorios
void add_stairs (Mapa map[MAP_WIDTH][MAP_HEIGHT], int stairs_number); //adiciona escadas para outros níveis
void cave_gen (STATE *s); //gera as cavernas
void create_lake(int center_row, int center_column, int radius, char map[MAP_WIDTH][MAP_HEIGHT]); //criação dos lagos
void create_cave(int center_row, int center_column, int radius, char new_map[MAP_WIDTH][MAP_HEIGHT]); //criação das cavernas
void replace_map (Mapa map[MAP_WIDTH][MAP_HEIGHT], char new_map[MAP_WIDTH][MAP_HEIGHT]); //transpõe mapa novo ao mapa anterior
void generate_map (Mapa map[MAP_WIDTH][MAP_HEIGHT]);
void find_space (STATE *s, int row, int column); //encontra espaço para o jogador aparecer quando voltar de uma caverna
void draw_visible_map (Mapa map[MAP_WIDTH][MAP_HEIGHT]);
void draw_map (Mapa map[MAP_WIDTH][MAP_HEIGHT]);
int count_walls (int row, int column, int span, Mapa map[MAP_WIDTH][MAP_HEIGHT]); //span: lado do quadrado centrado na tile (precisa ser ímpar)

#endif
