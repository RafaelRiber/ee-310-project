#include "battleships.h"
#include <nds.h>
#include <stdio.h>
ship player_ships[NUM_SHIPS];
ship enemy_ships[NUM_SHIPS];

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

// Game FSM
void place_ships(){


	return;
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
    switch (*state) {
    case STATE_HOME:

    	//TODO: DETERMINE VALUE OF HOSTING BOOLEAN FROM USER INPUT



    	if (hosting) {
			*state = STATE_HOST;
		} else if (!hosting) {
			*state = STATE_JOIN;
		}
    	break;
    case STATE_HOST:
    	// Wait for other player to join
    	// When player has joined, prompt to start and transition to start
    	// TODO : ADD USER INPUT HANDLING FOR STARTING GAME
    	// TODO : HANDLE WAITING FOR PLAYER
    	*state = STATE_START_GAME;
    	break;
    case STATE_JOIN:
    	// TODO : Send join message and wait for start.
    	*state = STATE_START_GAME;
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
		if (all_ships_placed()) {
			//TODO: ADD send_ships();
			if (hosting){
				*state = STATE_WAIT_FOR_ENEMY_PLACEMENT;
			}
			else {
				*state = STATE_WAITING_FOR_TURN;
			}
		}
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
