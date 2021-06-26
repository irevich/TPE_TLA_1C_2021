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
    PROPERTY_NODE,
    NODE_LIST,
    PARAM_NODE_LIST,
    LOG_COMP,
    REL_COMP,
    IF_NODE,
    IF_OTHERWISE,
    WHILE_NODE,
    FUNCTION_NODE,
}node_type;

typedef enum{
    EMPTY = 0,
    INT_TYPE,
    STRING_TYPE,
    CIRCLE_TYPE,
    TRIANGLE_TYPE,
    RECTANGLE_TYPE,
    FIGURE_TYPE,
}data_type;

typedef enum{
    RADIUS_TYPE,
    SIDE_1_TYPE,
    SIDE_2_TYPE,
    SIDE_3_TYPE,
    HEIGHT_TYPE,
    BASE_TYPE,
}figure_property_type;

typedef enum{
    PLUS_TYPE = 0,
    MINUS_TYPE,
    PRODUCT_TYPE,
    DIVISION_TYPE,
}aritmetic_op_type;

typedef enum{
    GT_TYPE,
    GE_TYPE,
    LT_TYPE,
    LE_TYPE,
    EQ_TYPE,
    NEQ_TYPE,
}relational_op_type;

typedef enum{
    AND_TYPE = 0,
    OR_TYPE,
}logical_op_type;

typedef struct node{
    node_type type; 
}node;

typedef struct node_list{
    node_type type; //it could be node_param_list or node_list, it depends where is used
    struct node * node;
    struct node_list * next;
}node_list;

typedef struct property_node{
    node_type type;
    node * var_node;
    figure_property_type property_type;
    data_type return_type;
}property_node;

typedef struct variable_node{ 
    node_type type;
    data_type variable_type;
    char * name;
}variable_node;

typedef struct function_node{
    node_type type;
    data_type return_type;
    char * name;
    node * node_param_list;
}function_node;

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
    node * var_node;
    node * assigned_node;
}assignation_node; 

typedef struct exp_node{ 
    node_type type;
    char * op;
    int has_parentheses;
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