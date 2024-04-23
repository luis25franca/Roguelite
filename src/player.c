#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include "state.h"
#include "player.h"
#include "save_load.h"
#include "hud.h"

//A função wall verifica se um dado espaço é uma parede
int wall(Mapa m){
	int x=2;
	if (m.terreno == '#') {x=1;}
	else {x=0;}
	return x;
} 

//Atualiza a coordenada do jogador baseado num movimento
void do_movement_action(STATE *st, int dx, int dy) {
	st->p1.playerX += dx;
	st->p1.playerY += dy;
}
void slash (STATE *st, int dx, int dy) {
    int x =  st->p1.playerX;
    int y =  st->p1.playerY;  
    int lin,col;
    //verifica a direção em que é preciso atacar baseado nas 4 direções cardinais                            
        if(abs(dx) >= abs(dy)) { //caso o ataque seja para cima ou para baixo
            if(dx >= 0) {x++;} //baixo
            if(dx < 0) {x--;} //cima
            //atacar numa linha de 3 blocos baseado na direção do ataque
            for(lin = -1; lin< 2; lin++){
                mvchgat(x, y+lin, 1, A_NORMAL, 9, NULL);
                if(st->mapa[x][y+lin].tem_mob > -1){ //verifica se tem inimigo
                    int id_mob = st->mapa[x][y+lin].tem_mob;
                    st->mob[id_mob].hp--;} //retira hp ao inimigo
            }
        }
                                    
        if(abs(dx) < abs(dy)) { //caso o ataque seja para a esquerda ou para a direita
            if(dy >= 0) {y++;} //direita
            if(dy < 0) {y--;} //esqueda
            //atacar numa coluna de 3 blocos baseado na direção do ataque
            for(col = -1; col< 2; col++){
                mvchgat(x +col, y, 1, A_NORMAL, 9, NULL);
                    if(st->mapa[x+col][y].tem_mob > -1){ //ferifica se tem inimigo
                        int id_mob = st->mapa[x+col][y].tem_mob;
                        st->mob[id_mob].hp--;} //retira hp ao inimigo
            }
        }
    refresh();  
    sleep(1);
}
void bomb(STATE*st, int mouse_x, int mouse_y, double angulo_rato, int distancia_rato) {
    int x, y, lin ,col;
    if(st->p1.mana >= 5) { //verifica se tem mana suficiente
        //verificar se o rato está dentro do alcance do ataque
        if (distancia_rato < 5) {
            x = mouse_y;
            y = mouse_x;}
        //caso não esteja ataca na mesma direção mas na distância máxima que é permitida
        else {
            x = st->p1.playerX + 5 * sin(angulo_rato);
            y = st->p1.playerY + 5 * cos(angulo_rato);
        }
        //cria um quadrado de 3 por 3 correspondente à área de ataque
        for(lin = -1; lin < 2; lin++) {
            for(col = -1; col < 2; col++) {
                if (wall(st->mapa[x+lin][y+col]) != 1 && st->mapa[x+lin][y+col].e_iluminado == 1) {
                    mvchgat((x+lin), (y+col), 1, A_NORMAL, 10, NULL);
                    if(st->mapa[x+lin][y+col].tem_mob > -1){ //verifica se tem inimigo
                        int id_mob = st->mapa[x+col][y+lin].tem_mob;
                        st->mob[id_mob].hp-=2;} //retira hp ao inimigo
                    if(x + lin == st->p1.playerX && y+col == st->p1.playerY) {st->p1.hp -= 3;} //verifica se o jogador foi acertado
                }
            }
        }       
        st->p1.mana -= 10; //retira mana ao jogador
        refresh();
        sleep(1);}                               
}

