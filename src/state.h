#ifndef ___STATE_H___
#define ___STATE_H___

#define MAP_WIDTH 38
#define MAP_HEIGHT 150
#define MAX_OBJETOS 20
#define MAX_MOBS 25
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct player { //todos os dados referentes ao player
	int hp;
	int hp_max;
	int mana_max;
	int mana;
	int playerX;
	int playerY;
	char atk; //ataque selecionado
	int dir_x;
	int dir_y;
	int p_level; //nivel do jogador
} Player;


typedef struct mapa { //dados referentes a cada espaço do mapa
	int mapaX;
	int mapaY;
	char terreno;
	int e_iluminado;
	int foi_iluminado;
	int tem_mob;
	int dist;
} Mapa;

typedef struct mobs { //dados referentes aos mobs
	int mobX;
	int mobY;
	int hp;
	char kind;
} Mobs;


typedef struct state { //estrutura de dados acerca do estado do jogo
	Player p1;
	Mapa mapa[MAP_WIDTH][MAP_HEIGHT];
	Mapa save_map[MAP_WIDTH][MAP_HEIGHT];
	Mobs mob[50];
	int key_presses;
	int mobnum;
	int level;
	int wave;
	int turns;
} STATE;

#endif
