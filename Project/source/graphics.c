#include <nds.h>
#include "graphics.h"
#include "stdio.h"

#include "test_gameboard.h"

#include "battleship_title.h"
#include "battleship_title_sub.h"
#include "hosting_waiting_sub.h"

#include "text_and_shots.h"
#include "spritemap.h"
#include "battleships.h"
#include "text.h"
#define GB_BG 0
#define SHOT_BG 1
#define TEXT_BG 2


#define GB_BG_MAP_BASE 0 // [0, 2)
#define TEXT_MAP_BASE 1  // [2, 4)
#define SHOT_MAP_BASE 2 // [6, 8)
#define GB_BG_TILE_BASE 1 // [16, 32)
#define TEXT_TILE_BASE 4 
#define SHOT_TILE_BASE 7 

#define GAMEBOARD_PRIORITY 2
#define SPRITE_PRIORITY 1
#define SHOT_PRIORITY 0
#define TEXT_PRIORITY 1


#define HIT_IDX 0 
#define MISS_IDX 4
#define BLANK_IDX 8

#define BG0_RESERVED_PAL_SPACE sizeof(u16)*100



#define NUM_SHIP_SPRITES (CARRIER_SIZE * NUM_SHIPS)
#define SHIP_SPRITE_IDX(i, j) (i*CARRIER_SIZE + j)
#define HIT_MISS_SPRITE_IDX(i) (1 + NUM_SHIP_SPRITES + i)
#define TARGET_SPRITE_IDX(i) (i)
#define MAP_DIM 32
#define MAP_IDX(y,x) ((y)*MAP_DIM + (x))

#define TARGET_SPRITE_OFFSET 19
#define MISS_SPRITE_OFFSET 18
#define HIT_SPRITE_OFFSET 17

uint8 num_hits_misses; 
uint8 num_ship_sprites;

// gfx's for hits and misses.
u16 * hit_sprite_main;
u16 * hit_sprite_sub;
u16* miss_sprite_main;
u16* miss_sprite_sub;

/*
main engine: mode 0 
    - bg0: gameboard
    - bgx: effects (explosions..)
    - Sprites: ships
*/

struct background {
	const unsigned int * tiles;
	int tilesLen;
	const unsigned short * pal;
	int palLen;
	const unsigned short * map;
	int mapLen;
};


int gameboard_pal_usage_main = 0;
int gameboard_pal_usage_sub = 0;
const struct background main_backgrounds[NUM_SCREENS] = {
	[MAIN_MENU] = {
		battleship_titleTiles,
		battleship_titleTilesLen,
		battleship_titlePal,
		battleship_titlePalLen,
		battleship_titleMap,
		battleship_titleMapLen
	},
	[WAIT] = {
	hosting_waiting_subTiles,
	hosting_waiting_subTilesLen,
	hosting_waiting_subPal,
	hosting_waiting_subPalLen,
	hosting_waiting_subMap,
	hosting_waiting_subMapLen
	},

	[SHIP_PLACE] = {
		test_gameboardTiles,
		test_gameboardTilesLen,
		test_gameboardPal,
		test_gameboardPalLen,
		test_gameboardMap,
		test_gameboardMapLen
	},
	[GAME] = {
		test_gameboardTiles,
		test_gameboardTilesLen,
		test_gameboardPal,
		test_gameboardPalLen,
		test_gameboardMap,
		test_gameboardMapLen
	},
};

const struct background sub_backgrounds[NUM_SCREENS] = {
	[MAIN_MENU] = {
		battleship_title_subTiles,
		battleship_title_subTilesLen,
		battleship_title_subPal,
		battleship_title_subPalLen,
		battleship_title_subMap,
		battleship_title_subMapLen
	},

	[WAIT] = {
	hosting_waiting_subTiles,
	hosting_waiting_subTilesLen,
	hosting_waiting_subPal,
	hosting_waiting_subPalLen,
	hosting_waiting_subMap,
	hosting_waiting_subMapLen
	},

	[SHIP_PLACE] = {
		test_gameboardTiles,
		test_gameboardTilesLen,
		test_gameboardPal,
		test_gameboardPalLen,
		test_gameboardMap,
		test_gameboardMapLen
	},
	[GAME] = {
		test_gameboardTiles,
		test_gameboardTilesLen,
		test_gameboardPal,
		test_gameboardPalLen,
		test_gameboardMap,
		test_gameboardMapLen
	},
};



