#include "battleships.h"
#include <nds.h>
#include <stdio.h>

ship player_ships[NUM_SHIPS];
ship enemy_ships[NUM_SHIPS];

target player_target;
bool hosting = false;

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
        
    }

	player_target.is_hidden = 1;
	return;
}
void set_ship_coords(ship * s, int x, int y) {
    int i;
    for (i = 0; i < s->len ; i ++) {
        if (s->is_horizontal) {
            s->coords[i] = SET_X(s->coords[i], x + i);
            s->coords[i] = SET_Y(s->coords[i], y);
        } else {
            s->coords[i] = SET_X(s->coords[i], x);
            s->coords[i] = SET_Y(s->coords[i], y + i);
        }
    }
}

void set_target_coords(int x, int y) {
	player_target.coords  = SET_X(player_target.coords, x);
	player_target.coords = SET_Y(player_target.coords, y);
}


// Game FSM

void place_ships() {
    scanKeys();
    u16 keys = keysDown();
    int ship_count = 0;

    if (player_ships[ship_count].is_hidden == 1)
        player_ships[ship_count].is_hidden = 0;

    int x_current = GET_X(player_ships[ship_count].coords[0]);
    int y_current = GET_Y(player_ships[ship_count].coords[0]);

    switch (keys) {
    case KEY_UP:
        if (y_current > 0)
            set_ship_coords(&player_ships[ship_count], x_current, y_current - 1);
        break;
    case KEY_DOWN:
        if(player_ships[ship_count].is_horizontal){
            if (y_current < 9) set_ship_coords(&player_ships[ship_count], x_current, y_current + 1);
        }
        else {
            if (y_current < 10 - player_ships[ship_count].len) set_ship_coords(&player_ships[ship_count], x_current, y_current + 1);
        }
        break;
    case KEY_RIGHT:
        if(player_ships[ship_count].is_horizontal){
            if (x_current < 10 - player_ships[ship_count].len) set_ship_coords(&player_ships[ship_count], x_current + 1, y_current);
        }
        else {
            if (x_current < 9) set_ship_coords(&player_ships[ship_count], x_current + 1, y_current);
        }
        break;
    case KEY_LEFT:
        if(player_ships[ship_count].is_horizontal){
            if (x_current > 0) set_ship_coords(&player_ships[ship_count], x_current - 1, y_current);
        }
        else {
            if (x_current > 0) set_ship_coords(&player_ships[ship_count], x_current - 1, y_current);
        }
        break;
    case KEY_B:
    	player_ships[ship_count].is_horizontal = !(player_ships[ship_count].is_horizontal);

    	// Handle if ship is too close to a wall when rotating
        if(player_ships[ship_count].is_horizontal){
            if (x_current > 10 - player_ships[ship_count].len)
                x_current = 10 - player_ships[ship_count].len;
        }
        else{
            if(y_current > 10 - player_ships[ship_count].len)
                y_current = 10 - player_ships[ship_count].len;
        }
        set_ship_coords(&player_ships[ship_count], x_current, y_current);
        break;
    }
}





void recv_enemy_ships(){

	return;
}

bool all_ships_placed() {
	//TODO: rewrite this

    // Iterate through all ships and check if they have been placed on the board
	for (int i = 0; i < NUM_SHIPS; i++) {
	        if (player_ships[i].len == 0) {
	            // at least one ship has not been placed
	            return false;
	        }
	    }
	    // all ships have been placed
	    return true;
}

bool all_ships_received() {
    // Iterate through all ships and check if they have been received
	for (int i = 0; i < NUM_SHIPS; i++) {
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
	for (int i = 0; i < NUM_SHIPS; i++) {
		for (int j = 0; j < enemy_ships[i].len; j++) {
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
    // Check if all of the player's ships have been sunk
	for (int i = 0; i < NUM_SHIPS; i++) {
		for (int j = 0; j < player_ships[i].len; j++) {
			if (!GET_HIT(player_ships[i].hits, j)) {
				// at least one cell of enemy ship has not been hit
				return false;
			}
		}
	}
	// all cells of all enemy ships have been hit
	return true;
}

void update_state(GameState* state) {
	touchPosition touch;
	touchRead(&touch);

	switch (*state) {

	case STATE_HOME:
		scanKeys();
		u16 keys = keysDown();
		// Handle "JOIN" touchscreen and button (A KEY)
		if (keys == KEY_A || touch.px > JOIN_BUTTON_LEFT && touch.px < JOIN_BUTTON_RIGHT && touch.py < JOIN_BUTTON_BOTTOM && touch.py > JOIN_BUTTON_TOP) {
			hosting = false;
			*state = STATE_JOIN;
		}
		// Handle "HOST" touchscreen button
		else if (keys == KEY_B || touch.px > HOST_BUTTON_LEFT && touch.px < HOST_BUTTON_RIGHT && touch.py < HOST_BUTTON_BOTTOM && touch.py > HOST_BUTTON_TOP){
			hosting = true;
			*state = STATE_HOST;
		}
    	break;
    case STATE_HOST:
    	// Wait for other player to join
    	
		//load_backgrounds(HOST_WAIT);
    	// When player has joined, prompt to start and transition to start
    	// TODO : ADD USER INPUT HANDLING FOR STARTING GAME
    	// TODO : HANDLE WAITING FOR PLAYER
		load_backgrounds(GAME);
    	*state = STATE_PLACE_SHIPS;
    	break;
    case STATE_JOIN:
    	// TODO : Send join message and wait for start.

    	load_backgrounds(GAME);

    	//*state = STATE_START_GAME;
    	break;
    case STATE_START_GAME:
    	// if hosting, go to place ships
    	// if guest, go to wait for enemy placement
    	if (hosting) {
    		*state = STATE_PLACE_SHIPS;
    	}
    	else {
    		*state = STATE_WAIT_FOR_ENEMY_PLACEMENT;
    	}
    	break;
    case STATE_PLACE_SHIPS:

		place_ships();
//		if (all_ships_placed()) {
//			//TODO: ADD send_ships();
//			if (hosting){
//				*state = STATE_WAIT_FOR_ENEMY_PLACEMENT;
//			}
//			else {
//				*state = STATE_WAITING_FOR_TURN;
//			}
//		}
    	break;
    case STATE_WAIT_FOR_ENEMY_PLACEMENT:
    	recv_enemy_ships();
    	if (all_ships_received()){
    		if (hosting){
    			*state = STATE_TAKING_TURN;
    		}
    		else {
    			*state = STATE_PLACE_SHIPS;
    		}
    	}
    	break;

    case STATE_WAITING_FOR_TURN:
    	// Wait to receive enemy hits
    	// Go to check win
    	//if (recv_ships())
    	*state = STATE_CHECKING_WIN;
    	break;

    case STATE_TAKING_TURN:
        // TODO: Handle user input

    	 //new_shot_sprite(1, 5, 4);

        //if (turn_ended()) {
            *state = STATE_CHECKING_WIN;
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
