#include "battleships.h"
#include <nds.h>
#include "text.h"
#include <stdio.h>

ship player_ships[NUM_SHIPS];
ship enemy_ships[NUM_SHIPS];

const int SHIP_SIZES[NUM_SHIPS]= {
    [CARRIER] = CARRIER_SIZE,
    [BATTLESHIP] = BATTLESHIP_SIZE,
    [CRUISER] = CRUISER_SIZE,
    [SUBMARINE] = SUBMARINE_SIZE,
    [DESTROYER] = DESTROYER_SIZE
};

target player_target;
bool hosting = false;
int place_ship_count = 0;

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
		player_ships[i].is_hidden = 1;
		player_ships[i].is_horizontal = 1;
        enemy_ships[i].hits = 0;
        memset(enemy_ships[i].coords, 0, CARRIER_SIZE);
        memset(player_ships[i].coords, 0, CARRIER_SIZE);

        // Initial coords of ship are (0,0)
        set_ship_coords(&player_ships[i], 0, 0);
        set_ship_coords(&enemy_ships[i], 0, 0);

    }

	player_target.is_hidden = 1;
	return;
}
void set_ship_coords(ship * s, int x, int y) {
    int i;
    for (i = 0; i < s->len ; i ++) {
        if (s->is_horizontal) {
            s->coords[i] = SET_X(s->coords[i], (x + i));
            s->coords[i] = SET_Y(s->coords[i], y);
        } else {
            s->coords[i] = SET_X(s->coords[i], x);
            s->coords[i] = SET_Y(s->coords[i], y + i);
        }
    }
}

void set_target_coords(int x, int y) {
	player_target.coords = SET_X(player_target.coords, x);
	player_target.coords = SET_Y(player_target.coords, y);
}


// Game FSM

// Function to check if ship overlaps with previously placed ships
bool is_ship_overlapped(int ship_index) {
    int i, j, k;
    for(i = 0; i < ship_index; i++) {
        if(i == ship_index) continue;
        for(j = 0; j < SHIP_SIZES[i]; j++) {
            for (k = 0; k < SHIP_SIZES[ship_index]; k++) {
                if(player_ships[i].coords[j] == player_ships[ship_index].coords[k])
                    return true;
            }
        }
    }
    return false;
}


void place_ships() {
	irqEnable(IRQ_TIMER0);
    scanKeys();
    u16 keys = keysDown();

    //player_ships[0].is_hidden = 0;

    int x_current = GET_X(player_ships[place_ship_count].coords[0]);
    int y_current = GET_Y(player_ships[place_ship_count].coords[0]);

    switch (keys) {
    case KEY_UP:
        if (y_current > 0) set_ship_coords(&player_ships[place_ship_count], x_current, y_current - 1);
        break;
    case KEY_DOWN:
    	// Keep ship inside board bounds
        if(player_ships[place_ship_count].is_horizontal){
            if (y_current < BRD_LEN - 1) set_ship_coords(&player_ships[place_ship_count], x_current, y_current + 1);
        }
        else {
            if (y_current < BRD_LEN - player_ships[place_ship_count].len) set_ship_coords(&player_ships[place_ship_count], x_current, y_current + 1);
        }
        break;
    case KEY_RIGHT:
    	// Keep ship inside board bounds
        if(player_ships[place_ship_count].is_horizontal){
            if (x_current < BRD_LEN - player_ships[place_ship_count].len) set_ship_coords(&player_ships[place_ship_count], x_current + 1, y_current);
        }
        else {
            if (x_current < BRD_LEN - 1) set_ship_coords(&player_ships[place_ship_count], x_current + 1, y_current);
        }
        break;
    case KEY_LEFT:
        if (x_current > 0) set_ship_coords(&player_ships[place_ship_count], x_current - 1, y_current);
        break;
    case KEY_B:
    	player_ships[place_ship_count].is_horizontal = !(player_ships[place_ship_count].is_horizontal);

    	// Handle if ship is too close to a wall when rotating
        if(player_ships[place_ship_count].is_horizontal){
            if (x_current > BRD_LEN - player_ships[place_ship_count].len)
                x_current = BRD_LEN - player_ships[place_ship_count].len;
        }
        else{
            if(y_current > BRD_LEN - player_ships[place_ship_count].len)
                y_current = BRD_LEN - player_ships[place_ship_count].len;
        }
        set_ship_coords(&player_ships[place_ship_count], x_current, y_current);
        break;
    case KEY_A:
		if (place_ship_count < NUM_SHIPS) {
			// Prevent ship placement on top of previously placed ships
			if (!is_ship_overlapped(place_ship_count)) {
				place_ship_count++;
				play_sound_effect(SFX_GUN);
				player_ships[place_ship_count].is_hidden = 0; // Show the next ship.
			}
		}
		break;
    }
    // Prevent ships from being stuck hidden if place happens while ship is hidden due to blink
    int i;
    for (i = 0; i<place_ship_count; i++) {
    	if (player_ships[i].is_hidden) player_ships[i].is_hidden = 0;
    }
    irqDisable(IRQ_TIMER0);
}

