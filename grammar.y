%{
    //Declarations

    #include "node.h"
    #include "translator.h"
    #include "stdbool.h"
    
    #define MAX_PARAMS 10
    #define STANDARD_FUNCTIONS 5

    int yylex();

    int extern yylineno;

    void yyerror(node_list ** node_list_param, char const * s);

    typedef struct variable_list_node{
        data_type type;
        char * name;
        struct variable_list_node * next; 
    }variable_list_node;

    typedef struct function_defintion{
        char * name;
        int total_params;
        data_type params_type[MAX_PARAMS];
    }function_definition;

    function_definition * functions_list[STANDARD_FUNCTIONS];

    variable_list_node * variable_header;
    variable_list_node * current_variable_node;

    bool check_variable(char * name);

    void create_variable(data_type type, char * name);

    variable_list_node * find_variable(char * name);

    bool is_figure (variable_list_node * variable);

    bool check_figure_property (char * name, figure_property_type property_type);

    bool figure_has_property(variable_list_node * variable, figure_property_type property_type);

    void free_variables();

    void free_functions_definitions();

    bool check_parameter_type(char* function_name, data_type type, int param_index);
    
%}

//Definitions

//Tokens

%union{
    char string[500];
    int num;
    node * node;
    node_list * node_list;
    data_type data;
    figure_property_type figure_property_type;
}

//Program delimeter tokens
%token START;
%token END;

//If-block tokens
%token IF;
%token OTHERWISE;
%token OPEN_PARENTHESES;
%token CLOSE_PARENTHESES;
%token OPEN_BRACES;
%token CLOSE_BRACES;

//While-block tokens
%token WHILE;

//Instruction delimeter token
%token SEMICOLON;

//Aritmethical tokens
%token<string> PLUS;
%token<string> MINUS;
%token<string> PRODUCT;
%token<string> DIVISION;

//Assign token
%token ASSIGN;

//Relational tokens
%token<string> GT;
%token<string> LT;
%token<string> GE;
%token<string> LE;
%token<string> EQ;
%token<string> NEQ;

//Logical tokens
%token<string> AND;
%token<string> OR;

//Datatypes tokens
%token<data_type> INT; 
%token<data_type> STRING;
%token<data_type> CIRCLE;
%token<data_type> RECTANGLE;
%token<data_type> TRIANGLE;

//String delimeter token
%token QM;
%token COMA;

//Figure property token
%token ARROW;

//Rectangle properties tokens
%token<figure_property_type> BASE;
%token<figure_property_type> HEIGHT;

//Figure functions tokens

%token<string> PERIMETER;
%token<string> AREA;

//Rectangle functions tokens
%token<string> CREATE_R;

//Triangle properties tokens
%token<figure_property_type>SIDE_1;
%token<figure_property_type>SIDE_2;
%token<figure_property_type>SIDE_3;

//Triangle functions tokens
%token<string> CREATE_T;

//Circle properties tokens
%token<figure_property_type> RADIUS;

//Circle functions tokens
%token<string> CREATE_C;

//General functions tokens

%token PRINT;

//Text token
%token<string> TEXT;

//Identifier token
%token<string> IDENTIFIER;

//Num token
%token<num> NUM;

//Anything else
%token SYNTAX_ERROR;


//Non-Terminals
%type<node> code;
%type<node> instruction;
%type<node> declaration;
%type<node> factor;
%type<node> assignation;
%type<node> exp;
%type<node> term;
%type<node> param;
%type<node_list> program; 
%type<node> if_block;
%type<node> while_block;
%type<node> comp;
%type<node> comp_term;
%type<node> comp_factor;
%type<string> or;
%type<data> var_type;
%type<node> otherwise_block;
%type<node> figure_property;
%type<figure_property_type> property;
%type<node> func;  



//Starting rule
%start program

%parse-param{node_list ** program_list}