/*
	grit palette len is max palette len. Not used palette.
	get actual used palette size. 
	returns atleast 1.
*/
int get_pal_size(const unsigned short * pal, int max_size) {
	
	int bytes = 0;
	int i;
	for (i = 0; i< max_size / sizeof(short); i ++) {
		if (pal[i] != (unsigned short) 0)
			bytes += sizeof(short);
		else if (i != 0)
			break;
	}

	return bytes;
}


void configure_graphics() {
    //MAIN engine
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE;
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

	//SUB Engine
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE;
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	// //BG0 (bitmaps) configuration
	BGCTRL_SUB[GB_BG] = BG_COLOR_256 | BG_MAP_BASE(GB_BG_MAP_BASE) | BG_TILE_BASE(GB_BG_TILE_BASE) | BG_32x32 | BG_PRIORITY(GAMEBOARD_PRIORITY);
	BGCTRL[GB_BG] = BG_COLOR_256 | BG_MAP_BASE(GB_BG_MAP_BASE) | BG_TILE_BASE(GB_BG_TILE_BASE) | BG_32x32 | BG_PRIORITY(GAMEBOARD_PRIORITY);
	
	//BG1 (shots - hits and misses) configuration
	BGCTRL_SUB[SHOT_BG] = BG_COLOR_256 | BG_MAP_BASE(SHOT_MAP_BASE) | BG_TILE_BASE(SHOT_TILE_BASE) | BG_32x32 | BG_PRIORITY(SHOT_PRIORITY);
	BGCTRL[SHOT_BG] = BG_COLOR_256 | BG_MAP_BASE(SHOT_MAP_BASE) | BG_TILE_BASE(SHOT_TILE_BASE) | BG_32x32 | BG_PRIORITY(SHOT_PRIORITY);
	
	//bg2 (text) configuration
	BGCTRL[TEXT_BG] = BG_COLOR_256 | BG_MAP_BASE(TEXT_MAP_BASE) | BG_TILE_BASE(TEXT_TILE_BASE) | BG_32x32 | BG_PRIORITY(TEXT_PRIORITY);
	BGCTRL_SUB[TEXT_BG] = BG_COLOR_256 | BG_MAP_BASE(TEXT_MAP_BASE) | BG_TILE_BASE(TEXT_TILE_BASE) | BG_32x32 | BG_PRIORITY(TEXT_PRIORITY);
	
	
	// palette filling
	int i;
	for (i = 0; i < NUM_SCREENS; i++) {
		int bg_pal_main = get_pal_size(main_backgrounds[i].pal, main_backgrounds[i].palLen);
		if (bg_pal_main > gameboard_pal_usage_main)
			gameboard_pal_usage_main = bg_pal_main;

		int bg_pal_sub = get_pal_size(sub_backgrounds[i].pal, sub_backgrounds[i].palLen);
		if (bg_pal_sub > gameboard_pal_usage_sub)
			gameboard_pal_usage_sub = bg_pal_sub;
	}

	dmaCopy(shotsTiles, BG_TILE_RAM(SHOT_TILE_BASE), SHOTS_LEN);
	dmaCopy(shotsTiles, BG_TILE_RAM_SUB(SHOT_TILE_BASE), SHOTS_LEN);
	

	//fill main pal with shot pal	manually.
	BG_PALETTE[HIT_PAL_IDX] = ARGB16(1,31, 0, 0);
	BG_PALETTE[MISS_PAL_IDX] = ARGB16(1,0, 0, 31);
	
	BG_PALETTE_SUB[HIT_PAL_IDX] = ARGB16(1,31, 0, 0);
	BG_PALETTE_SUB[MISS_PAL_IDX] = ARGB16(1,0, 0, 31);

	clear_shots();
	
	init_text_api(BG_MAP_RAM(TEXT_MAP_BASE), (u8*)BG_TILE_RAM(TEXT_TILE_BASE), BG_MAP_RAM_SUB(TEXT_MAP_BASE), (u8*)BG_TILE_RAM_SUB(TEXT_TILE_BASE));

	// ###############################################################################################################################


    //sprites
   
    //MAIN: Set up memory bank to work in sprite mode (offset since we are using VRAM A for backgrounds)
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_SPRITE_0x06400000;
	//SUB:
	VRAM_D_CR = VRAM_ENABLE | VRAM_D_SUB_SPRITE;

	//Initialize sprite manager and the engine
	oamInit(&oamMain, SpriteMapping_1D_32, false);
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	//target, miss, hit allocaiton
	allocate_sprite(&player_target.sprite_buff, spritemapTiles + TARGET_SPRITE_OFFSET*64, 256, 1);
	
	// allocate_sprite(&hit_sprite_main, spritemapTiles + 64*HIT_SPRITE_OFFSET, 256, 1);
	// allocate_sprite(&hit_sprite_sub, spritemapTiles + 64*HIT_SPRITE_OFFSET, 256, 0);
	// allocate_sprite(&miss_sprite_main, spritemapTiles + 64*MISS_SPRITE_OFFSET, 256, 1);
	// allocate_sprite(&miss_sprite_sub, spritemapTiles + 64*MISS_SPRITE_OFFSET, 256, 0);

	oamRotateScale(&oamMain, 0, degreesToAngle(-90), intToFixed(1, 8), intToFixed(1, 8));
	oamRotateScale(&oamSub, 0, degreesToAngle(-90), intToFixed(1, 8), intToFixed(1, 8));
	
	
	dmaCopy(spritemapPal, (u16*)SPRITE_PALETTE, spritemapPalLen);
	dmaCopy(spritemapPal, (u16*)SPRITE_PALETTE_SUB, spritemapPalLen);
	int scount = 0;
	//Allocate all ship sprites.
	int j; 
	for (i = 0; i < NUM_SHIPS; i++) {
		ship * p_ship = player_ships + i;
		for (j = 0; j < p_ship->len; j ++) {
			allocate_sprite( 
				p_ship->sprite_buffs + j, 
				spritemapTiles + 64*scount,256,0
			);
			scount++;
		}
	}


	num_hits_misses = 0;


	// Init. text
	status_txt_id = new_text("", 48,0,0);
}

