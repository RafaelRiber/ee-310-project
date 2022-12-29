#ifndef BATTLESHIPS_H
#define BATTLESHIPS_H

#include "stdint.h"
#include "stdlib.h"
#include <stdbool.h>
#include <string.h> /* memset */

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
#define GET_HIT(hits, i) ((hits >> i) & 0x1)
#define SET_HIT(hits, i) (hits | (0x1 << i))

typedef struct ship {
    uint8_t len;
    uint8_t coords[CARRIER_SIZE]; //car size is the biggest ship so it will always be enough.
    uint8_t hits; //bit map to conserve space :)
} ship;

//game data
extern ship player_ships[NUM_SHIPS];
extern ship enemy_ships[NUM_SHIPS];

extern bool hosting;

void init_ships(void);

// Game FSM

typedef enum {
	STATE_HOME,
	STATE_HOST,
    STATE_JOIN,
	STATE_START_GAME,
	STATE_PLACE_SHIPS,
	STATE_WAIT_FOR_ENEMY_PLACEMENT,
    STATE_WAITING_FOR_TURN,
    STATE_TAKING_TURN,
    STATE_CHECKING_WIN,
    STATE_WIN,
    STATE_LOSE
} GameState;

void place_ships();

void recv_enemy_ships();

bool all_ships_placed();

bool all_ships_received();

bool game_won();

bool game_lost();

void update_state(GameState* state);

#endif
