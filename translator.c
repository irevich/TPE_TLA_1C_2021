#include "translator.h"

char * get_data_type(data_type type){
    char * result = NULL;
    switch(type){
        case INT_TYPE:
            result = malloc(strlen("int") + 1);
            strcpy(result, "int");
            break;
        case STRING_TYPE:
            result = malloc(strlen("char *") + 1);
            strcpy(result, "char *");
            break;
        case CIRCLE_TYPE:
            result = malloc(strlen("circle *") + 1);
            strcpy(result, "circle *");
            break;
        case RECTANGLE_TYPE:
            result = malloc(strlen("rectangle *") + 1);
            strcpy(result, "rectangle *");
            break;
        case TRIANGLE_TYPE:
            result = malloc(strlen("triangle *") + 1);
            strcpy(result, "traingle *");
            break;
        default:
            break;    
    }
    return result;
}

char * get_figure_property_name(figure_property_type type){
    char * property_name = NULL;
    switch(type){
        case RADIUS_TYPE:
            property_name = malloc(strlen("radius")+1);
            strcpy(property_name,"radius");
            break;
        case SIDE_1_TYPE:
            property_name = malloc(strlen("side_1")+1);
            strcpy(property_name,"side_1");
            break;
        case SIDE_2_TYPE:
            property_name = malloc(strlen("side_2")+1);
            strcpy(property_name,"side_2");
            break;

        case SIDE_3_TYPE:
            property_name = malloc(strlen("side_3")+1);
            strcpy(property_name,"side_3");
            break;

        case BASE_TYPE:
            property_name = malloc(strlen("base")+1);
            strcpy(property_name,"base");
            break;
            
        case HEIGHT_TYPE:
            property_name = malloc(strlen("height")+1);
            strcpy(property_name,"height");
            break;

        default:
            break;  
        
    }
    return property_name;
}


int get_cant_digits(int num){
    
    int cant = 0;

    if(num == 0 )
        return 1;

    if( num < 0){
        cant++;
    }
    
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
        case VARIABLE:    
            //printf("Por entrar al translate de variable node\n");
            result = translate_variable_node((variable_node *) node_to_translate);
            //printf("Hecho el translate de variable node\n");
            break;

        case CONSTANT_INT:
            result = translate_constant_int_node((constant_int_node *)node_to_translate);
            break;

        case CONSTANT_STRING:
            result = translate_constant_string_node((constant_string_node *) node_to_translate);
            break;
        
        case DECLARATION:
            result = translate_declaration_node((declaration_node *) node_to_translate);
            break;
            
        case ASSIGNATION:
            result = translate_assignation_node((assignation_node *) node_to_translate);
            break;

        case EXP: 
            result = translate_exp_node((exp_node *)node_to_translate);
            break;

        case FUNCTION_NODE:
            result = translate_function_node((function_node *) node_to_translate);
            break;
        case NODE_LIST:
        case PARAM_NODE_LIST:
            result = translate_node_list((node_list * ) node_to_translate);
            break;

        case PRINT_NODE:
            result = translate_print_node((print_node *) node_to_translate);
            break; 

        case REL_COMP:
            result = translate_rel_comp_node((rel_comp_node *) node_to_translate);
            break; 

        case LOG_COMP:
            result = translate_log_comp_node((log_comp_node *) node_to_translate);
            break;   
        
        case IF_NODE:
            result = translate_if_node((if_node *) node_to_translate);
            break;  

        case IF_OTHERWISE:
            result = translate_if_otherwise_node((if_otherwise_node *) node_to_translate);
            break; 

        case WHILE_NODE:
            result = translate_while_node((while_node *) node_to_translate);
            break;

        case PROPERTY_NODE:
            result = translate_property_node((property_node *) node_to_translate);
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
    
    char * data_type = get_data_type(dec_node->declaration_type);

    char * translated_node = malloc(strlen(translated_assigned_node) + strlen(data_type) + strlen(dec_node->name) + strlen("=") + strlen(";\n") + 3 * strlen(" ") + 1);
    
    sprintf(translated_node, "%s %s = %s;\n", data_type, dec_node->name, translated_assigned_node);

    free(translated_assigned_node);

    free(data_type);

    return translated_node;
    
}

char * translate_assignation_node(assignation_node * assign_node){
    
    char * translated_assigned_node = translate_node(assign_node->assigned_node);

    char * translated_var_node = translate_node(assign_node->var_node);

    char * translated_node = malloc(strlen(translated_var_node) + strlen(translated_assigned_node) + strlen(";\n") + strlen("=") + 2* strlen(" ") + 1);
    
    sprintf(translated_node, "%s = %s;\n", translated_var_node, translated_assigned_node);

    free(translated_assigned_node);

    free(translated_var_node);

    return translated_node;
}


