#ifndef _NODE_H_
#define _NODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum{
    VARIABLE,
    CONSTANT_INT,
    DECLARATION,
    ASSIGNATION,
    EXP,
    PRINT_NODE,
    NODE_LIST
}node_type;

typedef enum{
    INT_TYPE = 0,
    STRING_TYPE = 1,
    CIRCLE_TYPE = 2,
    TRIANGLE_TYPE = 3,
    RECTANGLE_TYPE = 4,
    EMPTY = 5,
}data_type;

typedef enum{
    PLUS_TYPE = 0,
    MINUS_TYPE = 1,
    PRODUCT_TYPE = 2,
    DIVISION_TYPE = 3,
}aritmetic_op_type;

typedef enum{
    GT_TYPE = 0,
    GE_TYPE = 1,
    LT_TYPE = 2,
    LE_TYPE = 3,
    EQ_TYPE = 4,
    NEQ_TYPE = 5,
}relational_op_type;

typedef enum{
    AND_TYPE = 0,
    OR_TYPE = 1,
}logical_op_type;

typedef struct node{
    node_type type; 
}node;

typedef struct node_list{
    node_type type;
    struct node * node;
    struct node_list * next;
}node_list;

typedef struct variable_node{   //juani --> Listo
    node_type type;
    data_type variable_type;
    char * name;
}variable_node;

typedef struct constant_int_node{ //Igol ---> Listo
    node_type type;
    int value;
}constant_int_node;

typedef struct declaration_node{   //BETO  ---> Listo
    node_type type;
    char * name;
    data_type declaration_type;
    node * assigned_node;
}declaration_node;

typedef struct assignation_node{  //BETO --> Listo
    node_type type;
    char * name;
    node * assigned_node;
}assignation_node; 

typedef struct exp_node{ //Igol ---> Listo
    node_type type;
    char * op;
    node * left_node;
    node * right_node;
}exp_node;

typedef struct print_node{  //juani ---> Listo
    node_type type;
    node * content_node;   
}print_node;


variable_node * create_variable_node(data_type variable_type, char * name);
void free_variable_node(variable_node * node);
constant_int_node * create_constant_int_node( int value);
void free_constant_int_node (constant_int_node * node);
declaration_node * create_declaration_node(char * name, data_type type, node * assigned_node);
void free_declaration_node(declaration_node * node);
assignation_node * create_assignation_node(char * name, node * assigned_node);
void free_assignation_node(assignation_node * node);
exp_node * create_exp_node(char * op, node * left_node, node * right_node);
void free_exp_node(exp_node * node);
print_node * create_print_node(node * content_node);
void free_print_node(print_node * node);
node_list * add_node_list(node * node_header, node * node);
node_list * create_node_list(node * node);
void free_node_list(node_list * node);
void free_node(node * node);

#endif