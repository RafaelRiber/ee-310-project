#include "battleships.h"
#include <nds.h>
#include <stdio.h>
ship player_ships[NUM_SHIPS];
ship enemy_ships[NUM_SHIPS];

void init_ships(void) {
    player_ships[CARRIER].len = CARRIER_SIZE;
    player_ships[BATTLESHIP].len = BATTLESHIP_SIZE;
    player_ships[CRUISER].len = CRUISER_SIZE;
    player_ships[SUBMARINE].len = SUBMARINE_SIZE;
    player_ships[DESTROYER].len = DESTROYER_SIZE;

    enemy_ships[CARRIER].len = CARRIER_SIZE;
    enemy_ships[BATTLESHIP].len = BATTLESHIP_SIZE;
    enemy_ships[CRUISER].len = CRUISER_SIZE;
    enemy_ships[SUBMARINE].len = SUBMARINE_SIZE;
    enemy_ships[DESTROYER].len = DESTROYER_SIZE;


    int i;
    int j;
    for (i = 0; i < NUM_SHIPS; i ++) {
        player_ships[i].hits = 0;
        enemy_ships[i].hits = 0;
        memset(enemy_ships[i].coords, 0, CARRIER_SIZE);
        memset(player_ships[i].coords, 0, CARRIER_SIZE);
        
    }

    enemy_ships[0].coords[0] = SET_X(enemy_ships[0].coords[0], 0);
    enemy_ships[0].coords[0]  = SET_Y(enemy_ships[0].coords[0], 0);

    enemy_ships[0].coords[1] = SET_X(enemy_ships[0].coords[1], 1);
    enemy_ships[0].coords[1]  = SET_Y(enemy_ships[0].coords[1], 0);

    enemy_ships[0].coords[2] = SET_X(enemy_ships[0].coords[2], 2);
    enemy_ships[0].coords[2]  = SET_Y(enemy_ships[0].coords[2], 0);

    enemy_ships[0].coords[3] = SET_X(enemy_ships[0].coords[3], 3);
    enemy_ships[0].coords[3]  = SET_Y(enemy_ships[0].coords[3], 0);

    enemy_ships[0].coords[4] = SET_X(enemy_ships[0].coords[4], 4);
    enemy_ships[0].coords[4]  = SET_Y(enemy_ships[0].coords[4], 0);
    printf("%d %d\n", GET_X(enemy_ships[0].coords[4]), GET_Y(enemy_ships[0].coords[4]));
    return;
}