void load_backgrounds(int screen) {
	if (screen > NUM_SCREENS || screen < 0)
		return;


	//main
	if ( screen != WAIT) {
		dmaCopy(main_backgrounds[screen].tiles, BG_TILE_RAM(GB_BG_TILE_BASE), main_backgrounds[screen].tilesLen);
		dmaCopy(main_backgrounds[screen].pal, BG_PALETTE, gameboard_pal_usage_main);
		dmaCopy(main_backgrounds[screen].map, BG_MAP_RAM(GB_BG_MAP_BASE), main_backgrounds[screen].mapLen);
	}

	//sub
	swiCopy(sub_backgrounds[screen].tiles, BG_TILE_RAM_SUB(GB_BG_TILE_BASE), sub_backgrounds[screen].tilesLen/2);
	swiCopy(sub_backgrounds[screen].pal, BG_PALETTE_SUB, gameboard_pal_usage_sub/sizeof(u16));
	swiCopy(sub_backgrounds[screen].map, BG_MAP_RAM_SUB(GB_BG_MAP_BASE),sub_backgrounds[screen].mapLen/2);
	
	
}

// put a hit or miss on the enemey board.
// x and y are ga,eboard cordinates 0-9
void new_shot_sprite(int isHit, int x, int y, int is_main){

	u16 * map = (u16*) BG_MAP_RAM(SHOT_MAP_BASE);
	if (!is_main)
		map = (u16*) BG_MAP_RAM_SUB(SHOT_MAP_BASE);

	u16 idx = HIT_IDX;
	if (!isHit)
		idx = MISS_IDX;
	
	// translate x and y into screen space tile coordinates.
	x = 6+ (x*2);
	y = 2+ (y*2); 
	

	map[MAP_IDX(y, x)] = idx;
	map[MAP_IDX(y, x+1)] = idx+1;
	map[MAP_IDX(y+1, x)] = idx+2;
	map[MAP_IDX(y+1, x+1)] = idx+3;

}

