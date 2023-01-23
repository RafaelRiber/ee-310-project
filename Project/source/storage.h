// Battleships Storage Library
// EE-310 - Tharen Candi, Rafael Riber

#ifndef STORAGE_H_
#define STORAGE_H_
#include "battleships.h"

#define FILENAME "battleshipScores"
#define SHOTS_TXT_X 8
#define SHOTS_TXT_Y 144

#define HITS_TXT_X 8
#define HITS_TXT_Y 152

#define WINS_TXT_X 8
#define WINS_TXT_Y 160

#include <stdio.h>
#include "fat.h"

typedef struct {
    int shotsCount;
    int hitsCount;
    int winsCount;
    uint8_t shots_txt_id;
    uint8_t hits_txt_id;
    uint8_t wins_txt_id;
} score;

void init_scores();

void read_scores();

void write_scores();

void display_scores();

void hide_scores();

void increment_scores(int wonGame, int shotsMade, int hitsMade);


#endif /* STORAGE_H_ */
