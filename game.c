#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum {
    RUNNING,
    WIN,
    EXIT
}GameState ;

GameState game1;


#define ROWS 6
#define COLS 10

char map[ROWS][COLS + 1] = {
    "##########",
    "#..S....##",
    "#.#####.##",
    "#.....##.#",
    "####...E##",
    "##########"
};
int escapeX;
int escapeY;



void setEscapeCoord(char map[ROWS][COLS + 1]) {

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            if (map[i][j] == 'E') {
                escapeX = j;
                escapeY = i;
                return;
            }

        }
    }
}

void printMap (char map[ROWS][COLS + 1]) {

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS ; j++) {
            printf("%c", map[i][j]);

        }
        printf("\n");
    }

    printf("\n");
}

void clearScreen() {
    printf("\033[2J\033[H");
}




typedef struct {

    int posX;
    int posY;
    int spawned; //  1/0
    int numMosse;

}Player;


void spawnPlayer(Player *p, char map[ROWS][COLS + 1]) {

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            if (map[i][j] == 'S') {
                p->spawned = 1;
                p->numMosse = 15;
                p->posX = j;
                p->posY = i;
                return;
            }
        }
    }

    // If we reach this point, no spawn point was found
    printf("ERROR SPAWN\n");
    p->spawned = 0;
}


/* update the map with the new player position */
void updateMap (char map[ROWS][COLS + 1], Player *p) {

    for (int i = 0 ; i < ROWS ; i++) {
        for (int j = 0 ; j < COLS ; j++) {


            if (map[i][j] == '@' || map[i][j] == 'S') map[i][j] = '.';

            if (i == p->posY && j == p->posX) {
                map[i][j] = '@';
            }

        }
    }

    clearScreen();
    printMap(map);
}



/* Check if the input move goes into a wall or not, if yes it will return 0, else the move is possible, so return 1 */
void movePlayer (int x, int y, char map [ROWS][COLS + 1], Player* player) {
    if (map[y][x] != '#') {
        player->posX = x;
        player->posY = y;
        player->numMosse--;
        updateMap(map, player);
    } else {
        player->numMosse--;
        clearScreen();
        printMap(map);
    }
}





int main (void) {

     game1 = RUNNING;


     printf("PARTITA INIZIATA\n");
     printMap(map);

     Player player;
     player.numMosse = 15;
     spawnPlayer(&player, map);

     setEscapeCoord(map);


     while (game1 == RUNNING) {

         printf("MOSSE RIMANENTI : %d\n", player.numMosse - 1);

         char key = getchar();

         int x = player.posX;
         int y = player.posY;

         switch (key) {
                   case 'w': y--; break;
                   case 's': y++; break;
                   case 'a': x--; break;
                   case 'd': x++; break;
                   default:
                       continue;
               }

        movePlayer(x,y,map,&player);

        if (player.posX == escapeX && player.posY == escapeY) {
             game1 = WIN;
             break;
        }

        if (player.numMosse == 0) { game1 = EXIT; break; }
     }

     if (game1 == WIN) {
         printf("VINTO\n");
     } else if (game1 == EXIT) {
         printf("PERSO\n");
     }
     return 0;


}
