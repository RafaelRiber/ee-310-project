#ifndef TEXT_H
#define TEXT_H

#include <nds.h>

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define MAX_TEXTS 10


#define TEXT_HEIGHT 8
#define TEXT_WIDTH 8
typedef enum text_errors {NONE, MEM_ERR, INDEX_OUT_OF_BOUNDS, INVALID_PARAM } text_errors;


// initialise text api with tile mode setup map and tile pointers for main and sub engines.
// palette offsets should first available pal index, assumes all following are available. 
int init_text_api(u16 * map_main, u16* tile_main, u16 *map_sub, u16* tile_sub, int main_pal_off, int sub_pal_off);

/*
    adds new text obj to datastructure and returns its id.
    returns -text_error if error.
*/
int new_text(char * string, int x, int y, int is_main);

/*
    changes the text displayed for text object "id" and its position.
    give negative position if want same position.

    returns 0 on success;
*/
int update_text(int id, char * string, int x, int y);



#endif