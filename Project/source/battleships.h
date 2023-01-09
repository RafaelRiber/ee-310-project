#ifndef BATTLESHIPS_H
#define BATTLESHIPS_H

#include "stdint.h"
#include "stdlib.h"
#include "nds.h"
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
#define GET_X(coord) ((coord & 0xF0) >> 4)
#define GET_Y(coord) (coord & 0x0F)
#define SET_X(coord, x) ((coord & 0x0F) | (x << 4))
#define SET_Y(coord, y) ((coord & 0xF0) | (y))

//get is_hit for coord i of hits bitmap
#define GET_HIT(hits, i) ((hits >> i) & 0x1);
#define SET_HIT(hits, i) (hits | (0x1 << i));

typedef struct ship {
    uint8_t len;
    uint8_t coords[CARRIER_SIZE]; //car size is the biggest ship so it will always be enough.
    uint8_t hits; //bit map to conserve space :)
    u16 *sprite_buffs[CARRIER_SIZE];
} ship;

//game data
extern ship player_ships[NUM_SHIPS];
extern ship enemy_ships[NUM_SHIPS];

void init_ships(void);

#endif
