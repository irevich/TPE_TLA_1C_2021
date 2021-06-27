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
            strcpy(result, "triangle *");
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
            property_name = malloc(strlen("side1")+1);
            strcpy(property_name,"side1");
            break;
        case SIDE_2_TYPE:
            property_name = malloc(strlen("side2")+1);
            strcpy(property_name,"side2");
            break;

        case SIDE_3_TYPE:
            property_name = malloc(strlen("side3")+1);
            strcpy(property_name,"side3");
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

    switch(node_to_translate->type){
        case VARIABLE:    
            result = translate_variable_node((variable_node *) node_to_translate);
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

        case PARENTHESES_EXP_NODE:
            result = translate_parentheses_exp_node((parentheses_exp_node *) node_to_translate);
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

        case READ_NUM_NODE:
            result = translate_read_num_node((read_num_node *) node_to_translate);
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
    
    char * exp = malloc(strlen(translated_left_node)+strlen(node->op)+strlen(translated_right_node)+4);
    sprintf(exp,"%s %s %s",translated_left_node,node->op, translated_right_node);

    free(translated_left_node);
    free(translated_right_node);
    return exp;
}

char * translate_parentheses_exp_node(parentheses_exp_node * node){
    
    char * translated_exp_node = translate_node(node->exp_node);

    char * translated_node = malloc(strlen(translated_exp_node) + strlen("(") + strlen(")") + 1);
    sprintf(translated_node, "(%s)", translated_exp_node);

    free(translated_exp_node);

    return translated_node;

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

    if(get_node_data_type(node->content_node) == INT_TYPE){
        before = "printf(\"%d\", ";
    }
    else if(get_node_data_type(node->content_node) == STRING_TYPE){
        before = "printf(\"%s\", ";
    }
    else{ //figure type
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
    char * after = ");\n";
    
    string = malloc(strlen(before) + strlen(translated_content_node) + strlen(after) + 1);
    sprintf(string, "%s%s%s", before, translated_content_node, after);
    free(translated_content_node);
    
    return string;
}

char * translate_rel_comp_node (rel_comp_node * node){
    char * translated_left_node = translate_node(node->left_node);
    char * translated_right_node = translate_node(node->right_node);
    char * comp_string;

    if(is_type_figure(get_node_data_type(node->left_node))){
        
        if (strcmp(node->op, "!=") == 0){
            comp_string = malloc(strlen("!are_the_same_figure(") + 2 *strlen("(figure *)") + strlen(",") + strlen(translated_left_node) + strlen(translated_right_node) + strlen(")") + 1);
            sprintf(comp_string, "!are_the_same_figure((figure *)%s,(figure *)%s)", translated_left_node, translated_right_node);
        }
        else{
            comp_string = malloc(strlen("are_the_same_figure(") + 2 *strlen("(figure *)") + strlen(",") + strlen(translated_left_node) + strlen(translated_right_node) + strlen(")") + 1);
            sprintf(comp_string, "are_the_same_figure((figure *)%s,(figure *)%s)", translated_left_node, translated_right_node);
        }
    }
    
    else{
        comp_string = malloc(strlen(translated_left_node)+strlen(node->op)+strlen(translated_right_node)+4);
        sprintf(comp_string,"%s %s %s",translated_left_node,node->op, translated_right_node);
    }
    free(translated_left_node);
    free(translated_right_node);
    return comp_string;
}

char * translate_log_comp_node (log_comp_node * node){
    char * translated_left_node = translate_node(node->left_node);
    char * translated_right_node = translate_node(node->right_node);
    
    char * comp_string = malloc(strlen(translated_left_node)+strlen(node->op)+strlen(translated_right_node)+4);
    sprintf(comp_string,"%s %s %s",translated_left_node,node->op, translated_right_node);

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

char * translate_read_num_node(read_num_node * node){
    char * string = NULL;

    char * translated_content_node = translate_node(node->content_node);

    char * before = NULL;

    before = "scanf(\"%d\", &";
    
    char * after = ");\n";
    
    string = malloc(strlen(before) + strlen(translated_content_node) + strlen(after) + 1);
    sprintf(string, "%s%s%s", before, translated_content_node, after);
    free(translated_content_node);
    
    return string;
}

void print_initial_functions(){
    
    printf("#define TO_STRING_INITIAL_SIZE 200\n\
        typedef enum{CIRCLE_TYPE,RECTANGLE_TYPE,TRIANGLE_TYPE,}figure_type;\n\
        typedef struct figure{figure_type type;}figure;\n\
        typedef struct circle{figure_type type;int radius;}circle;\n\
        typedef struct rectangle{figure_type type;int base;int height;}rectangle;\n\
        typedef struct triangle{figure_type type;\
        int side1;int side2;int side3;\
        }triangle;");

    printf("typedef struct pointer_to_free_struct{\n\
                void * pointer;\n\
                struct pointer_to_free_struct * next;\n\
            }pointer_to_free_struct;\n\
    pointer_to_free_struct * pointer_to_free_header = NULL;\n\
    pointer_to_free_struct * pointer_to_free_current;\n\
    void add_pointer_to_free(void * pointer_to_free);\n\
    void free_alloqued_pointers();\n\n\
    void add_pointer_to_free(void * pointer_to_free){\n\
        if (pointer_to_free_header == NULL){\n\
            pointer_to_free_header = malloc(sizeof(pointer_to_free_struct));\n\
            pointer_to_free_header->pointer = pointer_to_free;\n\
            pointer_to_free_header->next = NULL;\n\
            \npointer_to_free_current = pointer_to_free_header;\n\
        \n}else{\n\
            pointer_to_free_struct * new_pointer_to_free = malloc(sizeof(pointer_to_free_struct));\
            pointer_to_free_current->next = new_pointer_to_free;\
            new_pointer_to_free->pointer = pointer_to_free;new_pointer_to_free->next = NULL;\
            pointer_to_free_current = new_pointer_to_free;}\n\
        }\n\
        void free_alloqued_pointers(){\n\
        pointer_to_free_struct * curr = pointer_to_free_header;\n\
        while (curr != NULL){\n\
            pointer_to_free_struct * aux = curr->next;\n\
            free(curr->pointer);free(curr);curr = aux;\n\
        }\n}\
        ");

    printf("\n\n//Create functions\n\
        circle * create_circle(int radius);rectangle * create_rectangle(int base, int height);triangle * create_triangle(int side1, int side2, int side3);\
        \n//Free functions\n\
        void free_figure(figure * figure);void free_circle(circle * circle);void free_rectangle(rectangle * rectangle);void free_triangle(triangle * triangle);\
        \n//To string functions\n\
        char * figure_to_string(figure * figure);char * circle_to_string(circle * circle);char * rectangle_to_string(rectangle * rectangle);char * triangle_to_string(triangle * triangle);\
        \n//Perimeter functions\n\
        int get_perimeter(figure * figure);int get_circle_perimeter (circle * circle);int get_rectangle_perimeter (rectangle * rectangle);int get_triangle_perimeter (triangle * triangle);\
        \n//Area functions\n\
        int get_area(figure * figure);int get_circle_area (circle * circle);int get_rectangle_area (rectangle * rectangle);int get_triangle_area (triangle * triangle);\n\
        \n//Figure equals functions\n\
        int are_the_same_figure(struct figure * f1, struct figure * f2);\nint are_the_same_circle(struct circle * c1, struct circle * c2);\nint are_the_same_triangle(struct triangle * t1, struct triangle * t2);\nint are_the_same_rectangle(struct rectangle * r1, struct rectangle * r2);");
    
    printf("//Create functions\n\n\
        circle * create_circle(int radius){\n\
        circle * new_circle = malloc(sizeof(struct circle));\n\
        new_circle->radius = radius;\n\
        new_circle->type = CIRCLE_TYPE;\n\
        add_pointer_to_free((void *) new_circle);\n\
        return new_circle;}\n\
        \
        rectangle * create_rectangle(int base, int height){\n\
        rectangle * new_rectangle = malloc(sizeof(struct rectangle));\n\
        new_rectangle->base = base;\n\
        new_rectangle->height = height;\n\
        new_rectangle->type = RECTANGLE_TYPE;\n\
        add_pointer_to_free((void *) new_rectangle);\n\
        return new_rectangle;}\n\
        \
        triangle * create_triangle(int side1, int side2, int side3){\n\
        triangle * new_triangle = malloc(sizeof(struct triangle));\n\
        new_triangle->side1 = side1;\n\
        new_triangle->side2 = side2;\n\
        new_triangle->side3 = side3;\n\
        new_triangle->type = TRIANGLE_TYPE;\n\
        add_pointer_to_free((void *) new_triangle);\n\
        return new_triangle;}\n");
    
    printf("//Free functions\n\
        void free_figure(figure * figure){\n\
        switch(figure->type){\n\
            case CIRCLE_TYPE:\n\
            free_circle((circle *)figure);break;\n\
            case RECTANGLE_TYPE:\n\
            free_rectangle((rectangle*)figure);break;\n\
            case TRIANGLE_TYPE:\n\
            free_triangle((triangle*)figure);break;}}\n\
        \n\
        void free_circle(circle * circle){free(circle);}\n\
        void free_rectangle(rectangle * rectangle){free(rectangle);}\n\
        void free_triangle(triangle * triangle){free(triangle);}\n");

    printf("//To string functions\n\n\
        char * figure_to_string(figure * figure){\n\
        switch(figure->type){\n\
            case CIRCLE_TYPE:\n\
            return circle_to_string((circle *)figure);\n\
            case RECTANGLE_TYPE:\n\
            return rectangle_to_string((rectangle*)figure);\n\
            case TRIANGLE_TYPE:\n\
            return triangle_to_string((triangle*)figure);}\n\
        return NULL;}\n\
        \n\
        char * circle_to_string(circle * circle){\n\
        char * string = malloc(TO_STRING_INITIAL_SIZE);\n\
        sprintf(string, \"Circle with radius : %%d\", circle->radius);\n\
        string = realloc(string, strlen(string) + 1);\n\
        add_pointer_to_free((void *) string);\n\
        return string;}\n\
        \n\
        char * rectangle_to_string(rectangle * rectangle){\n\
        char * string = malloc(TO_STRING_INITIAL_SIZE);\n\
        sprintf(string, \"Rectangle with base : %%d and height : %%d\", rectangle->base, rectangle->height);\n\
        string = realloc(string, strlen(string) + 1);\n\
        add_pointer_to_free((void *) string);return string;}\n\
        \n\
        char * triangle_to_string(triangle * triangle){\n\
        char * string = malloc(TO_STRING_INITIAL_SIZE);\n\
        sprintf(string, \"Triangle with first side : %%d, second side : %%d and third side : %%d\", triangle->side1, triangle->side2, triangle->side3);\n\
        string = realloc(string, strlen(string) + 1);\n\
        add_pointer_to_free((void *) string);\n\
        return string;}\n");

    printf("//Perimeter functions\n\
        int get_perimeter(figure * figure){\n\
        switch(figure->type){\n\
            case CIRCLE_TYPE:\n\
            return get_circle_perimeter((circle *)figure);\n\
            case RECTANGLE_TYPE:\n\
            return get_rectangle_perimeter((rectangle*)figure);\n\
            case TRIANGLE_TYPE:\n\
            return get_triangle_perimeter((triangle*)figure);\n\
        }\n\
        return -1;\n}\n\
        \
        int get_circle_perimeter (circle * circle){\nreturn (int) (circle->radius * 2 * M_PI);\n}\n\
        int get_rectangle_perimeter (rectangle * rectangle){\nreturn rectangle->base * 2 + rectangle->height * 2;\n}\n\
        int get_triangle_perimeter (triangle * triangle){\nreturn triangle->side1 + triangle->side2 + triangle->side3;\n}\n");

    printf("//Area functions\nint get_area(figure * figure){\n\
        switch(figure->type){\n\
            case CIRCLE_TYPE:\n\
            return get_circle_area((circle *)figure);\n\
            case RECTANGLE_TYPE:\n\
            return get_rectangle_area((rectangle*)figure);\n\
            case TRIANGLE_TYPE:\n\
            return get_triangle_area((triangle*)figure);\n}\
        return -1;\n}\n\
        \
        int get_circle_area (circle * circle){\n\
        return (int) (pow(circle->radius, 2) * M_PI);\n}\
        \
        int get_rectangle_area (rectangle * rectangle){\n\
        return rectangle->base * rectangle->height;\n}\n\
        \
        //Using Heron formula\n\
        int get_triangle_area (triangle * triangle){\n\
        double semiperimeter = get_triangle_perimeter(triangle) / 2;\n\
        return (int) (sqrt(semiperimeter * (semiperimeter - triangle->side1) * (semiperimeter - triangle->side2) * (semiperimeter - triangle->side3))); \n}\n");

    printf("//Figure equals functions\n\
        int are_the_same_figure(struct figure * f1, struct figure * f2){\n\
        if (f1->type != f2->type) return 0;\n\
        figure_type figs_type = f1->type;\n\
        switch (figs_type) {\n\
            case CIRCLE_TYPE:\nreturn are_the_same_circle((struct circle *) f1, (struct circle *) f2);\n\
            case TRIANGLE_TYPE:\nreturn are_the_same_triangle((struct triangle *) f1, (struct triangle *) f2);\n\
            case RECTANGLE_TYPE:\nreturn are_the_same_rectangle((struct rectangle *) f1, (struct rectangle *) f2);\n\
            }return 0;}\n\
        \
        int are_the_same_circle(struct circle * c1, struct circle * c2){\n\
        return c1->radius == c2->radius;\
        }\n\
        \
        \nint are_the_same_rectangle(struct rectangle * r1, struct rectangle * r2){\n\
            return r1->base == r2->base && r1->height == r2->height;\
        }\n\
        \
        int are_the_same_triangle(struct triangle * t1, struct triangle * t2){\n\
            return t1->side1 == t2->side1 && t1->side2 == t2->side2 && t1->side3 == t2->side3;\n\
        }\n\n");
}


char * translate_to_c(node_list * first_node){
    
    char * translation = translate_node((node *)first_node);

    char * return_value = malloc(strlen(translation) + 1);

    sprintf(return_value, "%s", translation);

    free(translation);

    return return_value;

}    