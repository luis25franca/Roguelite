#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#include "state.h"
#include "save_load.h"

void save_game(STATE *st) {
    FILE *file = fopen("savefile.txt", "wb"); // Abre o arquivo "savefile.txt" em modo de escrita binária ("wb").
    if (file == NULL) {
        mvprintw(28,MAP_HEIGHT+3,"Failed to create save file.\n");
        return;
    }

    fwrite(st, sizeof(STATE), 1, file); // Grava os dados contidos na estrutura de dados STATE (apontada por *st) no arquivo usando a função fwrite.
    fclose(file);

    mvprintw(30,MAP_HEIGHT+3,"Jogo salvo com sucesso");
}

void load_game(STATE *st) {
    FILE *file = fopen("savefile.txt", "rb"); // Abre o arquivo "savefile.txt" em modo de leitura binária ("rb").
    if (file == NULL) {
        mvprintw((38/2)+14, (150/2)-18, "Save não encontrado");
        return;
    }

    size_t items_read = fread(st, sizeof(STATE), 1, file); // Lê os dados do arquivo e armazena-os na estrutura de dados STATE (apontada por *st) usando a função fread.
    if (items_read != 1) {
    mvprintw((38/2)+14, (150/2)-18, "Erro a ler o ficheiro");
    return;
}
    fclose(file);
}