void lightning(STATE *st, double angulo_rato) {
    int x,y;
    if(st->p1.mana >=5) { //verifica se tem mana suficiente
        double ray_x = cos(angulo_rato);
        double ray_y = sin(angulo_rato);
        double posX = st->p1.playerX;
        double posY = st->p1.playerY;
        //similar ao calculo da visão, cria um raio, bloco a bloco, avançando baseado no angulo do rato
        while((posX >= 0 && posX < MAP_WIDTH && posY >= 0 && posY < MAP_HEIGHT)) {
            posX += ray_y;
            posY += ray_x;
            x = round (posX);
            y = round (posY);
            if (st->mapa[x][y].terreno == '#') {break;}
            mvchgat((x), (y), 1, A_NORMAL, 11, NULL);
            if(st->mapa[x][y].tem_mob > -1){ //verifica se tem um inimigo
                int id_mob = st->mapa[x][y].tem_mob;
                st->mob[id_mob].hp--;} //retira hp ao inimigo          
        }
        st->p1.mana -= 5;//retira mana ao jogador
        }
                                    
}

void earthquake (STATE *st) {
   int x,y,lin,col;
   if (st->p1.mana >= 15) { //verifica se tem mana suficiente
        x = st->p1.playerX;
        y = st->p1.playerY;
        //cria um quadrado 6 por 6 à olta do jogador correpsondedo à áre ado ataque
        for(lin = -3; lin < 4; lin++) {
            for(col = -3; col < 4; col++) {
                if (wall(st->mapa[x+lin][y+col]) != 1 /*&& st->mapa[x+lin][y+col].e_iluminado!= 1*/) {
                    mvchgat((x+col), (y+lin), 1, A_NORMAL, 13, NULL);
                    if(st->mapa[x+lin][y+col].tem_mob > -1){ //verifica se tem inimigo
                        int id_mob = st->mapa[x+lin][y+col].tem_mob;
                        st->mob[id_mob].hp -= 3; //retira hp ao inimigo
                    }
                }
            }
        }
        st->p1.mana -= 15; //retira mana ao jogador
        refresh();
        sleep(1);} 
}