char * translate_constant_int_node (constant_int_node * node){
    int cant_digits = get_cant_digits(node->value);
    char * string_num = malloc(cant_digits+1);
    //printf("Node value : %d\n",node->value);
    sprintf(string_num,"%d",node->value);
    return string_num;
}

char * translate_constant_string_node(constant_string_node * node){
    char * string_value = malloc(strlen(node->value) +1);
    strcpy(string_value, node->value);
    return string_value;
}

char * translate_exp_node ( exp_node * node){
    char * translated_left_node = translate_node(node->left_node);
    char * translated_right_node = translate_node(node->right_node);
    char * exp;
    if (node->has_parentheses) {
        exp = malloc(strlen(translated_left_node)+strlen(node->op)+strlen(translated_right_node)+6);
        sprintf(exp,"(%s %s %s)",translated_left_node,node->op, translated_right_node);
    }
    else {
        exp = malloc(strlen(translated_left_node)+strlen(node->op)+strlen(translated_right_node)+4);
        sprintf(exp,"%s %s %s",translated_left_node,node->op, translated_right_node);
    }
    free(translated_left_node);
    free(translated_right_node);
    return exp;
}

char * translate_function_node(function_node * node){
    char * translated_param_list = translate_node(node->node_param_list);
    char * translated_node;
    char * cast;
    
    if(strcmp(node->name, "get_perimeter") == 0 || strcmp(node->name, "get_area") == 0){
        cast = "(figure *)";
        translated_node = malloc(strlen(node->name) + strlen("(") + strlen(cast) + strlen(translated_param_list) + strlen(")") + 1);
    }
    else{
        cast = "";
        translated_node = malloc(strlen(node->name) + strlen("(") + strlen(translated_param_list) + strlen(")") + 1);
    }
    
    sprintf(translated_node, "%s(%s%s)", node->name, cast, translated_param_list);

    free(translated_param_list);
    return translated_node;
}

char * translate_print_node(print_node * node){
    char * string = NULL;

    char * translated_content_node = translate_node(node->content_node);

    char * translated_print_params;

    char * before = NULL;

    if(node->content_node->type == EXP || node->content_node->type == CONSTANT_INT){
        before = "printf(\"%d\", ";
    }
    else if(node->content_node->type == CONSTANT_STRING){
        before = "printf(\"%s\", "; 
    }
    else if(node->content_node->type == PROPERTY_NODE){
        if( ((property_node *)(node->content_node))->return_type == INT_TYPE){
            before = "printf(\"%d\", ";
        }
    }
    else if(node->content_node->type == FUNCTION_NODE){ //TEMA FUNCIONES CREATE
        if(((function_node *)(node->content_node))->return_type == INT_TYPE){
            before = "printf(\"%d\", ";
        } 
    }
    else if(node->content_node->type == VARIABLE){
        if(((variable_node *)(node->content_node))->variable_type == INT_TYPE){
            before = "printf(\"%d\", "; 
        }
        else if(((variable_node *)(node->content_node))->variable_type == STRING_TYPE){
            before = "printf(\"%s\", "; 
        }
        else{ //special case as type is figure
            before = "printf(\"%s\", "; 
            translated_print_params = malloc(strlen("figure_to_string((figure *) ") + strlen(translated_content_node) + strlen(")") + 1);
            sprintf(translated_print_params, "figure_to_string((figure *) %s)", translated_content_node);
            char * after = ");\n";
    
            string = malloc(strlen(before) + strlen(translated_print_params) + strlen(after) + 1);
            sprintf(string, "%s%s%s", before, translated_print_params, after);
            free(translated_content_node);
            free(translated_print_params);            
            return string;
        }
    }

    char * after = ");\n";
    
    string = malloc(strlen(before) + strlen(translated_content_node) + strlen(after) + 1);
    sprintf(string, "%s%s%s", before, translated_content_node, after);
    free(translated_content_node);
    
    return string;
}

char * translate_rel_comp_node (rel_comp_node * node){
    char * translated_left_node = translate_node(node->left_node);
    char * translated_right_node = translate_node(node->right_node);
    char * comp_string = malloc(strlen(translated_left_node)+strlen(node->op)+strlen(translated_right_node)+4);
    sprintf(comp_string,"%s %s %s",translated_left_node,node->op, translated_right_node);
    free(translated_left_node);
    free(translated_right_node);
    return comp_string;
}

char * translate_log_comp_node (log_comp_node * node){
    char * translated_left_node = translate_node(node->left_node);
    char * translated_right_node = translate_node(node->right_node);
    char * comp_string;
    if (node->has_parentheses) {
        comp_string = malloc(strlen(translated_left_node)+strlen(node->op)+strlen(translated_right_node)+6);
        sprintf(comp_string,"(%s %s %s)",translated_left_node,node->op, translated_right_node);
    }
    else {
        comp_string = malloc(strlen(translated_left_node)+strlen(node->op)+strlen(translated_right_node)+4);
        sprintf(comp_string,"%s %s %s",translated_left_node,node->op, translated_right_node);

    }
    free(translated_left_node);
    free(translated_right_node);
    return comp_string;
}

