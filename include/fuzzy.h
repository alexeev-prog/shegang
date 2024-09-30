#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
                                                                        /*          Optional         */
int fuzzy_search(const char *text, const char *query, bool build_score, int **score, size_t *score_len);