void cura (STATE *st) {
    int x,y;
    if (st->p1.mana >= 10) { //verifica se tem mana suficiente
        x = st->p1.playerX;
        y = st->p1.playerY; 
        mvchgat(x, y, 1, A_NORMAL, 12, NULL);
        refresh();
        sleep(1);
        if(st->p1.hp > st->p1.hp_max - 5) {st->p1.hp = st->p1.hp_max;} //verificação para ver se o valor da cura não passa do limite máximo
        else {st->p1.hp += 5;} //adiciona 5 hp ao jogador caso contrário
        st->p1.mana -= 10; //retira mana ao jogador
    }
}
//Função responsável por todos os movimentos e ataques do jogador.
void update(STATE *st) {
	int key = getch();
	mvaddch(st->p1.playerX, st->p1.playerY, ' ');
    cave_gen(st);
    MEVENT mouse_event; //linha necessária para a captura de eventos do rato
    int level_flag = 0;
    int lin, col;
    float pi = 3.14159;
    
    //o jogo reconhece que o player está numa caverna se houver um char '%'
    for (lin = 0; lin < MAP_WIDTH; lin++) {
        for (col = 0; col < MAP_HEIGHT; col++) {
            if (st->mapa[lin][col].terreno == '%') level_flag = 1;
        }
    }

    if (level_flag) st->level = 2;
    else st->level = 1; 

    //Captura das teclas do teclado e butões do rato para o movimento, seleção de armas, ataques e save.
    switch(key) {
        //movimento para a esquerda-cima
		case KEY_A1:
		case '7': 
        if (st->p1.mana < st->p1.mana_max) {st->p1.mana++;} //regeneração de mana
        st->key_presses++;
        if (wall(st->mapa[st->p1.playerX-1][st->p1.playerY-1]) == 1) {break;} //verifica se vai contra uma parede
        if (st->mapa[st->p1.playerX][st->p1.playerY].terreno == '+' && st->key_presses % 2 == 0) { //movimento na água
            break;
        } 
        else {do_movement_action(st, -1, -1); break;}
		
        //movimento para cima
        case KEY_UP:
		case '8':
        case 'w': 
        if (st->p1.mana < st->p1.mana_max) {st->p1.mana++;}
        st->key_presses++;
        if (st->mapa[st->p1.playerX-1][st->p1.playerY].terreno == '#') {break;}
        if (st->mapa[st->p1.playerX][st->p1.playerY].terreno == '+' && st->key_presses % 2 == 0) {
            break;
        } 
        else {do_movement_action(st, -1, +0); break;}
		
        //movimento para direita-cima
        case KEY_A3:
		case '9': 
        if (st->p1.mana < st->p1.mana_max) {st->p1.mana++;}
        st->key_presses++;
        if (wall(st->mapa[st->p1.playerX-1][st->p1.playerY+1]) == 1) {break;}
        if (st->mapa[st->p1.playerX][st->p1.playerY].terreno == '+' && st->key_presses % 2 == 0) {
            break;
        } 
        else {do_movement_action(st, -1, +1); break;}
		
        //movimento para a esquerda
        case KEY_LEFT:
		case '4': 
        case 'a':
        if (st->p1.mana < st->p1.mana_max) {st->p1.mana++;}
        st->key_presses++;
        if (wall(st->mapa[st->p1.playerX][st->p1.playerY-1]) == 1) {break;}
        if (st->mapa[st->p1.playerX][st->p1.playerY].terreno == '+' && st->key_presses % 2 == 0) {
            break;
        } 
        else {do_movement_action(st, +0, -1); break;}
		
        //ficar parado
        case KEY_B2:
		case '5': 
        if (st->p1.mana < st->p1.mana_max) {st->p1.mana++;}
        st->key_presses++;
        break;
		
        //movimento para a direita
        case KEY_RIGHT:
		case '6': 
        case 'd':
        if (st->p1.mana < st->p1.mana_max) {st->p1.mana++;}
        st->key_presses++;
        if (wall(st->mapa[st->p1.playerX][st->p1.playerY+1]) == 1) {break;}
        if (st->mapa[st->p1.playerX][st->p1.playerY].terreno == '+' && st->key_presses % 2 == 0) {
            break;
        } 
        else {do_movement_action(st, +0, +1); break;}
		
        //movimento esquerda-baixo
        case KEY_C1:
		case '1': 
        if (st->p1.mana < st->p1.mana_max) {st->p1.mana++;}
        st->key_presses++;
        if (wall(st->mapa[st->p1.playerX+1][st->p1.playerY-1]) == 1) {break;}
        if (st->mapa[st->p1.playerX][st->p1.playerY].terreno == '+' && st->key_presses % 2 == 0) {
            break;
        } 
        else {do_movement_action(st, +1, -1); break;}
		
        //movimento para baixo
        case KEY_DOWN:
        case '2': 
        case 's':
        if (st->p1.mana < st->p1.mana_max) {st->p1.mana++;}
        st->key_presses++;
        if (wall(st->mapa[st->p1.playerX+1][st->p1.playerY]) == 1) {break;}
        if (st->mapa[st->p1.playerX][st->p1.playerY].terreno == '+' && st->key_presses % 2 == 0) {
            break;
        } 
        else {do_movement_action(st, +1, +0); break;}
		
        //movimento para direita-baixo
        case KEY_C3:
		case '3': 
        if (st->p1.mana < st->p1.mana_max) {st->p1.mana++;}
        st->key_presses++;
        if (wall(st->mapa[st->p1.playerX+1][st->p1.playerY+1]) == 1) {break;}
        if (st->mapa[st->p1.playerX][st->p1.playerY].terreno == '+' && st->key_presses % 2 == 0) {
            break;
        } 
        else {do_movement_action(st, +1, +1); break;}
		
        case 'q': endwin(); exit(0); break; //sai do jogo

        case 'm': generate_map(st->mapa); st->p1.playerX = 20; st->p1.playerY = 20; criar(st, 40); break; //regenera o mapa
        case 'j' : st->p1.atk = 'j'; break; //seleciona slash
        case 'k' : st->p1.atk = 'k'; break; //seleciona bomba
        case 'l' : st->p1.atk = 'l'; break; //seleciona relampago
        case 'i' : st->p1.atk = 'i'; break; //seleciona terramoto
        case 'o' : st->p1.atk = 'o'; break; //seleciona cura
        case 'x': save_game(st); break; //guarda o jogo
        //captura dos inputs do mouse para os ataques.
        case KEY_MOUSE :      
                if (getmouse(&mouse_event) == OK) {           
                    if (mouse_event.bstate & BUTTON1_CLICKED) {
                            int mouse_x = mouse_event.x;
                            int mouse_y = mouse_event.y;
                            int dx = mouse_y - st->p1.playerX; //cálculo da distancia no eixo do x
                            int dy = mouse_x - st->p1.playerY; //cálculo da distancia no eixo do y
                            double distancia_rato = sqrt((dx*dx) + (dy*dy)); //cálculo da distancia do rato ao jogador
                            double angulo_rato; //cálculo do angulo do rato peranto o jogador considerando os casos particulares para caso o dx ou o dy sejam igual a 0
                            if (dy == 0) {if (dx >= 0) {angulo_rato = (pi/2);} else {angulo_rato = -(pi/2);}} 
                            if (dx == 0) {if (dy >= 0) {angulo_rato = (0);} else {angulo_rato = -(pi);}}
                            else { angulo_rato = atan2(dx, dy);}
                            //realização de um ataque correspondente ao que foi anteriormente selecioando
                            if(st->p1.atk == 'j') {
                                slash(st,dx,dy);
                                }
                            if(st->p1.atk == 'k') {
                                bomb(st, mouse_x, mouse_y, angulo_rato, distancia_rato);                                
                                }
                            if(st->p1.atk == 'l') {
                                lightning(st, angulo_rato);
                                }
                            if(st->p1.atk == 'i') {
                                earthquake(st);
                                }  
                            if(st->p1.atk == 'o') {
                                cura(st);
                                }
                            } break;
                            
                } {
    
    }
    } 
} 


