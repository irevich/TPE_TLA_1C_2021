#ifndef _NODE_H_
#define _NODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

variable_node * create_variable_node(data_type variable_type, char * name);
void free_variable_node(variable_node * node);
constant_int_node * create_constant_int_node( int value);
void free_constant_int_node (constant_int_node * node);
constant_string_node * create_constant_string_node(char * value);
void free_constant_string_node(constant_string_node * string_node);
declaration_node * create_declaration_node(char * name, data_type type, node * assigned_node);
void free_declaration_node(declaration_node * node);
assignation_node * create_assignation_node(char * name, node * assigned_node);
void free_assignation_node(assignation_node * node);
exp_node * create_exp_node(char * op, node * left_node, node * right_node);
void free_exp_node(exp_node * node);
print_node * create_print_node(node * content_node);
void free_print_node(print_node * node);
node_list * add_node_list(node * node_header, node * node, node_type type);
node_list * create_node_list(node * node, node_type type);
void free_node_list(node_list * node);
void free_node(node * node);

rel_comp_node * create_relational_comp_node(char * op, node * left_node, node * right_node);
void free_relational_comp_node(rel_comp_node * node);
log_comp_node * create_logical_comp_node(char * op, node * left_node, node * right_node);
void free_logical_comp_node(log_comp_node * node);
if_node * create_if_node(node * cond, node * code);
void free_if_node(if_node * node);
if_otherwise_node * create_if_otherwise_node(node * cond, node * left, node * right);
void free_if_otherwise_node(if_otherwise_node * node);
while_node * create_while_node(node * cond, node * code);
void free_while_node(while_node * node);

property_node * create_property_node(char * var_name, figure_property_type property_type);
void free_property_node(property_node * node);

function_node * create_function_node(data_type return_type, char * name, node * node_param_header);
void free_function_node(function_node * node);

#endif