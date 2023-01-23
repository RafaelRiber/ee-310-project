#ifndef TEXT_AND_SHOTS_H
#define TEXT_AND_SHOTS_H
#include <stdint.h>

#define SHOTS_LEN 512
#define HIT_PAL_IDX 253
#define MISS_PAL_IDX 254
#define SHOT_LEN 256
#define SHOTS_PAL_LEN 2
extern uint8_t shotsTiles[SHOTS_LEN];

#define TEXT_LEN 2368
#define TEXT_PAL_IDX 255
#define TEXT_PAL_LEN 1
#define CHAR_LEN 64
extern uint8_t textTiles[TEXT_LEN];
#endif