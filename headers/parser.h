#pragma once

#include "first_follow_sets.h"

struct vector_int ***make_parse_table(struct grammar *g, struct set **first, struct set **follow);
void free_parse_table(struct vector_int ****p_parse_table, struct grammar *g);