char * translate_if_node (if_node * node){
    char * string = NULL;

    char * if_start = "if(";                                  
    char * translated_comp = translate_node(node->cond);
    char * if_close = "){\n";
    char * translated_code = translate_node(node->code);
    char * code_end = "}\n";
    
    string = malloc(strlen(if_start) + strlen(translated_comp) + strlen(if_close) + strlen(translated_code) + strlen(code_end) + 1);
    sprintf(string, "%s%s%s%s%s", if_start, translated_comp, if_close, translated_code, code_end);
    free(translated_comp);
    free(translated_code);
    
    return string;
}

char * translate_if_otherwise_node (if_otherwise_node * node){
    char * string = NULL;

    char * if_start = "if(";                                  
    char * translated_comp = translate_node(node->cond);
    char * if_close = "){\n";
    char * translated_code = translate_node(node->left_node);
    char * if_code_end = "} else {\n";
    char * translated_else_code = translate_node(node->right_node);
    char * else_code_end = "}\n";
    
    string = malloc(strlen(if_start) + strlen(translated_comp) + strlen(if_close) + strlen(translated_code) + strlen(if_code_end) + strlen(translated_else_code) + strlen(else_code_end) + 1);
    sprintf(string, "%s%s%s%s%s%s%s", if_start, translated_comp, if_close, translated_code, if_code_end, translated_else_code, else_code_end);
    free(translated_comp);
    free(translated_code);
    free(translated_else_code);

    return string;
}

char * translate_property_node (property_node * node){

    char * property_type_name = get_figure_property_name(node->property_type);

    char * translated_var_node = translate_node(node->var_node);

    char * translated_node = malloc(strlen(translated_var_node) + strlen("->") + strlen(property_type_name) + 1);

    sprintf(translated_node, "%s%s%s", translated_var_node, "->", property_type_name);
    
    free(property_type_name);

    free(translated_var_node);
    
    return translated_node;
}