void place_target() {
	//irqEnable(IRQ_TIMER0);
    scanKeys();
    u16 keys = keysDown();
    player_target.is_hidden = 0;

    int x_current = GET_X(player_target.coords);
    int y_current = GET_Y(player_target.coords);

    switch (keys) {
    case KEY_UP:
    	if (y_current > 0) set_target_coords(x_current, y_current-1);
    	break;
    case KEY_DOWN:
    	if (y_current < BRD_LEN-1) set_target_coords(x_current, y_current+1);
    	break;
    case KEY_RIGHT:
    	if (x_current < BRD_LEN-1) set_target_coords(x_current+1, y_current);
        break;
    case KEY_LEFT:
    	if (x_current > 0) set_target_coords(x_current-1, y_current);

        break;
    case KEY_B:

        break;
    case KEY_A:

		break;
    }
    //irqDisable(IRQ_TIMER0);
}

bool all_ships_received() {
    // Iterate through all ships and check if they have been received
	int i;
	for (i = 0; i < NUM_SHIPS; i++) {
	        if (enemy_ships[i].len == 0) {
	            // at least one ship has not been received
	            return false;
	        }
	    }
	    // all ships have been received
	    return true;
}

bool game_won() {

	// A ship is sunk if (hits & 0x0F >> (8-len) =! 0)
	// So the game is won if this is the case for all ships


	// Check if all of the opponent's ships have been sunk
	int i, j;
	for (i = 0; i < NUM_SHIPS; i++) {
		for (j = 0; j < enemy_ships[i].len; j++) {
			if (!GET_HIT(enemy_ships[i].hits, j)) {
				// at least one cell of enemy ship has not been hit
				return false;
			}
		}
	}
	// all cells of all enemy ships have been hit
	return true;
}

bool game_lost() {
	int i, j;
    // Check if all of the player's ships have been sunk
	for (i = 0; i < NUM_SHIPS; i++) {
		for (j = 0; j < player_ships[i].len; j++) {
			if (!GET_HIT(player_ships[i].hits, j)) {
				// at least one cell of enemy ship has not been hit
				return false;
			}
		}
	}
	// all cells of all enemy ships have been hit
	return true;
}

void writeShipBuffer(char *buff){
	int buff_counter = 0;
	int i, j;
	for (i=0; i<NUM_SHIPS; i++){
		for (j = 0; j<player_ships[i].len; j++){
			buff[buff_counter] = (char) player_ships[i].coords[j];
			buff_counter++;
		}
	}
}

void initEnemyShips(char *buff) {
	int buff_counter = 0;
	int i, j;
	for (i = 0; i < NUM_SHIPS; i++) {
		for (j = 0; j < enemy_ships[i].len; j++) {
			enemy_ships[i].coords[j] = buff[buff_counter];
			buff_counter++;
		}
	}
}

void place_ships_transition(GameState *state) {
	char buff[MSG_SIZE];
	writeShipBuffer(buff);
#ifndef DEBUG
	sendMessage(SHIPS, buff);
#endif
	play_sound_effect(SFX_LETS_DO_THIS);
	hide_player_ships();
	load_backgrounds(WAIT);
	*state = STATE_WAIT_FOR_ENEMY_PLACEMENT;
}

void wait_placement_transition(GameState *state) {
	load_backgrounds(GAME);
	initEnemyShips(recv_buffer + HEADER_LEN);
	int i, j;
	for (i = 0; i < NUM_SHIPS; i++) {
		for (j = 0; j < enemy_ships[i].len; j++) {
			new_shot_sprite(0, GET_X(enemy_ships[i].coords[j]), GET_Y(enemy_ships[i].coords[j]));
		}
	}
	if (!hosting) {
		load_backgrounds(WAIT); //TODO: CHANGE TO MAIN SCREEN ?
		*state = STATE_WAITING_FOR_TURN;
	} else {
		*state = STATE_TAKING_TURN;
	}
}

