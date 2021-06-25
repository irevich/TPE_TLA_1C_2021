#ifndef _NODE_DEFINITIONS_H_
#define _NODE_DEFINITIONS_H_


typedef enum{
    VARIABLE,
    CONSTANT_INT,
    CONSTANT_STRING,
    DECLARATION,
    ASSIGNATION,
    EXP,
    PRINT_NODE,
    NODE_LIST,
    LOG_COMP,
    REL_COMP,
    IF_NODE,
    IF_OTHERWISE,
    WHILE_NODE,
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

typedef struct variable_node{ 
    node_type type;
    data_type variable_type;
    char * name;
}variable_node;

typedef struct constant_int_node{ 
    node_type type;
    int value;
}constant_int_node;

typedef struct constant_string_node{
    node_type type;
    char * value;
}constant_string_node;

typedef struct declaration_node{ 
    node_type type;
    char * name;
    data_type declaration_type;
    node * assigned_node;
}declaration_node;

typedef struct assignation_node{
    node_type type;
    char * name;
    node * assigned_node;
}assignation_node; 

typedef struct exp_node{ 
    node_type type;
    char * op;
    node * left_node;
    node * right_node;
}exp_node;

typedef struct print_node{ 
    node_type type;
    node * content_node;   
}print_node;

typedef struct rel_comp_node{
    node_type type;
    char * op;
    node * left_node;
    node * right_node;
}rel_comp_node;

typedef struct log_comp_node{
    node_type type;
    char * op;
    node * left_node;
    node * right_node; 
}log_comp_node;

typedef struct if_node{
    node_type type;
    node * cond;
    node * code; 
}if_node;

typedef struct if_otherwise_node{
    node_type type;
    node * cond;
    node * left_node;
    node * right_node; 
}if_otherwise_node;

typedef struct while_node{
    node_type type;
    node * cond;
    node * code; 
}while_node;

#endif