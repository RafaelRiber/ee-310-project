/*
 * storage.h
 *
 *  Created on: 20 Jan 2023
 *      Author: Rafael
 */

#ifndef STORAGE_H_
#define STORAGE_H_
#include "battleships.h"

#define FILENAME "battleshipScores"
#define SHOTS_TXT_X 62
#define SHOTS_TXT_Y 143

#define HITS_TXT_X 62
#define HITS_TXT_Y 155

#define WINS_TXT_X 62
#define WINS_TXT_Y 167

#include <stdio.h>
#include <fat.h>

typedef struct {
    int shotsCount;
    int hitsCount;
    int winsCount;
    uint8_t shots_txt_id;
    uint8_t hits_txt_id;
    uint8_t wins_txt_id;
} score;

void readScores();

void writeScores(score* myScore);

void displayScores();

void hideScore();


#endif /* STORAGE_H_ */
