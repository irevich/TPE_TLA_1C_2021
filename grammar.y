%{
    //Declarations

    #include "node.h"
    #include "translator.h"
    #include "stdbool.h"
    #include "utils.h"
    
    #define MAX_PARAMS 10
    #define STANDARD_FUNCTIONS 5

    int yylex();

    int extern yylineno;

    //Prints error message and line where error was found
    void yyerror(node_list ** node_list_param, char const * s);

    //To store user defined variables
    typedef struct variable_list_node{
        data_type type;
        char * name;
        struct variable_list_node * next; 
    }variable_list_node;

    //To check scope when declaring and using variables
    typedef struct scope_list_node{
        struct variable_list_node * variables_list;
        struct scope_list_node * next;
        struct scope_list_node * prev;
    }scope_list_node;

    //To store defined functions
    typedef struct function_defintion{
        char * name;
        int total_params;
        data_type params_type[MAX_PARAMS];
    }function_definition;

    function_definition * functions_list[STANDARD_FUNCTIONS];

    scope_list_node * scope_list_header;
    scope_list_node * current_scope_list_node;
    
    void push_scope();

    void pop_scope();

    void create_variable(data_type type, char * name);

    variable_list_node * find_variable(char * name);
    
    variable_list_node * find_variable_in_scope(char * name, variable_list_node * variable_header);

    bool is_figure (variable_node * variable);

    bool check_node_aritmethic_expression(node * node);
    
    bool check_figure_property (node * var_node, figure_property_type property_type);

    bool figure_has_property(variable_node * variable, figure_property_type property_type);

    void free_variables(variable_list_node * variable_header);

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
%token READ_NUM;

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
%type<node> variable;  



//Starting rule
%start program

%parse-param{node_list ** program_list}

%right ASSIGN
%left PLUS MINUS
%left PRODUCT DIVISION
%left LT LE GE GT


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
                |   READ_NUM OPEN_PARENTHESES variable CLOSE_PARENTHESES SEMICOLON  {   if(get_node_data_type($3) != INT_TYPE){
                                                                                            char * error_message = malloc(strlen("Error. Invalid variable data type on read_num") + 1);
                                                                                            sprintf(error_message, "Error. Invalid variable data type on read_num");
                                                                                            yyerror(NULL, error_message);
                                                                                        }
                                                                                        
                                                                                        $$ = (node*) create_read_num_node($3);                                                                                      
                                                                                    }
                ;

    if_block    :   IF {push_scope();} OPEN_PARENTHESES comp CLOSE_PARENTHESES OPEN_BRACES code CLOSE_BRACES {pop_scope();} otherwise_block    {  
                                                                                                                            
                                                                                                                                    if($10 == NULL){
                                                                                                                                        $$ = (node*) create_if_node($4, $7);
                                                                                                                                    } 
                                                                                                                                    else {
                                                                                                                                        $$ = (node*) create_if_otherwise_node($4, $7, $10);
                                                                                                                                    }
                                                                                                                                
                                                                                                                }
                ;

    otherwise_block : OTHERWISE {push_scope();} OPEN_BRACES code CLOSE_BRACES   {
                                                                                    $$ = $4;
                                                                                    pop_scope();
                                                                                }
                    |                                                           {$$ = NULL;}
                    ;

    while_block :   WHILE {push_scope();} OPEN_PARENTHESES comp CLOSE_PARENTHESES OPEN_BRACES code CLOSE_BRACES         {
                                                                                                                             $$ = (node*) create_while_node($4, $7);
                                                                                                                             pop_scope();
                                                                                                                        }

    comp        :   comp OR comp_term       {   $$ = (node*) create_logical_comp_node("||",$1, $3);  }
                |   comp_term               {   $$ = $1;    }
                ;

    comp_term   :   comp_term AND comp_factor    {   $$ = (node*) create_logical_comp_node("&&",$1, $3);  }
                |   comp_factor                  {   $$ = $1;   }
                ;

    comp_factor :   exp or exp              {   
                                                    
                                                    $$ = (node *) create_relational_comp_node($2,$1,$3);

                                                    if($$ == NULL){
                                                        char * error_message = malloc(strlen("Error. Invalid relational expression") + 1);
                                                        sprintf(error_message, "Error. Invalid relational expression");
                                                        yyerror(NULL, error_message);
                                                    }

                                            }
                | OPEN_PARENTHESES comp CLOSE_PARENTHESES    { 
                    $$ = (node *) create_parentheses_exp_node($2);
                    }
                ; 
    
    or          :   LT          { strcpy($$, "<");}
                |   LE          { strcpy($$, "<=");}
                |   GT          { strcpy($$, ">");}
                |   GE          { strcpy($$, ">=");}
                |   EQ          { strcpy($$, "==");}
                |   NEQ         { strcpy($$, "!=");}
                ;

    
    declaration :   var_type IDENTIFIER ASSIGN param  {                  
                    if(find_variable($2) != NULL){
                        char * error_message = malloc(strlen("Error. Variable %s already declared") + strlen($2) + 1);
                        sprintf(error_message, "Error. Variable %s already declared", $2);
                        yyerror(NULL, error_message);
                    }
                    create_variable($1, $2);
                    $$ = (node*) create_declaration_node($2,$1, $4);
                        if($$ == NULL){
                            char * error_message = malloc(strlen("Error. Declaration of variable %s with incorrect data type") + strlen($2) + 1);
                            sprintf(error_message, "Error. Declaration of variable %s with incorrect data type", $2);
                            yyerror(NULL, error_message);
                        }                    
                    }
                ;

    var_type   :   INT     { $$ = INT_TYPE;}
               |   STRING  { $$ = STRING_TYPE;}
               |   CIRCLE  { $$ = CIRCLE_TYPE;}
               |   RECTANGLE { $$ = RECTANGLE_TYPE;}
               |   TRIANGLE  { $$ = TRIANGLE_TYPE;}
               ;

    assignation :   variable ASSIGN param   {
                                                $$ = (node *) create_assignation_node($1, $3);
                                                if($$ == NULL){
                                                    char * error_message = malloc(strlen("Error. Assignation of variable %s with incorrect data type\n") + strlen(((variable_node *)($1))->name) + 1);
                                                    sprintf(error_message, "Error. Assignation of variable %s with incorrect data type", ((variable_node *)($1))->name);
                                                    yyerror(NULL, error_message);
                                                }
                                            }

                |   figure_property ASSIGN param    {
                                                        $$ = (node *) create_assignation_node($1, $3);
                                                        if($$ == NULL){
                                                            char * error_message = malloc(strlen("Error. Assignation of variable %s with incorrect data type\n") + strlen((((variable_node *)((property_node *)($1))->var_node))->name) + 1);
                                                            sprintf(error_message, "Error. Assignation of variable %s with incorrect data type", (((variable_node *)((property_node *)($1))->var_node))->name);
                                                            yyerror(NULL, error_message);
                                                        }
                                                    
                                                    }
                ;


    param       :   exp         {$$ = $1;}
                |   TEXT        {$$ = (node *) create_constant_string_node($1);}
                ;
    

    exp         :   exp PLUS term       {   
                                            node * term_node = (node *) $3;
                                            node * exp_node = (node *) $1;
                                            if(get_node_data_type(exp_node)!= INT_TYPE || get_node_data_type(term_node)!= INT_TYPE ){
                                                char * error_message = malloc(strlen("Error. Invalid aritmethic expression") + 1);
                                                sprintf(error_message, "Error. Invalid aritmethic expression");
                                                yyerror(NULL, error_message);
                                            }
                                            $$ = (node*) create_exp_node("+",$1,$3);  
                                        }
                |   exp MINUS term      {   
                                            node * term_node = (node *) $3;
                                            node * exp_node = (node *) $1;
                                            if(get_node_data_type(exp_node)!= INT_TYPE || get_node_data_type(term_node)!= INT_TYPE ){
                                                char * error_message = malloc(strlen("Error. Invalid aritmethic expression") + 1);
                                                sprintf(error_message, "Error. Invalid aritmethic expression");
                                                yyerror(NULL, error_message);
                                            }
                                            $$ = (node*) create_exp_node("-",$1,$3);  
                                        }
                |   term                {$$ = $1;}
                ;

    term        :   term PRODUCT factor     {   node * factor_node = (node *) $3;
                                                node * term_node = (node *) $1;
                                                if(get_node_data_type(factor_node)!= INT_TYPE || get_node_data_type(term_node)!= INT_TYPE ){
                                                    char * error_message = malloc(strlen("Error. Invalid aritmethic expression") + 1);
                                                    sprintf(error_message, "Error. Invalid aritmethic expression");
                                                    yyerror(NULL, error_message);
                                                }
                                                $$ = (node*) create_exp_node("*",$1,$3);  
                                            }
                |   term DIVISION factor    {   
                                                node * factor_node = (node *) $3;
                                                node * term_node = (node *) $1;
                                                if(get_node_data_type(factor_node)!= INT_TYPE || get_node_data_type(term_node)!= INT_TYPE ){
                                                    char * error_message = malloc(strlen("Error. Invalid aritmethic expression") + 1);
                                                    sprintf(error_message, "Error. Invalid aritmethic expression");
                                                    yyerror(NULL, error_message);
                                                }
                                                $$ = (node*) create_exp_node("/",$1,$3);  
                                            }
                |   factor                  { $$ = $1; }
                ;

    factor      :   variable      {$$ = $1;}
                |   NUM             {$$ = (node*)create_constant_int_node($1);}
                |   MINUS NUM       {$$ = (node*)create_constant_int_node($2 * (-1));}
                |   figure_property {$$ =  $1;}
                |   func            {$$ = $1;}
                | OPEN_PARENTHESES exp CLOSE_PARENTHESES    { $$ = (node *) create_parentheses_exp_node($2);}
                ;

    func    :   CREATE_C OPEN_PARENTHESES param CLOSE_PARENTHESES                           {   
                                                                                                if(!check_parameter_type($1, get_node_data_type($3), 0)){
                                                                                                    char * error_message = malloc(strlen("Error. Invalid function parameter type on function %s") + strlen($1) + 1);
                                                                                                    sprintf(error_message, "Error. Invalid function parameter type on function %s", $1);
                                                                                                    yyerror(NULL, error_message);
                                                                                                }
                                                                                                node_list * param_list = create_node_list($3, PARAM_NODE_LIST);
                                                                                                $$ = (node *) create_function_node(CIRCLE_TYPE, "create_circle", (node *) param_list );
                                                                                            }
            |   CREATE_R OPEN_PARENTHESES param COMA param CLOSE_PARENTHESES                {
                                                                                                if(!check_parameter_type($1, get_node_data_type($3), 0) || !check_parameter_type($1, get_node_data_type($5), 1)){
                                                                                                    char * error_message = malloc(strlen("Error. Invalid function parameter type on function %s") + strlen($1) + 1);
                                                                                                    sprintf(error_message, "Error. Invalid function parameter type on function %s", $1);
                                                                                                    yyerror(NULL, error_message);
                                                                                                }
                                                                                                node_list * param_list = create_node_list($5, PARAM_NODE_LIST);
                                                                                                param_list = add_node_list((node*)param_list, $3, PARAM_NODE_LIST);
                                                                                                $$ = (node *) create_function_node(RECTANGLE_TYPE, "create_rectangle", (node *) param_list );
                                                                                            }
            |   CREATE_T OPEN_PARENTHESES param COMA param COMA param CLOSE_PARENTHESES     {
                                                                                                if(!check_parameter_type($1, get_node_data_type($3), 0) || !check_parameter_type($1, get_node_data_type($5), 1) || !check_parameter_type($1, get_node_data_type($7), 2)){
                                                                                                    char * error_message = malloc(strlen("Error. Invalid function parameter type on function %s") + strlen($1) + 1);
                                                                                                    sprintf(error_message, "Error. Invalid function parameter type on function %s", $1);
                                                                                                    yyerror(NULL, error_message);
                                                                                                }
                                                                                                node_list * param_list = create_node_list($7, PARAM_NODE_LIST);
                                                                                                param_list = add_node_list((node*)param_list, $5, PARAM_NODE_LIST);
                                                                                                param_list = add_node_list((node*)param_list, $3, PARAM_NODE_LIST);
                                                                                                $$ = (node *) create_function_node(TRIANGLE_TYPE, "create_triangle", (node *) param_list);}
            |   PERIMETER OPEN_PARENTHESES factor CLOSE_PARENTHESES                         {
                                                                                                if(!check_parameter_type($1, get_node_data_type($3), 0)){
                                                                                                    char * error_message = malloc(strlen("Error. Invalid function parameter type on function %s") + strlen($1) + 1);
                                                                                                    sprintf(error_message, "Error. Invalid function parameter type on function %s", $1);
                                                                                                    yyerror(NULL, error_message);
                                                                                                }
                                                                                                node_list * param_list = create_node_list($3, PARAM_NODE_LIST);
                                                                                                $$ = (node *) create_function_node(INT_TYPE, "get_perimeter", (node *) param_list );
                                                                                            }
            |   AREA OPEN_PARENTHESES factor CLOSE_PARENTHESES                              {
                                                                                                if(!check_parameter_type($1, get_node_data_type($3), 0)){
                                                                                                    char * error_message = malloc(strlen("Error. Invalid function parameter type on function %s") + strlen($1) + 1);
                                                                                                    sprintf(error_message, "Error. Invalid function parameter type on function %s", $1);
                                                                                                    yyerror(NULL, error_message);
                                                                                                }
                                                                                                node_list * param_list = create_node_list($3, PARAM_NODE_LIST);
                                                                                                $$ = (node *) create_function_node(INT_TYPE, "get_area", (node *) param_list );
                                                                                            }
            ;

    figure_property :   variable ARROW property {
                                                    if(!check_figure_property($1,$3)){
                                                        char * error_message = malloc(strlen("Error. Invalid property\n") + 1);
                                                        sprintf(error_message, "Error. Invalid property");
                                                        yyerror(NULL, error_message);
                                                    }
                                                    $$ = (node*) create_property_node($1, $3);
                                                            
                                                }
                    ;

    property    :   RADIUS  {$$ = RADIUS_TYPE;}
                |   SIDE_1  {$$ = SIDE_1_TYPE;}
                |   SIDE_2  {$$ = SIDE_2_TYPE;}
                |   SIDE_3  {$$ = SIDE_3_TYPE;}
                |   BASE    {$$ = BASE_TYPE;}
                |   HEIGHT  {$$ = HEIGHT_TYPE;}
                ;
    
    variable:   IDENTIFIER  {
                                variable_list_node * variable = find_variable($1);
                                if(variable == NULL){
                                    char * error_message = malloc(strlen("Error. Variable %s not declared") + strlen($1) + 1);
                                    sprintf(error_message, "Error. Variable %s not declared", $1);
                                    yyerror(NULL, error_message);
                                }
                                $$ = (node*)create_variable_node(variable->type, $1);
                            }
                ;

%%

//C Routines

int yywrap(){
    return 1;
}

void yyerror(node_list ** node_list_param, char const * s){
    fprintf(stderr, "%s\n", s);
    fprintf(stderr, "Line: %d\n", yylineno);
    exit(-1);
}

void create_variable(data_type type, char * name){
    variable_list_node * variable_header = current_scope_list_node->variables_list;
    if(variable_header == NULL){
        current_scope_list_node->variables_list = malloc(sizeof(variable_list_node));
        current_scope_list_node->variables_list->name = malloc(strlen(name) + 1);
        strcpy(current_scope_list_node->variables_list->name, name);
        current_scope_list_node->variables_list->type = type;
        current_scope_list_node->variables_list->next = NULL;
    }
    else{
        variable_list_node * current_variable_node = variable_header;
        while(current_variable_node->next != NULL){
            current_variable_node = current_variable_node->next;
        }
        current_variable_node->next = malloc(sizeof(variable_list_node));
        current_variable_node->next->name = malloc(strlen(name) + 1);
        strcpy(current_variable_node->next->name, name);
        current_variable_node->next->type = type;
        current_variable_node->next->next = NULL;
    }
}

bool check_figure_property (node * var_node, figure_property_type property_type){
    if(!is_figure((variable_node *) var_node))
        return false;
    return figure_has_property((variable_node *) var_node,property_type); 
}

bool is_figure (variable_node * variable){
    switch(variable->variable_type){
        case CIRCLE_TYPE:
        case RECTANGLE_TYPE:
        case TRIANGLE_TYPE:
            return true;
        default:
            return false;
    }
    return false;
}

bool figure_has_property(variable_node * variable, figure_property_type property_type){
    switch(variable->variable_type){
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

    scope_list_node * aux_scope = scope_list_header;
    variable_list_node * ret_variable = NULL;

    while(aux_scope != NULL){
        ret_variable = find_variable_in_scope(name, aux_scope->variables_list);
        if(ret_variable != NULL){
            return ret_variable;
        }
        aux_scope = aux_scope->next;
    }

    return ret_variable;

}


variable_list_node * find_variable_in_scope(char * name, variable_list_node * variable_header){
    
    variable_list_node * aux_node = variable_header;
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

void free_variables(variable_list_node * variable_header){

    variable_list_node * current_variable_node = variable_header;
    variable_list_node * aux_node;

    while(current_variable_node != NULL){
        aux_node = current_variable_node->next;
        free(current_variable_node->name);
        free(current_variable_node);
        current_variable_node = aux_node;
    }

}

bool check_parameter_type(char* function_name, data_type type, int param_index){
    int i = 0;
    while(i < STANDARD_FUNCTIONS && strcmp(functions_list[i]->name, function_name) != 0){
        i++;
    }

    if(i < STANDARD_FUNCTIONS && param_index < functions_list[i]->total_params){
        data_type expected = functions_list[i]->params_type[param_index];
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

void push_scope(){

    scope_list_node * new_scope_list_node = malloc(sizeof(scope_list_node));
    new_scope_list_node->variables_list = NULL;
    new_scope_list_node->next = NULL;
    new_scope_list_node->prev = current_scope_list_node;
    current_scope_list_node->next = new_scope_list_node;
    current_scope_list_node = new_scope_list_node;
}

void pop_scope(){

    free_variables(current_scope_list_node->variables_list);

    scope_list_node * aux_prev = current_scope_list_node->prev;
    free(current_scope_list_node);
    
    current_scope_list_node = aux_prev;

    if(current_scope_list_node != NULL){
        current_scope_list_node->next = NULL;
    }
}

int main(int argc, char * argv[]){
    
    scope_list_header = malloc(sizeof(scope_list_node));
    scope_list_header->variables_list = NULL;
    scope_list_header->next = NULL;
    scope_list_header->prev = NULL;
    current_scope_list_node = scope_list_header;
    fill_functions_definitions();

    struct node_list * program_list;
    yyparse(&program_list);
    
    printf("#include <stdio.h>\n");
    printf("#include <math.h>\n");
    printf("#include <stdlib.h>\n");
    printf("#include <string.h>\n");
    print_initial_functions();
    printf("\n\n\nint main(int argc, char * argv[]) { \n");
    
    char * program = translate_to_c(program_list);
    
    printf("%s\n", program);
    printf("\nfree_alloqued_pointers();");
    printf("return 0;\n");
    printf("}\n\n");
    
    pop_scope();
    free_functions_definitions();
    
    free_node((node *) program_list);
    free(program);
    
    return 0;
}

