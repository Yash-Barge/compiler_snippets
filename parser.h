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


/**
 * @brief Generate the Parse Table
 * 
 * @param g The grammar rules
 * @param first The First set
 * @param follow The follow Set
 * @return struct vector_int*** The Parse table 
 */
struct vector_int ***createParseTable(struct grammar *g, struct set **first, struct set **follow);



/**
 * @brief Used to print the parse table
 * 
 * @param g The grammar
 * @param parse_table The parse table
 */
void print_parse_table(struct grammar *g, struct vector_int ***parse_table);


/**
 * @brief Used to free the memory allocated to the parse table
 * 
 * @param p_parse_table Pointer to the pointer to the memory allocated to the parse table
 * @param g The grammar
 */
void free_parse_table(struct vector_int ****p_parse_table, struct grammar *g);


/**
 * @brief Used to parse in the file
 * 
 * @param file_name Path to the file
 * @param g The grammar
 * @param st The symbol table
 * @return struct tree_node* The parse tree generated from parsing the file
 */
struct tree_node *parse(char *file_name, struct grammar *g, struct symbol_table *st);
