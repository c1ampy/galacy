#ifndef HIGH_SCORE_SAVE_LOAD_H
#define HIGH_SCORE_SAVE_LOAD_H

#define DIFFICULTY_COUNT 3
#define HIGH_SCORE_FILE "high_score.dat"

void high_score_save(const int high_scores[DIFFICULTY_COUNT]);
void high_score_load(int high_scores[DIFFICULTY_COUNT]);

#endif /* HIGH_SCORE_SAVE_LOAD_H */
