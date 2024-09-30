/**
 * @title SheGang Linux Shell | Fuzzy Finding
 * @filename colors.c
 * @brief Module with fuzzy search function
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#include "fuzzy.h"

/**
 * @brief      Fuzzy search
 *
 * @param[in]  text         The text
 * @param[in]  query        The query
 * @param[in]  build_score  The build score
 * @param      score        The score
 * @param      score_len    The score length
 *
 * @return     { description_of_the_return_value }
 */
int fuzzy_search(const char *text, const char *query, bool build_score, int **score, size_t *score_len) {
	size_t total_score = 0;
	if (build_score) { // Build score is an optimization when searching through large database
		(*score) = (int*)malloc(sizeof(int) * strlen(text));
		memset(*score, 0, sizeof(int)*strlen(text));
		*score_len = strlen(text);
	}

	size_t first_character_boosts = 1;

	for (size_t t_idx = 0; t_idx < strlen(text); t_idx++) {
		char t = tolower(text[t_idx]); // NOTE(deter0): to lower performs kind of strangely probably due to UTF8
		for (size_t q_idx = 0; q_idx < strlen(query); q_idx++) {
			char q = tolower(query[q_idx]);

			if (t == q) {
			 // Start of word awards more but falls off fast
			if (t_idx == 0 || (t_idx > 0 && isspace(text[t_idx - 1]))) {
				int factor = 8/(first_character_boosts++);
				
				if (build_score) (*score)[t_idx] += factor;
				total_score += factor;
			} else { 
				if (build_score) (*score)[t_idx]++;
				total_score++;
			}
			
			size_t streak = 0;
			for (size_t s_idx = 1; s_idx < strlen(query)-q_idx; s_idx++) {
				char sq = tolower(query[q_idx + s_idx]);
				char st = tolower(text[t_idx + s_idx]);

				if (sq != st) break;
					streak++;
				
				// Beginning of string yields few points more; eg -> "Term" :: "Terminus", "Fluent Terminal"
				if (((float)t_idx / (float)strlen(text)) <= 0.35) {
					streak++;
				}

				int factor = streak*3/(strlen(query)*0.2);
				if (build_score)
					(*score)[t_idx + s_idx] += factor;
				total_score += factor;
			}

			// (N * (N + 1) ) /2
			// (*score)[t_idx] += (streak * (streak + 1)) / 2;
			t_idx += streak;
			}
		}
	}

	return total_score;
}