// Update sprites with new shot (check if hit or miss)
void wait_turn_transition(GameState *state) {
	*state = STATE_TAKING_TURN;
}

void update_state(GameState* state) {
	touchPosition touch;
	touchRead(&touch);

	switch (*state) {

	case STATE_HOME:
		scanKeys();
		u16 keys = keysDown();
		// Handle "JOIN" touchscreen and button (A KEY)
		if (keys == KEY_A || (touch.px > JOIN_BUTTON_LEFT && touch.px < JOIN_BUTTON_RIGHT && touch.py < JOIN_BUTTON_BOTTOM && touch.py > JOIN_BUTTON_TOP)) {
			play_sound_effect(SFX_GUN);
			hosting = false;
			*state = STATE_JOIN;
			load_backgrounds(WAIT);
		}
		// Handle "HOST" touchscreen button
		else if (keys == KEY_B || (touch.px > HOST_BUTTON_LEFT && touch.px < HOST_BUTTON_RIGHT && touch.py < HOST_BUTTON_BOTTOM && touch.py > HOST_BUTTON_TOP)){
			play_sound_effect(SFX_GUN);
			hosting = true;
			*state = STATE_HOST;
			load_backgrounds(WAIT);
		}
    	break;
    case STATE_HOST:
    	// Wait for other player to join
		#ifndef DEBUG
    	if(recvMessage(JOIN) > 0) {
			load_backgrounds(SHIP_PLACE);
			*state = STATE_PLACE_SHIPS;
			sendMessage(ACK,NULL);
    	}
		#endif
		#ifdef DEBUG
    	load_backgrounds(SHIP_PLACE);
    	*state = STATE_PLACE_SHIPS;
		#endif

    	// When player has joined, prompt to start and transition to start
    	// TODO : ADD USER INPUT HANDLING FOR STARTING GAME
    	// TODO : HANDLE WAITING FOR PLAYER
//		load_backgrounds(SHIP_PLACE);
//    	*state = STATE_PLACE_SHIPS;
    	break;
    case STATE_JOIN:
    	// TODO : Send join message and wait for start.
		#ifndef DEBUG
		sendMessage(JOIN, NULL);
    	if (recvMessage(ACK) > 0) {
			load_backgrounds(GAME);
			*state = STATE_PLACE_SHIPS;
		}
		#endif
		#ifdef DEBUG
    	load_backgrounds(GAME);
    	*state = STATE_PLACE_SHIPS;
		#endif
    	break;
    case STATE_PLACE_SHIPS:
		place_ships();
		if (place_ship_count == NUM_SHIPS) {
			place_ships_transition(state);
		}
    	break;
    case STATE_WAIT_FOR_ENEMY_PLACEMENT:
		#ifndef DEBUG
		if (recvMessage(SHIPS)) {
			wait_placement_transition(state);
		}
		#endif
		#ifdef DEBUG
		load_backgrounds(GAME);
		writeShipBuffer(send_buffer);
		initEnemyShips(send_buffer);
		clearBuffer(send_buffer);
		#endif
    	break;

    case STATE_WAITING_FOR_TURN:
    	if (recvMessage(SHOT)){
			wait_turn_transition(state);
    	}
    	break;

    case STATE_TAKING_TURN:
        // TODO: Handle user input
    	place_target();

        //if (turn_ended()) {
            //*state = STATE_CHECKING_WIN;
        //}
        break;
    case STATE_CHECKING_WIN:
		// Check if the game has been won or lost
		if (game_won()) {
			*state = STATE_WIN;
		} else if (game_lost()) {
			*state = STATE_LOSE;
		} else {
			*state = STATE_WAITING_FOR_TURN;
		}
		break;
    case STATE_WIN:
		// TODO: Display "you win" message
		// TODO: Increment wins stat in file

		// TODO: Prompt for restart
//        	if(play_again()){
//        		*state = STATE_HOMESCREEN;
//        	}
		break;
	case STATE_LOSE:
		// TODO: Display "you lose" message

		// TODO: Prompt for restart
//        	if(play_again()){
//        		*state = STATE_HOMESCREEN;
//        	}
		break;
	}
}
