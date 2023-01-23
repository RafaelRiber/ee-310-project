#include "battleships.h"
#include <nds.h>
#include "text.h"
#include <stdio.h>

// Player and enemy ship structures
ship player_ships[NUM_SHIPS];
ship enemy_ships[NUM_SHIPS];

// Text ID of the status text
int status_txt_id;

// Target structure for player aiming
target player_target;

// True if hosting game, false if joining game
bool hosting = false;

// Counter for ships placed
int place_ship_count = 0;

// Player and enemy shots array.
// Used to keep track of already placed shots to avoid stacking them
uint8_t shots[BRD_LEN][BRD_LEN];
uint8_t enemy_shots[BRD_LEN][BRD_LEN];

// Counter for number of ships received
int ships_received = 0;

/*
 * Count how many shots have been taken by player using shots array
 */

int count_shots() {
    int count = 0;
    int i,j;
    for (i = 0; i < BRD_LEN; i++) {
        for (j = 0; j < BRD_LEN; j++) {
            if (shots[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}

/*
 * Count the number of successful shots taken by player
 */

int count_hits() {
	int count = 0;
	int i;
	for (i = 0; i < NUM_SHIPS; i++) {
		count = count + enemy_ships[i].hits;
	}
	return count;
}

/*
 * Transition from STATE_WAIT_PLACEMENT to next state
 */

void wait_placement_transition(GameState *state);

/*
 * Game initialization
 */
void init_game(void) {
	// Set ship lengths
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

    // Initialize ship properties
    int i,j;
    for (i = 0; i < NUM_SHIPS; i ++) {
        player_ships[i].hits = 0;
        // Hide all ships at first
		player_ships[i].is_hidden = 1;
		player_ships[i].is_horizontal = 1;
        enemy_ships[i].hits = 0;
        memset(enemy_ships[i].coords, 0, CARRIER_SIZE);
        memset(player_ships[i].coords, 0, CARRIER_SIZE);

        // Initial coords of ship are (0,0)
        set_ship_coords(&player_ships[i], 0, 0);
        set_ship_coords(&enemy_ships[i], 0, 0);

    }
    // Set shot arrays to zero
    for (i = 0; i < BRD_LEN; i++){
    	for (j = 0; j < BRD_LEN; j++){
    		shots[i][j] = 0;
    		enemy_shots[i][j] = 0;
    	}
    }
    ships_received = 0;
    place_ship_count = 0;
	player_target.is_hidden = 1;
	set_target_coords(0,0);
	return;
}

/*
 * Moves a ship to a specified coordinate, and handles
 * transformation for horizontal or vertical
 */
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

/*
 * Moves the target to a specified coordinate
 */

void set_target_coords(int x, int y) {
	player_target.coords = SET_X(player_target.coords, x);
	player_target.coords = SET_Y(player_target.coords, y);
}

/*
 * Function to check if ship overlaps with previously placed ships.
 */
bool is_ship_overlapped(int ship_index) {
    int i, j, k;
    for(i = 0; i < ship_index; i++) {
        if(i == ship_index) continue;
        for(j = 0; j < player_ships[i].len; j++) {
            for (k = 0; k < player_ships[ship_index].len; k++) {
                if(player_ships[i].coords[j] == player_ships[ship_index].coords[k])
                    return true;
            }
        }
    }
    return false;
}

/*
 * Place ship on first available coordinates to
 * avoid ships appearing under already placed ships when starting to place them
 */

void set_to_free_coordinates(int id) {
	if (id == 0)
		return;
	int x = 0;
	int y = 0;
	int i = 0;

	for (x = 0; x < BRD_LEN ; x ++) {
		for (y = 0; y < BRD_LEN; y ++) {
			int can_place = 1;
		
			for ( i = 0; i < id; i ++) {
				
				int olen = player_ships[i].len;
				int k;
				for (k = 0; k < olen; k ++ ) {
					int ox = GET_X(player_ships[i].coords[k]);	
					int oy = GET_Y(player_ships[i].coords[k]);
					if (ox >= x && (ox <= x + player_ships[id].len) && oy == y) {
						can_place = 0;
					}
						
				}
			}

			if (can_place) {
				set_ship_coords(player_ships + id, x, y);
				return;
			}
				
		}
	}
	
}

/*
 * Place ships on the game board.
 * UP/DOWN/LEFT/RIGHT to move ship
 * B to rotate current ship
 * A to place ship
 * Ships are blinked with timer0 interrupt while being placed
 */
void place_ships() {
	irqEnable(IRQ_TIMER0);
    scanKeys();
    u16 keys = keysDown();

    // Get current ship coordinates
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
				set_to_free_coordinates(place_ship_count);
				play_sound_effect(SFX_GUN); // Play sound effect upon ship placement
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

/*
 * Check if a shot is successful
 */
bool shot_successful(target* target){
	int x,y,i,j;
	x = GET_X(target->coords);
	y = GET_Y(target->coords);
	// Check if target coordinates match with a block from an enemy ship
	for (i = 0; i < NUM_SHIPS; i++) {
			for (j = 0; j < enemy_ships[i].len; j++) {
				if (x == GET_X(enemy_ships[i].coords[j]) && y == GET_Y(enemy_ships[i].coords[j])){
					// Increment enemy ship hit counter if hit successful
					enemy_ships[i].hits++;
					return true;
				}
			}
		}
	return false;
}

/*
 * Place target on the gameboard
 * UP/DOWN/LEFT/RIGHT to move
 * A to place shot at target location
 * Sounds are played and tiles are displayed depending on shot success
 */

void place_target(GameState *state) {
	//irqEnable(IRQ_TIMER0);
    scanKeys();
    u16 keys = keysDown();
    show_player_ships();
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
    case KEY_A:; // Place a shot at target location, send it, and transition to next state
    	int isHit;
    	isHit = shot_successful(&player_target);
    	if (shots[x_current][y_current] == 0) {
    		//
    		new_shot_tile(isHit, x_current,y_current, 1);
    		if (isHit){
    			play_sound_effect(SFX_EXPLOSION);
    		}
    		else play_sound_effect(SFX_SPLASH);
    		shots[x_current][y_current] = 1;
    		sendMessage(SHOT, (char*) &player_target.coords); //shot is a uint8
    		update_text(status_txt_id, "PACKET LOSS PRESS START", -1,-1);
    		player_target.is_hidden = 1;
    		irqEnable(IRQ_KEYS); // ENABLE KEY INTERRUPT
    		*state = STATE_SENT_SHOT; // Go to sent shot state to wait for Acknowledge
    	}
    	else {
    		// Play error sound if there already is a shot at this location
    		play_sound_effect(SFX_ERROR);
    	}
		break;
    }
}

/*
 * Check if game is won.
 * Player wins if all enemy ships have been sunk i.e. all blocks have been hit
 */
bool game_won() {
    int i;
    for (i = 0; i < NUM_SHIPS; i++) {
        if (enemy_ships[i].hits < enemy_ships[i].len) {
            return false;
        }
    }
    return true;
}

/*
 * Check if game is lost.
 * Player loses if all their ships have been sunk i.e. all blocks have been hit
 */

bool game_lost() {
    int i;
    for (i = 0; i < NUM_SHIPS; i++) {
        if (player_ships[i].hits < player_ships[i].len) {
            return false;
        }
    }
    return true;
}

/*
 * Write ship coordinates to a buffer for sending via WiFi
 */

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

/*
 * Read buffer and parse received ship coordinates
 */

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

/*
 * Returns -1 on illegal shot received (most likely double receive due to wifi error)
 */
int updateShipHits(char coord) {
	int x,y,i,j;
	int isHit = 0;
	x = GET_X(coord);
	y = GET_Y(coord);

	if (enemy_shots[x][y] == 1) return -1;
	enemy_shots[x][y] = 1;

	for (i = 0; i < NUM_SHIPS; i++) {
		for (j = 0; j < player_ships[i].len; j++) {
			if (x == GET_X(player_ships[i].coords[j])
					&& y == GET_Y(player_ships[i].coords[j])) {
				player_ships[i].hits++;
				isHit = 1;
			}
		}
	}
	new_shot_tile(isHit, x, y, 0);

	if (isHit) play_sound_effect(SFX_EXPLOSION);
	else play_sound_effect(SFX_SPLASH);
	return 0;
}

/*
 * Transition from ship placement to next state
 */

void place_ships_transition(GameState *state, int ships_received) {
	char buff[MSG_SIZE];
	writeShipBuffer(buff);
	sendMessage(SHIPS, buff);
	play_sound_effect(SFX_LETS_DO_THIS);
	if (!ships_received) {
		load_backgrounds(WAIT);
		hide_player_ships();
		update_text(status_txt_id, "", -1,-1);
		*state = STATE_WAIT_FOR_ENEMY_PLACEMENT;
	} else {
		wait_placement_transition(state);
	}
}

/*
 * Transition from wait for enemy placement to next state
 */

void wait_placement_transition(GameState *state) {
	show_player_ships();
	load_backgrounds(GAME);
	initEnemyShips(recv_buffer + HEADER_LEN);

	if (!hosting) {
		update_text(status_txt_id, "ENEMYS TURN", -1,-1);
		*state = STATE_WAITING_FOR_TURN;
	} else {
		*state = STATE_TAKING_TURN;
		update_text(status_txt_id, "YOUR TURN", -1,-1);
	}
}

/*
 * Transition from waiting for enemy turn to next state
 */
void wait_turn_transition(GameState *state) {
	load_backgrounds(GAME);
	show_player_ships();
	check_win_transition(state, STATE_TAKING_TURN);
}

/*
 * Check win and transition to "stateIfNotOver" if the game is not over
 */

void check_win_transition(GameState *state, GameState stateIfNotOver) {
	// Check if the game has been won or lost
	if (game_won()) {
		increment_scores(1, count_shots(), count_hits());
		update_text(status_txt_id, "YOU WIN", -1, -1);
		play_sound_effect(SFX_GAME_OVER);
		*state = STATE_GAMEOVER;
		start_timer();
		
	} else if (game_lost()) {
		increment_scores(0, count_shots(), count_hits());
		update_text(status_txt_id, "YOU LOSE", -1, -1);
		play_sound_effect(SFX_GAME_OVER);
		*state = STATE_GAMEOVER;
		start_timer();
	} else {
		*state = stateIfNotOver;
	}
}

/*
 * Game FSM
 * Initial state is STATE_HOME.
 * Players choose if they host or join a game, then place their ships on the board.
 * Ships are sent to other player and then all game logic is done locally.
 * Shots are sent upon placement. In case of packet drop, the START key interrupt resends the shot message to other player.
 * A check is made every time a turn takes place to check for game over.
 *
 */

void update_state(GameState* state) {
	touchPosition touch;
	touchRead(&touch);
	u16 keys;

	switch (*state) {

	case STATE_HOME:
		scanKeys();
		keys = keysDown();
		// Handle "JOIN" touchscreen and button (A KEY)
		if (keys == KEY_A || (touch.px > JOIN_BUTTON_LEFT && touch.px < JOIN_BUTTON_RIGHT && touch.py < JOIN_BUTTON_BOTTOM && touch.py > JOIN_BUTTON_TOP)) {
			play_sound_effect(SFX_GUN);
			hosting = false;
			update_text(status_txt_id, "YOU ARE GUEST", -1,-1);
			*state = STATE_JOIN;
			load_backgrounds(WAIT);
		}
		// Handle "HOST" touchscreen button
		else if (keys == KEY_B || (touch.px > HOST_BUTTON_LEFT && touch.px < HOST_BUTTON_RIGHT && touch.py < HOST_BUTTON_BOTTOM && touch.py > HOST_BUTTON_TOP)){
			play_sound_effect(SFX_GUN);
			hosting = true;
			*state = STATE_HOST;
			update_text(status_txt_id, "YOU ARE HOST", -1,-1);
			load_backgrounds(WAIT);
		}
    	break;
    case STATE_HOST:
    	scanKeys();
    	keys = keysDown();
    	// Wait for other player to join
    	if(recvMessage(JOIN) > 0) {
			load_backgrounds(SHIP_PLACE);
			hide_scores();
			*state = STATE_PLACE_SHIPS;
			update_text(status_txt_id, "PLACE SHIPS", -1,-1);
			sendMessage(ACK,NULL);
    	}

    	break;
    case STATE_JOIN:
    	scanKeys();
    	keys = keysDown();
		sendMessage(JOIN, NULL);
    	if (recvMessage(ACK) > 0) {
			load_backgrounds(GAME);
			hide_scores();
			update_text(status_txt_id, "PLACE SHIPS", -1,-1);
			*state = STATE_PLACE_SHIPS;
		}
    	break;
    case STATE_PLACE_SHIPS:
		if (recvMessage(SHIPS)) {
			ships_received = 1;
		}
		place_ships();
		if (place_ship_count == NUM_SHIPS) {
			place_ships_transition(state, ships_received);
		}
    	break;
    case STATE_WAIT_FOR_ENEMY_PLACEMENT:
		if (!ships_received && recvMessage(SHIPS)) {
			wait_placement_transition(state);
		}
    	break;
    case STATE_WAITING_FOR_TURN:
    	if (recvMessage(SHOT)){
    		sendMessage(ACK,NULL); // Acknowledge that a shot has been received.
    		if (updateShipHits(recv_buffer[HEADER_LEN]) == 0){
    			wait_turn_transition(state);
    		}
    	}
    	break;
    case STATE_TAKING_TURN:
    	place_target(state);
        break;
    case STATE_SENT_SHOT:
    	if (recvMessage(ACK) > 0){
    		irqDisable(IRQ_KEYS);
    		update_text(status_txt_id, "ENEMYS TURN", -1,-1);
    		check_win_transition(state, STATE_WAITING_FOR_TURN);
    	}
    	break;
    case STATE_GAMEOVER:
		if (is_seconds(GAME_OVER_SCREEN_TIME)) {
			*state = STATE_HOME;
			display_scores();
			write_scores();
			hide_player_ships();
			clear_shots();
			init_game();
			update_text(status_txt_id, "", -1,-1);
			load_backgrounds(MAIN_MENU);
			player_target.is_hidden = 1;
		}
			
		break;
	
	}
}
