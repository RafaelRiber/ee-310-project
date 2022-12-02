#ifndef BATTLESHIPS_H
#define BATTLESHIPS_H

#include "stdint.h"
#include "stdlib.h"
//The 1990 Milton Bradley version
enum ship_type{CARRIER, BATTLESHIP, CRUISER, SUBMARINE, DESTROYER};

#define NUM_SHIPS 5

#define CARRIER_SIZE 5
#define BATTLESHIP_SIZE 4
#define CRUISER_SIZE 3
#define SUBMARINE_SIZE 3
#define DESTROYER_SIZE 2

//conserve space by having one byte for board 
//coordinates because why not.
#define GET_X(coord) (coords & 0xF0)
#define GET_Y(coord) (coords & 0xF)
#define SET_X(coord, x) (coords & (x << 4))
#define SET_Y(coord, y) (coords & y)

//get is_hit for coord i of hits bitmap
#define GET_HIT(hits, i) ((hits >> i) & 0x1);
#define SET_HIT(hits, i) (hits | (0x1 << i));

typedef struct ship {
    uint8_t len;
    uint8_t coords[CARRIER_SIZE]; //car size is the biggest ship so it will always be enough.
    uint8_t hits; //bit map to conserve space :)
} ship;

//game data
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

#endif
