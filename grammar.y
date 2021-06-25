%{
    //Declarations

    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "node.h"
    #include "translator.h"

    int yylex();

    void yyerror(node_list ** node_list_param, char const * s);

    typedef struct variable_list_node{
        data_type type;
        char * name;
        struct variable_list_node * next; 
    }variable_list_node;

    variable_list_node * variable_header;
    variable_list_node * current_variable_node;

    bool check_variable(char * name);

    void create_variable(data_type type, char * name);

    variable_list_node * find_variable(char * name);

    void free_variables();
    
%}

//Definitions

//Tokens

%union{
    char string[500];
    int num;
    struct node * node;
    struct node_list * node_list;
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

//Figure property token
%token ARROW;

//Rectangle properties tokens
%token BASE;
%token HEIGHT;

//Rectangle functions tokens
%token CREATE_R;
%token PERIMETER_R;
%token AREA_R;

//Triangle properties tokens
%token SIDE_1;
%token SIDE_2;
%token SIDE_3;

//Triangle functions tokens
%token CREATE_T;
%token PERIMETER_T;
%token AREA_T;

//Circle properties tokens
%token RADIUS;

//Circle functions tokens
%token CREATE_C;
%token PERIMETER_C;
%token AREA_C;

//General functions tokens

%token PRINT;

//Identifier token
%token<string> IDENTIFIER;

//Num token
%token<num> NUM;


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



//Starting rule
%start program

%parse-param{struct node_list ** program_list}


//Productions
%%

    program     :   START code END      { *program_list = (node_list *)$2; $$ = *program_list;}
                ;           
 
    code        :   instruction code        { $$ = (node *) add_node_list($2, $1);}
                |   instruction             {$$ = (node *) create_node_list($1);}
                ;    

    instruction :   declaration SEMICOLON       {;}
                |   assignation SEMICOLON       {;}
                |   if_block                    {;}
                |   while_block                 {;}
                |   PRINT OPEN_PARENTHESES QM param QM CLOSE_PARENTHESES SEMICOLON     {$$ = (node*) create_print_node($4);}
                ;

    if_block    :   IF OPEN_PARENTHESES comp CLOSE_PARENTHESES OPEN_BRACES code CLOSE_BRACES                                            {$$ = (node*) create_if_node($3, $6);}
                |   IF OPEN_PARENTHESES comp CLOSE_PARENTHESES OPEN_BRACES code CLOSE_BRACES OTHERWISE OPEN_BRACES code CLOSE_BRACES    {$$ = (node*) create_if_otherwise_node($3, $6, $10);}
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

    
    declaration :   INT IDENTIFIER ASSIGN param  {                  
                    if(check_variable($2)){
                        fprintf(stderr, "Error. Variable %s already declared\n", $2);
                        free_variables();
                        exit(-1);
                    }
                    create_variable(INT_TYPE, $2);
                    $$ = (node*) create_declaration_node($2, INT_TYPE, $4);                  
                    }
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
                ; 

%%

//C Routines

int yywrap(){
    return 1;
}

void yyerror(node_list ** node_list_param, char const * s){
    fprintf(stderr, "%s\n", s);
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

int main(int argc, char * argv[]){
    variable_header = malloc(sizeof(variable_list_node));
    variable_header->name = NULL;
    variable_header->type = EMPTY;
    variable_header->next = NULL;
    current_variable_node = variable_header;
    struct node_list * program_list;
    yyparse(&program_list);
    //printf("Program list pointer : %p\n", program_list);
    printf("#include <stdio.h>\n");
    printf("int main(int argc, char * argv[]) { \n");
    char * program = translate_to_c(program_list);
    printf("%s\n", program);
    printf("return 0;\n");
    printf("}\n");
    free_variables();
    free_node((node *) program_list);
    free(program);
    return 0;
}

