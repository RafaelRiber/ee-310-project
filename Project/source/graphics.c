#include <nds.h>
#include "graphics.h"
#include "stdio.h"

#include "test_gameboard.h"

#include "battleship_title.h"
#include "battleship_title_sub.h"
#include "hosting_waiting_sub.h"

#include "spritemap.h"
#include "battleships.h"
#define GB_BG 0
#define GB_BG_MP_BASE 0 
#define GB_BG_TILE_BASE 1



#define NUM_SHIP_SPRITES (CARRIER_SIZE * NUM_SHIPS)
#define SHIP_SPRITE_IDX(i, j) (i*CARRIER_SIZE + j)
#define HIT_MISS_SPRITE_IDX(i) (1 + NUM_SHIP_SPRITES + i)
#define TARGET_SPRITE_IDX(i) (i)

#define TARGET_SPRITE_OFFSET 19
#define MISS_SPRITE_OFFSET 18
#define HIT_SPRITE_OFFSET 17

uint8 num_hits_misses; 
uint8 num_ship_sprites;
// gfx's for hits and misses.
u16 * hits_misses_sprites[BRD_LEN*BRD_LEN];
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

struct background main_backgrounds[NUM_SCREENS] = {
	[MAIN_MENU] = {
	battleship_titleTiles,
	battleship_titleTilesLen,
	battleship_titlePal,
	battleship_titlePalLen,
	battleship_titleMap,
	battleship_titleMapLen
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

struct background sub_backgrounds[NUM_SCREENS] = {
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






void configure_graphics() {
    //MAIN engine
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

	//SUB Engine
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	BGCTRL_SUB[GB_BG] = BG_COLOR_256 | BG_MAP_BASE(GB_BG_MP_BASE) | BG_TILE_BASE(GB_BG_TILE_BASE) | BG_32x32;

	//BG0 configuration for the background
	BGCTRL[GB_BG] = BG_COLOR_256 | BG_MAP_BASE(GB_BG_MP_BASE) | BG_TILE_BASE(GB_BG_TILE_BASE) | BG_32x32;
	
    //sprites
    //Set up memory bank to work in sprite mode (offset since we are using VRAM A for backgrounds)
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_SPRITE_0x06400000;
	//Initialize sprite manager and the engine
	oamInit(&oamMain, SpriteMapping_1D_32, false);


	//SUB ENGINE

	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	//BG0 configuration for the background
	BGCTRL_SUB[GB_BG] = BG_COLOR_256 | BG_MAP_BASE(GB_BG_MP_BASE) | BG_TILE_BASE(GB_BG_TILE_BASE) | BG_32x32;
	
	//sprites
    //Set up memory bank to work in sprite mode
	VRAM_D_CR = VRAM_ENABLE | VRAM_D_SUB_SPRITE;
	//Initialize sprite manager and the engine
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	//target sprite
	allocate_sprite(&player_target.sprite_buff, spritemapTiles + TARGET_SPRITE_OFFSET*64, 256, 1);
	
	
	oamRotateScale(&oamMain, 0, degreesToAngle(-90), intToFixed(1, 8), intToFixed(1, 8));
	oamRotateScale(&oamSub, 0, degreesToAngle(-90), intToFixed(1, 8), intToFixed(1, 8));
	
	
	dmaCopy(spritemapPal, (u16*)SPRITE_PALETTE, spritemapPalLen);
	dmaCopy(spritemapPal, (u16*)SPRITE_PALETTE_SUB, spritemapPalLen);
	int scount = 0;
	//Allocate all ship sprites.
	int i,j; 
	for (i = 0; i < NUM_SHIPS; i++) {
		ship * p_ship = player_ships + i;
		//ship p_ship = player_ships[i];
		for (j = 0; j < p_ship->len; j ++) {
				
			allocate_sprite( 
				p_ship->sprite_buffs + j, 
				spritemapTiles + 64*scount,256,0
			);
			scount++;
			
		}
	}

	
	num_hits_misses = 0;
	memset(hits_misses_sprites, 0, sizeof(u16 *) * BRD_LEN * BRD_LEN);
}

void load_backgrounds(int screen) {
	if (screen > NUM_SCREENS || screen < 0)
		return;

	if (screen != WAIT) {
		swiCopy(main_backgrounds[screen].tiles, BG_TILE_RAM(GB_BG_TILE_BASE), main_backgrounds[screen].tilesLen/2);
		swiCopy(main_backgrounds[screen].pal, BG_PALETTE, main_backgrounds[screen].palLen/2);
		swiCopy(main_backgrounds[screen].map, BG_MAP_RAM(GB_BG_MP_BASE), main_backgrounds[screen].mapLen/2);
	}
		
	swiCopy(sub_backgrounds[screen].tiles, BG_TILE_RAM_SUB(GB_BG_TILE_BASE), sub_backgrounds[screen].tilesLen/2);
	swiCopy(sub_backgrounds[screen].pal, BG_PALETTE_SUB, sub_backgrounds[screen].palLen/2);
	swiCopy(sub_backgrounds[screen].map, BG_MAP_RAM_SUB(GB_BG_MP_BASE),sub_backgrounds[screen].mapLen/2);
}

// put a hit or miss on the enemey board.
// (MAIN ENGINE)
void new_shot_sprite(int isHit, int x, int y){
	OamState * oam = &oamMain;
	u16 * gfx = oamAllocateGfx(oam, SpriteSize_16x16, SpriteColorFormat_256Color);
	num_hits_misses += 1;
	hits_misses_sprites[num_hits_misses] = gfx;
	if (isHit) {
		dmaCopy(spritemapTiles+64*HIT_SPRITE_OFFSET, gfx,256);
		
	}
	else {
		dmaCopy(spritemapTiles+64*MISS_SPRITE_OFFSET, gfx,256);
	}

	int sx = (x * 16) + 48;
	int sy = (y * 16) + 16;

	oamSet(&oamMain, 	// oam handler
		HIT_MISS_SPRITE_IDX(num_hits_misses),				// Number of sprite
		sx, sy,			// Coordinates
		0,				// Priority
		0,				// Palette to use
		SpriteSize_16x16,			// Sprite size
		SpriteColorFormat_256Color,	// Color format
		gfx,			// Loaded graphic to display
		-1,				// Affine rotation to use (-1 none)
		false,			// Double size if rotating
		false,			// Hide this sprite
		false, false,	// Horizontal or vertical flip
		false			// Mosaic
	);
	

}

void clear_shots() {
	oamClear (&oamMain, num_ship_sprites, num_ship_sprites + num_hits_misses);
}
//todo: merge all sprites palletes (from grit) to one sprite pallete.
uint16_t sprite_pallete_memory_usage = 0;
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

	//sprite_pallete_memory_usage += palLen;
	
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
				0,				// Priority
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