void clear_shots() {
	u16 * map_main = (u16*) BG_MAP_RAM(SHOT_MAP_BASE);
	u16*map_sub = (u16*) BG_MAP_RAM_SUB(SHOT_MAP_BASE);
	int i;
	for (i = 0; i < MAP_DIM*MAP_DIM;i ++) {
		map_main[i] = (u16) BLANK_IDX;
		map_sub[i]= (u16) BLANK_IDX;
	} 
	
}
//todo: merge all sprites palettes (from grit) to one sprite palette.
uint16_t sprite_palette_memory_usage = 0;
void allocate_sprite(u16 ** gfx, const unsigned int * tiles, unsigned int tilesLen, char isMain ) {

	OamState * oam = &oamMain;

	if (!isMain)
		oam = &oamSub;
	
		

	*gfx = oamAllocateGfx(oam, SpriteSize_16x16, SpriteColorFormat_256Color);
	printf("tile: %p\n",*gfx );
	if (*gfx == NULL) {
	
		return;
	}
	
	//memory_usage += (SpriteSize_16x16 & 0xFFF) << 5;
	//Copy data for the graphic (palette and bitmap)

	
	dmaCopy(tiles, *gfx, tilesLen);

	//sprite_palette_memory_usage += palLen;
	
}

void update_ships() {
	int i,j;


	//MAIN ENGINE: TARGET
	oamSet(&oamMain, 
		TARGET_SPRITE_IDX(0), 
		(GET_X(player_target.coords) * 16) + 48,
		(GET_Y(player_target.coords) * 16) + 16,
		0,
		0, 
		SpriteSize_16x16,			// Sprite size
		SpriteColorFormat_256Color,	// Color format
		player_target.sprite_buff,			// Loaded graphic to display
		-1,				// Affine rotation to use (-1 none)
		false,			// Double size if rotating
		player_target.is_hidden,			// Hide this sprite
		false, false,	// Horizontal or vertical flip
		false			// Mosaic
	);
	
	int is_horizontal;
	//SUB ENGINE: PLAYER SHIPS

	for (i =0; i< NUM_SHIPS; i++) {
		
		ship *p_ship = player_ships + i;
		if (GET_X(p_ship->coords[0]) < GET_X(p_ship->coords[1]))
			is_horizontal = -1;
		else
			is_horizontal = 0;

		//printf("pp %p\n",*(e_ship.sprite_buffs + j));
		for(j = 0; j < p_ship->len; j ++) {
			int x = (GET_X(p_ship->coords[j]) * 16) + 48;
			int y = (GET_Y(p_ship->coords[j]) * 16) + 16;
	
			oamSet(&oamSub, 	// oam handler
				SHIP_SPRITE_IDX(i,j),				// Number of sprite
				x, y,			// Coordinates
				1,				// Priority
				0,				// Palette to use
				SpriteSize_16x16,			// Sprite size
				SpriteColorFormat_256Color,	// Color format
				*(p_ship->sprite_buffs + j),			// Loaded graphic to display
				is_horizontal,				// Affine rotation to use (-1 none)
				false,			// Double size if rotating
				p_ship->is_hidden,			// Hide this sprite
				false, false,	// Horizontal or vertical flip
				false			// Mosaic
    		);
			

			
		}


		
	}
   	
	
}

void hide_player_ships(){
	int i;
	for (i = 0; i<NUM_SHIPS; i++) player_ships[i].is_hidden = 1;
}

void show_player_ships(){
	int i;
	for (i = 0; i<NUM_SHIPS; i++) player_ships[i].is_hidden = 0;
}
