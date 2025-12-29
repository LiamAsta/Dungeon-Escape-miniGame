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



void ripristinaMappa(char map[ROWS][COLS + 1]) {

    if (map[0][0] != 'X') return;

    const char *mappa_iniziale[ROWS] = {
        "##########",
        "#..S....##",
        "#.#####.##",
        "#.....##.#",
        "####...E##",
        "##########"
    };

    for (int i = 0; i < ROWS; i++) {
        strncpy(map[i], mappa_iniziale[i], COLS + 1);
    }
}
void printMap (char map[ROWS][COLS + 1]) {
    for (int i = 0; i < ROWS; i++) {

        for (int j = 0; j < COLS ; j++) {
            printf("%c", map[i][j]);
            fflush(stdout);
        }

        printf("\n");
        fflush(stdout);
    }

    printf("\n");
}
void clearMap (char map[ROWS][COLS + 1]) {
    for (int i = 0; i < ROWS; i++) {

        for (int j = 0; j < COLS ; j++) {
            map[i][j] = 'X';
        }

    }
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

    char mappa_iniziale[ROWS][COLS + 1] = {
        "##########",
        "#..S....##",
        "#.#####.##",
        "#.....##.#",
        "####...E##",
        "##########"
    };

    mappa_iniziale[p->posY][p->posX] = '@';
    for (int i = 0; i < ROWS; i++) {
        strncpy(map[i], mappa_iniziale[i], COLS + 1);
    }

    clearScreen();
    printMap(map);
}


/* function that updates the game only if the player doesn go into a wall */
void update (char map[ROWS][COLS + 1], Player *p, int x, int y) {
    if (map[p->posY][p->posX] != '#') {
        p->posX = x;
        p->posY = y;
        p->numMosse--;
        updateMap(map, p);
    }
}


/* Check if the input move goes into a wall or not, if yes it will return 0, else the move is possible, so return 1 */
int isThisMovePossible (int x, int y, char map [ROWS][COLS + 1]) {
    if (map[y][x] != '#') return 1;
    return 0;
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

         clearScreen();printMap(map);

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

        if (isThisMovePossible(x,y,map)) {
            update(map, &player, x, y);
        } else {player.numMosse--; clearScreen();printMap(map); }

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
