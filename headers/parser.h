#pragma once

#include "tree.h"
#include "first_follow_sets.h"

struct vector_int ***make_parse_table(struct grammar *g, struct set **first, struct set **follow);
void print_parse_table(struct grammar *g, struct vector_int ***parse_table);
void free_parse_table(struct vector_int ****p_parse_table, struct grammar *g);
struct tree_node *parse(char *file_name, struct grammar *g);