//Productions
%%

    program     :   START code END      { *program_list = (node_list *)$2; $$ = *program_list;}
                ;           
 
    code        :   instruction code        { $$ = (node *) add_node_list($2, $1, NODE_LIST);}
                |   instruction             {$$ = (node *) create_node_list($1, NODE_LIST);}
                ;

    instruction :   declaration SEMICOLON       {;}
                |   assignation SEMICOLON       {;}
                |   if_block                    {;}
                |   while_block                 {;}
                |   PRINT OPEN_PARENTHESES param CLOSE_PARENTHESES SEMICOLON     {$$ = (node*) create_print_node($3);}
                ;

    if_block    :   IF OPEN_PARENTHESES comp CLOSE_PARENTHESES OPEN_BRACES code CLOSE_BRACES otherwise_block    {if($8 == NULL){$$ = (node*) create_if_node($3, $6);} else {$$ = (node*) create_if_otherwise_node($3, $6, $8);}}
                ;

    otherwise_block : OTHERWISE OPEN_BRACES code CLOSE_BRACES {$$ = $3;}
                    |                                        {$$ = NULL;}
                    ;

    while_block :   WHILE OPEN_PARENTHESES comp CLOSE_PARENTHESES OPEN_BRACES code CLOSE_BRACES         {$$ = (node*) create_while_node($3, $6);}

    comp        :   comp OR comp_term       {   $$ = (node*) create_logical_comp_node("||",$1, $3);  }
                |   comp_term               {   $$ = $1;    }
                ;

    comp_term   :   comp_term AND comp_factor    {   $$ = (node*) create_logical_comp_node("&&",$1, $3);  }
                |   comp_factor                  {   $$ = $1;   }
                ;

    comp_factor :   exp or exp              {   $$ = (node*) create_relational_comp_node($2,$1,$3);  }
                ; 
    
    or          :   LT          { strcpy($$, "<");}
                |   LE          { strcpy($$, "<=");}
                |   GT          { strcpy($$, ">");}
                |   GE          { strcpy($$, ">=");}
                |   EQ          { strcpy($$, "==");}
                |   NEQ         { strcpy($$, "!=");}
                ;

    
    declaration :   var_type IDENTIFIER ASSIGN param  {                  
                    if(check_variable($2)){
                        fprintf(stderr, "Error. Variable %s already declared\n", $2);
                        free_variables();
                        exit(-1);
                    }
                    create_variable($1, $2);
                    $$ = (node*) create_declaration_node($2,$1, $4);
                    if($$ == NULL){
                        fprintf(stderr, "Error. Declaration of variable %s with incorrect data type", $2);
                        free_variables();
                        free_node($4);
                        exit(-1);
                    }                    
                    }
                ;

    var_type   :   INT     { $$ = INT_TYPE;}
               |   STRING  { $$ = STRING_TYPE;}
               |   CIRCLE  { $$ = CIRCLE_TYPE;}
               |   RECTANGLE { $$ = RECTANGLE_TYPE;}
               |   TRIANGLE  { $$ = TRIANGLE_TYPE;}
               ;

    assignation :   IDENTIFIER ASSIGN param     {

                    if(!check_variable($1)){
                        fprintf(stderr, "Error. Variable %s not declared\n", $1);
                        free_variables();
                        exit(-1);
                    }
                    $$ = (node *) create_assignation_node($1, $3);
                    }
                ;

    param       :   exp         {$$ = $1;}
                |   TEXT        {$$ = (node *) create_constant_string_node($1);}
                ;
    

    exp         :   exp PLUS term       {   $$ = (node*) create_exp_node("+",$1,$3);  }
                |   exp MINUS term      {   $$ = (node*) create_exp_node("-",$1,$3);  }
                |   term                {$$ = $1;}
                ;

    term        :   term PRODUCT factor     {   $$ = (node*) create_exp_node("*",$1,$3);  }
                |   term DIVISION factor    {   $$ = (node*) create_exp_node("/",$1,$3);  }
                |   factor                  {$$ = $1;}
                ;

    factor      :   IDENTIFIER      {
                                        variable_list_node * variable = find_variable($1);
                                        if(variable == NULL){
                                            fprintf(stderr, "Error. Variable %s not declared\n", $1);
                                            free_variables();
                                            exit(-1);
                                        }
                                        $$ = (node*)create_variable_node(variable->type, $1);
                                    }
                |   NUM             {$$ = (node*)create_constant_int_node($1);}
                |   figure_property {$$ =  $1;}
                |   func            {;}
                ;

    func    :   CREATE_C OPEN_PARENTHESES param CLOSE_PARENTHESES                           {   
                                                                                                if(!check_parameter_type($1, get_node_data_type($3), 0)){
                                                                                                    fprintf(stderr, "Error. Invalid function parameter type on function %s\n", $1);
                                                                                                    free_variables();
                                                                                                    free_functions_definitions();
                                                                                                    exit(-1);
                                                                                                }
                                                                                                node_list * param_list = create_node_list($3, PARAM_NODE_LIST);
                                                                                                $$ = (node *) create_function_node(CIRCLE_TYPE, "create_circle", (node *) param_list );
                                                                                            }
            |   CREATE_R OPEN_PARENTHESES param COMA param CLOSE_PARENTHESES                {
                                                                                                if(!check_parameter_type($1, get_node_data_type($3), 0) || !check_parameter_type($1, get_node_data_type($5), 1)){
                                                                                                    fprintf(stderr, "Error. Invalid function parameter type on function %s\n", $1);
                                                                                                    free_variables();
                                                                                                    free_functions_definitions();
                                                                                                    exit(-1);
                                                                                                }
                                                                                                node_list * param_list = create_node_list($5, PARAM_NODE_LIST);
                                                                                                param_list = add_node_list((node*)param_list, $3, PARAM_NODE_LIST);
                                                                                                $$ = (node *) create_function_node(RECTANGLE_TYPE, "create_rectangle", (node *) param_list );
                                                                                            }
            |   CREATE_T OPEN_PARENTHESES param COMA param COMA param CLOSE_PARENTHESES     {
                                                                                                if(!check_parameter_type($1, get_node_data_type($3), 0) || !check_parameter_type($1, get_node_data_type($5), 1) || !check_parameter_type($1, get_node_data_type($7), 2)){
                                                                                                    fprintf(stderr, "Error. Invalid function parameter type on function %s\n", $1);
                                                                                                    free_variables();
                                                                                                    free_functions_definitions();
                                                                                                    exit(-1);
                                                                                                }
                                                                                                node_list * param_list = create_node_list($7, PARAM_NODE_LIST);
                                                                                                param_list = add_node_list((node*)param_list, $5, PARAM_NODE_LIST);
                                                                                                param_list = add_node_list((node*)param_list, $3, PARAM_NODE_LIST);
                                                                                                $$ = (node *) create_function_node(TRIANGLE_TYPE, "create_triangle", (node *) param_list);}
            |   PERIMETER OPEN_PARENTHESES factor CLOSE_PARENTHESES                         {
                                                                                                if(!check_parameter_type($1, get_node_data_type($3), 0)){
                                                                                                    fprintf(stderr, "Error. Invalid function parameter type on function %s\n", $1);
                                                                                                    free_variables();
                                                                                                    free_functions_definitions();
                                                                                                    exit(-1);
                                                                                                }
                                                                                                node_list * param_list = create_node_list($3, PARAM_NODE_LIST);
                                                                                                $$ = (node *) create_function_node(INT_TYPE, "get_perimeter", (node *) param_list );
                                                                                            }
            |   AREA OPEN_PARENTHESES factor CLOSE_PARENTHESES                              {
                                                                                                if(!check_parameter_type($1, get_node_data_type($3), 0)){
                                                                                                    fprintf(stderr, "Error. Invalid function parameter type on function %s\n", $1);
                                                                                                    free_variables();
                                                                                                    free_functions_definitions();
                                                                                                    exit(-1);
                                                                                                }
                                                                                                node_list * param_list = create_node_list($3, PARAM_NODE_LIST);
                                                                                                $$ = (node *) create_function_node(INT_TYPE, "get_area", (node *) param_list );
                                                                                            }
            ;

    figure_property :   IDENTIFIER ARROW property   {
                                                        if(!check_figure_property($1,$3)){
                                                            fprintf(stderr, "Error. Variable %s not declared or invalid property\n", $1);
                                                            free_variables();
                                                            exit(-1);
                                                        }
                                                        $$ = (node*) create_property_node($1, $3);
                                                            
                                                    }
                    ;

    property        :   RADIUS  {$$ = RADIUS_TYPE;}
                    |   SIDE_1  {$$ = SIDE_1_TYPE;}
                    |   SIDE_2  {$$ = SIDE_2_TYPE;}
                    |   SIDE_3  {$$ = SIDE_3_TYPE;}
                    |   BASE    {$$ = BASE_TYPE;}
                    |   HEIGHT  {$$ = HEIGHT_TYPE;}
                    ;

