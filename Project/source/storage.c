// Battleships Storage Library
// EE-310 - Tharen Candi, Rafael Riber

#include "storage.h"
#include "text.h"

// Filename
static const char *FILENAME_TXT = "/battleshipScores.txt";

// Declare score structure
score myScore;

/*
 * Initializes the score struct with zeros, and creates text IDs
 */
void init_scores(){
	fatInitDefault();
	myScore.shotsCount = 0;
	myScore.hitsCount = 0;
	myScore.winsCount = 0;
	myScore.shots_txt_id = new_text("", SHOTS_TXT_X, SHOTS_TXT_Y, 1);
	myScore.hits_txt_id = new_text("", HITS_TXT_X, HITS_TXT_Y, 1);
	myScore.wins_txt_id = new_text("", WINS_TXT_X, WINS_TXT_Y, 1);
}

/*
 * Reads score from SD and updates score struct with read data
 */
void read_scores(){
//	//Open the file in read mode
	FILE *file = fopen(FILENAME_TXT, "r");
		if(file != NULL)
		{
			//Read the value and put it into the variable max_score
			fscanf(file, "%d,%d,%d", &myScore.shotsCount, &myScore.hitsCount, &myScore.winsCount);
			// close the file
			fclose(file);
		}
}

/*
 * Write score structure to file.
 */
void write_scores(){
	FILE *file = fopen(FILENAME_TXT, "w+");
		if(file != NULL)
		{
			//Print the values in the file
			fprintf(file, "%d,%d,%d", myScore.shotsCount, myScore.hitsCount, myScore.winsCount);
			//Close the file
			fclose(file);
		}
}

/*
 * Display scores on screen using text IDs
 */

void display_scores(){
	char shots_str[10], hits_str[10], wins_str[10];
	snprintf(shots_str, sizeof(shots_str), "Shots %d", myScore.shotsCount);
	snprintf(hits_str, sizeof(hits_str), "Hits  %d", myScore.hitsCount);
	snprintf(wins_str, sizeof(wins_str), "Wins  %d", myScore.winsCount);
	update_text(myScore.shots_txt_id, shots_str, -1, -1);
	update_text(myScore.hits_txt_id, hits_str, -1, -1);
	update_text(myScore.wins_txt_id, wins_str, -1, -1);

}


/*
 * Update score text to hide score
 */
void hide_scores() {
	update_text(myScore.shots_txt_id, "", -1, -1);
	update_text(myScore.hits_txt_id, "", -1, -1);
	update_text(myScore.wins_txt_id, "", -1, -1);
}

/*
 * Increment score struct with finished game stats
 */
void increment_scores(int wonGame, int shotsMade, int hitsMade){
	myScore.winsCount = myScore.winsCount + wonGame;
	myScore.shotsCount = myScore.shotsCount + shotsMade;
	myScore.hitsCount = myScore.hitsCount + hitsMade;
}
