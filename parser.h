/*
    GROUP NUMBER - 27
    Shubham Gupta - 2021A7PS0468P
    Yash Sangram Barge - 2021A7PS0006P
    Venkatavihan Devaki - 2021A7PS0429P
    Priyank Shethia - 2021A7PS0657P
    Aditya Khandelwal - 2021A7PS2422P
*/


#pragma once

#include "others.h"

struct vector_int ***make_parse_table(struct grammar *g, struct set **first, struct set **follow);
void print_parse_table(struct grammar *g, struct vector_int ***parse_table);
void free_parse_table(struct vector_int ****p_parse_table, struct grammar *g);
struct tree_node *parse(char *file_name, struct grammar *g, struct symbol_table *st);
