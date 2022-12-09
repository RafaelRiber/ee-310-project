#include "battleships.h"

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
    for (i = 0; i < NUM_SHIPS; i ++) {
        player_ships[i].hits = 0;
        enemy_ships[i].hits = 0;
        memset(enemy_ships[i].coords, 0, CARRIER_SIZE);
        memset(player_ships[i].coords, 0, CARRIER_SIZE);
    }


    return;
}