%%

//C Routines

int yywrap(){
    return 1;
}

void yyerror(node_list ** node_list_param, char const * s){
    fprintf(stderr, "%s, at line %d\n", s, yylineno);
    free_variables();
    free_functions_definitions();
    exit(-1);
}

void create_variable(data_type type, char * name){
    current_variable_node->next = malloc(sizeof(variable_list_node));
    current_variable_node->next->name = malloc(strlen(name) + 1);
    strcpy(current_variable_node->next->name, name);
    current_variable_node->next->type = type;
    current_variable_node->next->next = NULL;
    current_variable_node = current_variable_node->next;
}

bool check_variable(char * name){
    variable_list_node * aux_node = variable_header->next;
    bool found = false;

    while(aux_node != NULL){

        if(strcmp(aux_node->name, name) == 0){
            found = true;
            break;
        }
        aux_node = aux_node->next;
    }

    return found; 
}

bool check_figure_property (char * name, figure_property_type property_type){
    variable_list_node * variable = find_variable(name);
    if(variable == NULL)
        return false;
    if(!is_figure(variable))
        return false;
    return figure_has_property(variable,property_type);
    
}

bool is_figure (variable_list_node * variable){
    switch(variable->type){
        case CIRCLE_TYPE:
        case RECTANGLE_TYPE:
        case TRIANGLE_TYPE:
            return true;
        default:
            return false;
    }
    return false;
}

