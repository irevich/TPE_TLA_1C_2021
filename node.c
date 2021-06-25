#include "node.h"


variable_node * create_variable_node(data_type variable_type, char * name){
    variable_node * new_node = malloc(sizeof(variable_node));
    new_node->type = VARIABLE;
    new_node->variable_type = variable_type;
    new_node->name = malloc(strlen(name)+1);
    strcpy(new_node->name, name);
    return new_node;
}

void free_variable_node(variable_node * node){
    free(node->name);
    free(node);
}

constant_int_node * create_constant_int_node( int value){
    //printf("Creando el node constant\n");
    constant_int_node * new_node = malloc(sizeof(constant_int_node));
    new_node->type = CONSTANT_INT;
    new_node->value = value;
    return new_node;

}

void free_constant_int_node (constant_int_node * node){
    free(node);
}


declaration_node * create_declaration_node(char * name, data_type type, node * assigned_node){
    //printf("Creando el node declaration\n");
    declaration_node * new_node = malloc(sizeof(declaration_node));
    new_node->type = DECLARATION;
    new_node->declaration_type = type;
    new_node->name = malloc(strlen(name) + 1);
    strcpy(new_node->name, name);
    new_node->assigned_node = assigned_node;
    return new_node;
    
}

void free_declaration_node(declaration_node * node){
    free(node->name);
    free_node(node->assigned_node);
    free(node);
}

assignation_node * create_assignation_node(char * name, node * assigned_node){
    assignation_node * new_node = malloc(sizeof(assignation_node));
    new_node->name = malloc(strlen(name) + 1);
    strcpy(new_node->name, name);
    new_node->type = ASSIGNATION;
    new_node->assigned_node =  assigned_node;
    return new_node;
}

void free_assignation_node(assignation_node * node){
    free(node->name);
    free_node(node->assigned_node);
    free(node);
}

exp_node * create_exp_node(char * op, node * left_node, node * right_node){
    exp_node * new_node = malloc(sizeof(exp_node));
    new_node->type = EXP;
    new_node->op = malloc(strlen(op) + 1);
    strcpy(new_node->op, op);
    new_node->left_node = left_node;
    new_node->right_node = right_node;
    return new_node;
}

void free_exp_node(exp_node * node){
    free_node(node->left_node);
    free_node(node->right_node);
    free(node->op);
    free(node);
}

print_node * create_print_node(node * content_node){
    //printf("Creando el node print\n");
    print_node * new_node = malloc(sizeof(print_node));
    new_node->type = PRINT_NODE;
    new_node->content_node = content_node;
    return new_node;
}

void free_print_node(print_node * node){
    free_node(node->content_node);
    free(node);
}

node_list * create_node_list(node * node){
    //printf("Creando node_list\n");
    node_list * new_node = malloc(sizeof(node_list));
    new_node->next = NULL;
    new_node->node = node;
    new_node->type = NODE_LIST;
    return new_node;
}

node_list * add_node_list(node * node_header, node * node){
    //printf("Haciendo add_node_list\n");
    node_list * new_node = malloc(sizeof(node_list));
    new_node->node = node;
    new_node->type = NODE_LIST;
    new_node->next = (node_list *) node_header;
    return new_node;
}

void free_node_list(node_list * node_to_free){
    free_node(node_to_free->node);
    if (node_to_free->next != NULL)
        free_node((node *) (node_to_free->next));
    free(node_to_free);
}

rel_comp_node * create_relational_comp_node(char * op, node * left_node, node * right_node){
    //printf("Creando el node relational comp\n");
    rel_comp_node * new_node = malloc(sizeof(rel_comp_node));
    new_node->type = REL_COMP;
    new_node->op = malloc(strlen(op) + 1);
    strcpy(new_node->op, op);
    new_node->left_node = left_node;
    new_node->right_node = right_node;
    return new_node;
}

void free_relational_comp_node(rel_comp_node * node){
    free_node(node->left_node);
    free_node(node->right_node);
    free(node->op);
    free(node);

}

log_comp_node * create_logical_comp_node(char * op, node * left_node, node * right_node){
    //printf("Creando el node logical comp\n");
    log_comp_node * new_node = malloc(sizeof(log_comp_node));
    new_node->type = LOG_COMP;
    new_node->op = malloc(strlen(op) + 1);
    strcpy(new_node->op, op);
    new_node->left_node = left_node;
    new_node->right_node = right_node;
    return new_node;
}

void free_logical_comp_node(log_comp_node * node){
    free_node(node->left_node);
    free_node(node->right_node);
    free(node->op);
    free(node);
}

if_node * create_if_node(node * cond, node * code){
    //printf("Creando el node if\n");
    if_node * new_node = malloc(sizeof(if_node));
    new_node->type = IF_NODE;
    new_node->cond = cond;
    new_node->code = code;
    return new_node;
}

void free_if_node(if_node * node){
    free_node(node->cond);
    free_node(node->code);
    free(node);
}

while_node * create_while_node(node * cond, node * code){
    //printf("Creando el node if\n");
    while_node * new_node = malloc(sizeof(while_node));
    new_node->type = WHILE_NODE;
    new_node->cond = cond;
    new_node->code = code;
    return new_node;
}

void free_while_node(while_node * node){
    free_node(node->cond);
    free_node(node->code);
    free(node);
}

if_otherwise_node * create_if_otherwise_node(node * cond, node * left, node * right){
    if_otherwise_node * new_node = malloc(sizeof(if_otherwise_node));
    new_node->type = IF_OTHERWISE;
    new_node->cond = cond;
    new_node->left_node = left;
    new_node->right_node = right;
    return new_node;
}

void free_if_otherwise_node(if_otherwise_node * node){
    free_node(node->cond);
    free_node(node->left_node);
    free_node(node->right_node);
    free(node);
}


void free_node(node * node){
    switch(node->type){
        case VARIABLE:
            free_variable_node((variable_node *)node);
            break;

        case CONSTANT_INT:
            free_constant_int_node((constant_int_node *)node);
            break;
        
        case DECLARATION:
            free_declaration_node((declaration_node *) node);
            break;
            
        case ASSIGNATION:
            free_assignation_node((assignation_node *) node);
            break;

        case EXP:
            free_exp_node((exp_node *)node);
            break;

        case NODE_LIST:
            free_node_list((node_list *) node);
            break;

        case PRINT_NODE:
            free_print_node((print_node *) node);
            break;   

        case REL_COMP:
            free_relational_comp_node((rel_comp_node *) node);
            break; 

        case LOG_COMP:
            free_logical_comp_node((log_comp_node *) node);
            break;  
        
        case IF_NODE:
            free_if_node((if_node *) node);
            break;  

        case IF_OTHERWISE:
            free_if_otherwise_node((if_otherwise_node *) node);
            break; 

        case WHILE_NODE:
            free_while_node((while_node *) node);
            break; 
    }
}