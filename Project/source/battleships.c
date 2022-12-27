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

// Game FSM

bool start_hosting(){

	return true;
}

bool join_game(){

	return true;
}

void place_ships(){


	return;
}

bool all_ships_placed() {
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

void recv_enemy_ships(){

	return;
}

bool my_turn() {
    // Check a flag that indicates whose turn it is
    // return my_turn;
	return true;
}

bool turn_ended() {
    // Check if the player has fired at a square on the board or if they have run out of time to take their turn
	// time_expired checked via timer/interrupt
	// return shot_fired || time_expired;
	return true;
}

bool game_won() {
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
    switch (*state) {
    	case STATE_HOMESCREEN:
    		// Display Homescreen and transition either to hosting game or joining game

    		if (start_hosting()){
    			// Setup socket and wait for request_join
    		}
    		else if (join_game()){
    			// Send join request through socket and wait for host ships
    		}

    		break;
        case STATE_INITIALIZING:
            // Initialize game board, ships, etc.
        	init_ships();
            *state = STATE_PLACING_SHIPS;
            break;
        case STATE_PLACING_SHIPS:
            // Allow player to place ships on the board

        	place_ships();

            if (all_ships_placed()) {
                *state = STATE_WAIT_FOR_ENEMY_PLACEMENT;
            }
            break;
        case STATE_WAIT_FOR_ENEMY_PLACEMENT:
        	// Wait until enemy has placed ships and sent them
        	// Set enemy ships in local memory
        	recv_enemy_ships();

        	*state = STATE_TAKING_TURN;
        	break;
        case STATE_WAITING_FOR_TURN:
            // Wait for other player to take their turn
            if (my_turn()) {
                *state = STATE_TAKING_TURN;
            }
            break;
        case STATE_TAKING_TURN:
            // Allow player to take their turn by firing at a square on the board
            if (turn_ended()) {
                *state = STATE_CHECKING_WIN;
            }
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
            // Display "you win" message

        	// Increment wins stat in file

        	// Prompt for restart
//        	if(play_again()){
//        		*state = STATE_HOMESCREEN;
//        	}
            break;
        case STATE_LOSE:
            // Display "you lose" message

        	// Prompt for restart
//        	if(play_again()){
//        		*state = STATE_HOMESCREEN;
//        	}
            break;
    }
}
