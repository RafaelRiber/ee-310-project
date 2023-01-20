#ifndef BATTLESHIPS_H
#define BATTLESHIPS_H

//#define DEBUG 1

#include "stdint.h"
#include "stdlib.h"
#include "nds.h"
#include "graphics.h"
#include "timers_interrupts.h"
#include "sounds.h"
#include <stdbool.h>
#include <string.h> /* memset */
#include "message_protocol.h"
#include "storage.h"

//The 1990 Milton Bradley version
enum ship_type{CARRIER, BATTLESHIP, CRUISER, SUBMARINE, DESTROYER};

#define BRD_LEN 10

#define NUM_SHIPS 5

#define CARRIER_SIZE 5
#define BATTLESHIP_SIZE 4
#define CRUISER_SIZE 3
#define SUBMARINE_SIZE 3
#define DESTROYER_SIZE 2

extern const int SHIP_SIZES[NUM_SHIPS];
extern int place_ship_count;

extern uint8_t shots[BRD_LEN][BRD_LEN];


//conserve space by having one byte for board 
//coordinates because why not.
#define GET_X(coord) ((coord & 0xF0) >> 4)
#define GET_Y(coord) (coord & 0x0F)
#define SET_X(coord, x) ((coord & 0x0F) | (x << 4))
#define SET_Y(coord, y) ((coord & 0xF0) | (y))

//get is_hit for coord i of hits bitmap
#define GET_HIT(hits, i) ((hits >> i) & 0x1)
#define SET_HIT(hits, i) (hits | (0x1 << i))

typedef struct ship {
    uint8_t len;
    uint8_t coords[CARRIER_SIZE]; //car size is the biggest ship so it will always be enough.
    uint8_t hits; //bit map to conserve space :)
    u16 *sprite_buffs[CARRIER_SIZE];
    uint8_t is_hidden;
    uint8_t is_horizontal;
} ship;

typedef struct target {
    uint8_t coords;
    u16 *sprite_buff;
    uint8_t is_hidden;
} target;

extern target player_target;
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
	STATE_PLACE_SHIPS,
	STATE_WAIT_FOR_ENEMY_PLACEMENT,
    STATE_WAITING_FOR_TURN,
    STATE_TAKING_TURN,
    STATE_CHECKING_WIN,
    STATE_WIN,
    STATE_LOSE
} GameState;

typedef enum {
    MAIN_MENU,
	WAIT,
    SHIP_PLACE, 
    GAME
} game_screens;
#define NUM_SCREENS 4

bool is_ship_overlapped();
void place_ships();
void place_target();
void recv_enemy_ships();

bool all_ships_received();

bool game_won();

bool game_lost();

void update_state(GameState* state);
void take_shot(int x_current, int y_current, GameState *state);
void set_ship_coords(ship * s, int x, int y);
void set_target_coords(int x, int y);
void writeShipBuffer(char *buff);
void initEnemyShips(char *buff);
void readShots(char *buff);
#endif
