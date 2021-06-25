#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_

#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * translate_to_c(node_list * first_node);

char * translate_node(node * node);

char * get_data_type(data_type type);

char * translate_declaration_node(declaration_node * dec_node);

char * translate_assignation_node(assignation_node * assign_node);

char * translate_constant_int_node(constant_int_node * node);

char * translate_variable_node(variable_node * node);

char * translate_exp_node ( exp_node * node);

char * translate_print_node(print_node * node);

char * translate_node_list(node_list * list_node);

char * translate_rel_comp_node (rel_comp_node * node);

char * translate_log_comp_node (log_comp_node * node);

char * translate_if_node (if_node * node);

char * translate_if_otherwise_node (if_otherwise_node * node);

char * translate_while_node (while_node * node);

#endif