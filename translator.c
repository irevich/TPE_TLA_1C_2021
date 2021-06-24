#include "translator.h"

char * get_data_type(data_type type){
    char * result = NULL;
    switch(type){
        case INT_TYPE:
            result = malloc(strlen("int") + 1);
            strcpy(result, "int");
            break;
        default:
            break;    
    }
    return result;
}


int get_cant_digits(int num){
    
    int cant = 0;

    if(num == 0 )
        return 1;
    
    while(num!=0){
        num/=10;
        cant++;
    }
    return cant;
}



char * translate_node(node * node_to_translate){
    char * result = NULL;
    //printf("Por entrar al switch de translate node\n");
    switch(node_to_translate->type){
        case VARIABLE:     //juani listo
            //printf("Por entrar al translate de variable node\n");
            result = translate_variable_node((variable_node *) node_to_translate);
            //printf("Hecho el translate de variable node\n");
            break;

        case CONSTANT_INT:  //Igol listo
            //printf("Por entrar al translate de constant node\n");
            result = translate_constant_int_node((constant_int_node *)node_to_translate);
            //printf("Hecho el translate de constant node\n");
            break;
        
        case DECLARATION: //BETO listo
            //printf("Por entrar al translate de declaration node\n");
            result = translate_declaration_node((declaration_node *) node_to_translate);
            //printf("Hecho el translate de declaration node\n");
            break;
            
        case ASSIGNATION: //BETO listo
            //printf("Por entrar al translate de assignation node\n");
            result = translate_assignation_node((assignation_node *) node_to_translate);
            //printf("Hecho el translate de assignation node\n");
            break;

        case EXP:   //Igol listo
            //printf("Por entrar al translate de exp node\n");
            result = translate_exp_node((exp_node *)node_to_translate);
            //printf("Hecho el translate de exp node\n");
            break;

        case NODE_LIST: //Beto
            //printf("Por entrar al translate de node_list node\n");
            result = translate_node_list((node_list * ) node_to_translate);
            //printf("Hecho el translate de node_list node\n");
            break;

        case PRINT_NODE:    //juani listo
            //printf("Por entrar al translate de print_node node\n");
            result = translate_print_node((print_node *) node_to_translate);
            //printf("Hecho el translate de print_node node\n");
            break; 
    }
    return result;
}


char * translate_variable_node(variable_node * node){
    char * name = malloc(strlen(node->name)+1);
    strcpy(name, node->name);
    return name;
}


char * translate_declaration_node(declaration_node * dec_node){
    
    char * translated_assigned_node = translate_node(dec_node->assigned_node);

    //printf("Assigned node: %s\n", translated_assigned_node);

    //printf("Por agarrar el data type\n");
    
    char * data_type = get_data_type(dec_node->declaration_type);

    //printf("Agarrado el data type\n");

    //printf("Data type : %s\n",data_type);

    char * translated_node = malloc(strlen(translated_assigned_node) + strlen(data_type) + strlen(dec_node->name) + strlen("=") + strlen(";\n") + 3 * strlen(" ") + 1);
    
    sprintf(translated_node, "%s %s = %s;\n", data_type, dec_node->name, translated_assigned_node);

    free(translated_assigned_node);

    free(data_type);

    return translated_node;
    
}

char * translate_assignation_node(assignation_node * assign_node){
    
    char * translated_assigned_node = translate_node(assign_node->assigned_node);

    char * translated_node = malloc(strlen(assign_node->name) + strlen(translated_assigned_node) + strlen(";\n") + strlen("=") + 2* strlen(" ") + 1);
    
    sprintf(translated_node, "%s = %s;\n", assign_node->name, translated_assigned_node);

    free(translated_assigned_node);

    return translated_node;
}


char * translate_constant_int_node (constant_int_node * node){
    int cant_digits = get_cant_digits(node->value);
    char * string_num = malloc(cant_digits+1);
    //printf("Node value : %d\n",node->value);
    sprintf(string_num,"%d",node->value);
    return string_num;
}

char * translate_exp_node ( exp_node * node){
    char * translated_left_node = translate_node(node->left_node);
    char * translated_right_node = translate_node(node->right_node);
    char * exp = malloc(strlen(translated_left_node)+strlen(node->op)+strlen(translated_right_node)+4);
    sprintf(exp,"%s %s %s",translated_left_node,node->op, translated_right_node);
    free(translated_left_node);
    free(translated_right_node);
    return exp;
}

char * translate_print_node(print_node * node){
    char * string = NULL;

    char * before = "printf(\"%d\", ";                                   //Do I need to escape the " " and %x
    char * translated_content_node = translate_node(node->content_node);
    char * after = ");\n";
    
    string = malloc(strlen(before) + strlen(translated_content_node) + strlen(after) + 1);
    sprintf(string, "%s%s%s", before, translated_content_node, after);
    free(translated_content_node);
    
    return string;
}

char * translate_node_list(node_list * list_node){
    char * translated_actual_node = translate_node(list_node->node);
    char * translated_next_node = "";
    char * translated_list_node;
    if (list_node->next != NULL){
        translated_next_node = translate_node((node *) list_node->next);
        translated_list_node = malloc(strlen(translated_actual_node) + strlen(translated_next_node) + 1);

        sprintf(translated_list_node, "%s%s",translated_actual_node, translated_next_node);

        free(translated_actual_node);
        free(translated_next_node);
    }
    else{
        translated_list_node = malloc(strlen(translated_actual_node) + 1);

        sprintf(translated_list_node, "%s",translated_actual_node);
        
        free(translated_actual_node);
    }

    return translated_list_node;

}

char * translate_to_c(node_list * first_node){

    //printf("Por hacer la translation de C\n");
    
    char * translation = translate_node((node *)first_node);

    //printf("Hice la translation de C\n");

    char * return_value = malloc(strlen(translation) + 1);

    sprintf(return_value, "%s", translation);

    free(translation);

    return return_value;

}    