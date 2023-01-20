/*
 * storage.c
 *
 *  Created on: 20 Jan 2023
 *      Author: Rafael
 */


#include "storage.h"
#include "text.h"


static const char *FILENAME_TXT = "/battleshipScores.txt";
score myScore;

void initScores(){
	myScore.shotsCount = 0;
	myScore.hitsCount = 0;
	myScore.winsCount = 0;
	myScore.shots_txt_id = new_text("A", SHOTS_TXT_X, SHOTS_TXT_Y, 1);
	myScore.hits_txt_id = new_text("A", HITS_TXT_X, HITS_TXT_Y, 1);
	myScore.wins_txt_id = new_text("A", WINS_TXT_X, WINS_TXT_Y, 1);
}

void readScores(){
//	//Open the file in read mode
//	FILE *file = fopen(FILENAME_TXT, "r");
//		if(file != NULL)
//		{
//			//Read the value and put it into the variable max_score
//			fscanf(file, "%d,%d,%d", &myScore.hitsCount, &myScore.shotsCount, &myScore.winsCount);
//			// close the file
//			fclose(file);
//		}
}

void writeScores(){
	FILE *file = fopen(FILENAME_TXT, "w+");
	fclose(file);
}

void displayScores(){

}

void hideScores(){

}