//Função que desenha o jogador ou faz game over se a sua hp chegar a 0
void draw_player(STATE *st) {
    if (st->p1.hp == 0){
        clear();
        int choice;
        while(1){
            mvprintw((38/2)-5, (150/2)-15, " #####                          #######                      ");
            mvprintw((38/2)-4, (150/2)-15, "#     #   ##   #    # ######    #     # #    # ###### #####  ");
            mvprintw((38/2)-3, (150/2)-15, "#        #  #  ##  ## #         #     # #    # #      #    # ");
            mvprintw((38/2)-2, (150/2)-15, "#  #### #    # # ## # #####     #     # #    # #####  #    # ");
            mvprintw((38/2)-1, (150/2)-15, "#     # ###### #    # #         #     # #    # #      #####  ");
            mvprintw((38/2),   (150/2)-15, "#     # #    # #    # #         #     #  #  #  #      #   #  ");
            mvprintw((38/2)+1, (150/2)-15, " #####  #    # #    # ######    #######   ##   ###### #    # ");
            mvprintw((38/2)+3, (150/2), "Prima '1' para sair\n");
            mvprintw((38/2)+5, (150/2), "Pontuação final: %d\n", calcularPontuacao(st));
            refresh();
            choice = getch();
            clear();
            switch (choice) {
                case '1': //sai do jogo
                    endwin();
                    exit(0);
                    break;
                default:
                    mvprintw((38/2), (150/2), "Tecla inválida. Tente novamente\n");
                    getch();
                    break;
            }
        }
    }
    
    if (st->mapa[st->p1.playerX][st->p1.playerY].terreno == '+') { //altera o color_pair do jogador caso esteja na água 
        attron(COLOR_PAIR(COLOR_BLUE) | A_BOLD);
        mvaddch(st->p1.playerX, st->p1.playerY, '@');
        attroff(COLOR_PAIR(COLOR_BLUE) | A_BOLD);
    } else {
        mvaddch(st->p1.playerX, st->p1.playerY, '@' | A_BOLD); //desenha o jogador
    }
    move(st->p1.playerX, st->p1.playerY);
    update(st);
}