bool figure_has_property(variable_list_node * variable, figure_property_type property_type){
    switch(variable->type){
        case CIRCLE_TYPE:
            return property_type == RADIUS_TYPE;
        case RECTANGLE_TYPE:
            return property_type == BASE_TYPE || property_type == HEIGHT_TYPE;
        case TRIANGLE_TYPE:
            return property_type == SIDE_1_TYPE || property_type == SIDE_2_TYPE || property_type == SIDE_3_TYPE;
        default:
            return false;
    }
    return false;
}



variable_list_node * find_variable(char * name){
    variable_list_node * aux_node = variable_header->next;
    bool found = false;

    while(aux_node != NULL){

        if(strcmp(aux_node->name, name) == 0){
            found = true;
            break;
        }
        aux_node = aux_node->next;
    }

    if (!found)
        aux_node = NULL;
    return aux_node; 
}

void free_variables(){
    current_variable_node = variable_header->next;
    variable_list_node * aux_node;

    while(current_variable_node != NULL){
        aux_node = current_variable_node->next;
        free(current_variable_node->name);
        free(current_variable_node);
        current_variable_node = aux_node;
    }

    free(variable_header);
}

bool check_parameter_type(char* function_name, data_type type, int param_index){
    int i = 0;
    while(i < STANDARD_FUNCTIONS && strcmp(functions_list[i]->name, function_name) != 0){
        i++;
    }

    data_type expected = functions_list[i]->params_type[param_index];

    if(i < STANDARD_FUNCTIONS && param_index < functions_list[i]->total_params){
        if(expected == type || (expected == FIGURE_TYPE && (type == CIRCLE_TYPE || type == RECTANGLE_TYPE || type == TRIANGLE_TYPE)))
            return true;
    }
    return false;
}

void free_functions_definitions(){
    int i = 0;
    while(i < STANDARD_FUNCTIONS){
        free(functions_list[i]);
        i++;
    }
}

void fill_functions_definitions(){
    int i = 0;

    functions_list[i] = malloc(sizeof(function_definition));
    functions_list[i]->name = "create_circle";
    functions_list[i]->params_type[0] = INT_TYPE;
    functions_list[i]->total_params = 1;
    i++;
    functions_list[i] = malloc(sizeof(function_definition));
    functions_list[i]->name = "create_rectangle";
    functions_list[i]->params_type[0] = INT_TYPE;
    functions_list[i]->params_type[1] = INT_TYPE;
    functions_list[i]->total_params = 2; 
    i++;
    functions_list[i] = malloc(sizeof(function_definition));
    functions_list[i]->name = "create_triangle";
    functions_list[i]->params_type[0] = INT_TYPE;
    functions_list[i]->params_type[1] = INT_TYPE;
    functions_list[i]->params_type[2] = INT_TYPE;
    functions_list[i]->total_params = 3;
    i++;
    
    functions_list[i] = malloc(sizeof(function_definition));
    functions_list[i]->name = "get_perimeter";
    functions_list[i]->params_type[0] = FIGURE_TYPE;
    functions_list[i]->total_params = 1;
    i++;
    
    functions_list[i] = malloc(sizeof(function_definition));
    functions_list[i]->name = "get_area";
    functions_list[i]->params_type[0] = FIGURE_TYPE;
    functions_list[i]->total_params = 1;
    i++;
    
    if (STANDARD_FUNCTIONS != i){
        fprintf(stderr, "Error filling function definitions\n");
        exit(-1);
    }
}

int main(int argc, char * argv[]){
    variable_header = malloc(sizeof(variable_list_node));
    variable_header->name = NULL;
    variable_header->type = EMPTY;
    variable_header->next = NULL;
    current_variable_node = variable_header;
    fill_functions_definitions();
    struct node_list * program_list;
    yyparse(&program_list);
    //printf("Program list pointer : %p\n", program_list);
    printf("#include <stdio.h>\n");
    printf("#include <math.h>\n");
    printf("#include <stdlib.h>\n");
    printf("#include <string.h>\n");
    print_initial_functions();
    printf("int main(int argc, char * argv[]) { \n");
    char * program = translate_to_c(program_list);
    printf("%s\n", program);
    printf("return 0;\n");
    printf("}\n");
    free_variables();
    free_functions_definitions();
    free_node((node *) program_list);
    free(program);
    return 0;
}

