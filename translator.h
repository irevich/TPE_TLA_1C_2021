#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//"Main" function to start translation
char * translate_to_c(node_list * first_node);

//Switch to call specific function based on node type
char * translate_node(node * node);

//Return string containing data type to print
char * get_data_type(data_type type);

//Specific translate functions
char * translate_declaration_node(declaration_node * dec_node);

//Prints our own defined functions so they can be called on the user program
void print_initial_functions();

char * translate_assignation_node(assignation_node * assign_node);

char * translate_constant_int_node(constant_int_node * node);

char * translate_constant_string_node(constant_string_node * node);

char * translate_variable_node(variable_node * node);

char * translate_exp_node ( exp_node * node);

char * translate_print_node(print_node * node);

char * translate_node_list(node_list * list_node);

char * translate_rel_comp_node (rel_comp_node * node);

char * translate_log_comp_node (log_comp_node * node);

char * translate_if_node (if_node * node);

char * translate_if_otherwise_node (if_otherwise_node * node);

char * translate_while_node (while_node * node);

char * translate_property_node (property_node * node);

char * translate_function_node (function_node * node);

char * translate_parentheses_exp_node(parentheses_exp_node * node);

char * translate_read_num_node(read_num_node * node);

#endif