char * translate_node_list(node_list * list_node){
    char * translated_actual_node = translate_node(list_node->node);
    char * translated_next_node = "";
    char * translated_list_node;
    if (list_node->next != NULL){
        translated_next_node = translate_node((node *) list_node->next);
        if(list_node->type == NODE_LIST){
            translated_list_node = malloc(strlen(translated_actual_node) + strlen(translated_next_node) + 1);
            sprintf(translated_list_node, "%s%s",translated_actual_node, translated_next_node);
        }
        else{
            translated_list_node = malloc(strlen(translated_actual_node) + strlen(translated_next_node) + strlen(",") + 1);
            sprintf(translated_list_node, "%s,%s",translated_actual_node, translated_next_node);
        }

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

char * translate_while_node (while_node * node){
    char * string = NULL;

    char * while_start = "while(";                                  
    char * translated_comp = translate_node(node->cond);
    char * while_close = "){\n";
    char * translated_code = translate_node(node->code);
    char * code_end = "}\n";
    
    string = malloc(strlen(while_start) + strlen(translated_comp) + strlen(while_close) + strlen(translated_code) + strlen(code_end) + 1);
    sprintf(string, "%s%s%s%s%s", while_start, translated_comp, while_close, translated_code, code_end);
    free(translated_comp);
    free(translated_code);
    
    return string;
}

void print_initial_functions(){
    
    printf("#define TO_STRING_INITIAL_SIZE 200\ntypedef enum{CIRCLE_TYPE,RECTANGLE_TYPE,TRIANGLE_TYPE,}figure_type;\ntypedef struct figure{figure_type type;}figure;\n\
        typedef struct circle{figure_type type;int radius;}circle;\ntypedef struct rectangle{figure_type type;int base;int height;}rectangle;\ntypedef struct triangle{figure_type type;\
        int side1;int side2;int side3;}triangle;");

    printf("\n\n//Create functions\ncircle * create_circle(int radius);rectangle * create_rectangle(int base, int height);triangle * create_triangle(int side1, int side2, int side3);\
        \n//Free functions\nvoid free_figure(figure * figure);void free_circle(circle * circle);void free_rectangle(rectangle * rectangle);void free_triangle(triangle * triangle);\
        \n//To string functions\nchar * figure_to_string(figure * figure);char * circle_to_string(circle * circle);char * rectangle_to_string(rectangle * rectangle);char * triangle_to_string(triangle * triangle);\
        \n//Perimeter functions\nint get_perimeter(figure * figure);int get_circle_perimeter (circle * circle);int get_rectangle_perimeter (rectangle * rectangle);int get_triangle_perimeter (triangle * triangle);\
        \n//Area functions\nint get_area(figure * figure);int get_circle_area (circle * circle);int get_rectangle_area (rectangle * rectangle);int get_triangle_area (triangle * triangle);\n\n");

    printf("circle * create_circle(int radius){\ncircle * new_circle = malloc(sizeof(struct circle));\nnew_circle->radius = radius;\nnew_circle->type = CIRCLE_TYPE;\nreturn new_circle;}\n\
        rectangle * create_rectangle(int base, int height){\nrectangle * new_rectangle = malloc(sizeof(struct rectangle));\nnew_rectangle->base = base;\nnew_rectangle->height = height;\nnew_rectangle->type = RECTANGLE_TYPE;return new_rectangle;}\n \
        triangle * create_triangle(int side1, int side2, int side3){\ntriangle * new_triangle = malloc(sizeof(struct triangle));\nnew_triangle->side1 = side1;\nnew_triangle->side2 = side2;\nnew_triangle->side3 = side3;\nnew_triangle->type = TRIANGLE_TYPE;\nreturn new_triangle;}\n");
    
    printf("//Free functions\nvoid free_figure(figure * figure){\nswitch(figure->type){\ncase CIRCLE_TYPE:free_circle((circle *)figure);break;\ncase RECTANGLE_TYPE:free_rectangle((rectangle*)figure);break;\n \
        case TRIANGLE_TYPE:free_triangle((triangle*)figure);break;}}\nvoid free_circle(circle * circle){free(circle);}\nvoid free_rectangle(rectangle * rectangle){free(rectangle);}\nvoid free_triangle(triangle * triangle){free(triangle);}\n");

    printf("//To string functions\nchar * figure_to_string(figure * figure){\nswitch(figure->type){\ncase CIRCLE_TYPE:return circle_to_string((circle *)figure);\ncase RECTANGLE_TYPE:return rectangle_to_string((rectangle*)figure);\ncase TRIANGLE_TYPE:  \
        return triangle_to_string((triangle*)figure);}\nreturn NULL;}\nchar * circle_to_string(circle * circle){char * string = malloc(TO_STRING_INITIAL_SIZE);sprintf(string, \"Circle with radius : %%d\", circle->radius);string = realloc(string, strlen(string) + 1);  \
        return string;}\nchar * rectangle_to_string(rectangle * rectangle){\nchar * string = malloc(TO_STRING_INITIAL_SIZE);sprintf(string, \"Rectangle with base : %%d and height : %%d\", rectangle->base, rectangle->height);string = realloc(string, strlen(string) + 1); \
        return string;}\nchar * triangle_to_string(triangle * triangle){\nchar * string = malloc(TO_STRING_INITIAL_SIZE);sprintf(string, \"Triangle with first side : %%d, second side : %%d and third side : %%d\", triangle->side1, triangle->side2, triangle->side3); \
        string = realloc(string, strlen(string) + 1);return string;}\n");

    printf("//Perimeter functions\nint get_perimeter(figure * figure){\nswitch(figure->type){\ncase CIRCLE_TYPE:\nreturn get_circle_perimeter((circle *)figure);\ncase RECTANGLE_TYPE:\nreturn get_rectangle_perimeter((rectangle*)figure);\ncase TRIANGLE_TYPE:\nreturn get_triangle_perimeter((triangle*)figure);\n}\nreturn -1;\n}\n\
        int get_circle_perimeter (circle * circle){\nreturn (int) (circle->radius * 2 * M_PI);\n}\n\
        int get_rectangle_perimeter (rectangle * rectangle){\nreturn rectangle->base * 2 + rectangle->height * 2;\n}\n\
        int get_triangle_perimeter (triangle * triangle){\nreturn triangle->side1 + triangle->side2 + triangle->side3;\n}\n");

    printf("//Area functions\nint get_area(figure * figure){\
        switch(figure->type){\ncase CIRCLE_TYPE:\nreturn get_circle_area((circle *)figure);\ncase RECTANGLE_TYPE:\nreturn get_rectangle_area((rectangle*)figure);\ncase TRIANGLE_TYPE:\nreturn get_triangle_area((triangle*)figure);\n}\nreturn -1;\n}\n\
        int get_circle_area (circle * circle){\nreturn (int) (pow(circle->radius, 2) * M_PI);\n}\
        int get_rectangle_area (rectangle * rectangle){\nreturn rectangle->base * rectangle->height;\n}\n\
        //Using Heron formula\nint get_triangle_area (triangle * triangle){\ndouble semiperimeter = get_triangle_perimeter(triangle) / 2;\nreturn (int) (sqrt(semiperimeter * (semiperimeter - triangle->side1) * (semiperimeter - triangle->side2) * (semiperimeter - triangle->side3))); \n}\n"
        );
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