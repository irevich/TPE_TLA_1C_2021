%{
    //Declarations

    #include <stdio.h>
    #include <string.h>

    int yylex();

    void yyerror(char const * s);
    
%}

//Definitions

//Tokens

%union{
    char * string;
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

//Instructuion delimeter token
%token SEMICOLON;

//Aritmethical tokens
%token PLUS;
%token MINUS;
%token PRODUCT;
%token DIVISION;

//Assign token
%token ASSIGN;

//Relational tokens
%token GT;
%token LT;
%token GE;
%token LE;

//Logical tokens
%token AND;
%token OR;
%token EQ;
%token NEQ;

//Datatypes tokens
%token INT; 
%token STRING;
%token CIRCLE;
%token RECTANGLE;
%token TRIANGLE;

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
%token<string> NUM;


//Non-Terminals
%type<string> param;
%type<string> exp;
%type<string> term;
%type<string> factor;

//Starting rule
%start program 


//Productions
%%

    program     :   START code END      {;}
                ;           
 
    code        :   instruction code        {;}
                |             {;}
                ;

    instruction :   declaration SEMICOLON       {;}
                |   assignation SEMICOLON       {;}
                |   PRINT OPEN_PARENTHESES QM param QM CLOSE_PARENTHESES SEMICOLON     {printf("printf(\"%%d\",%s);\n",$4);}
                ;

    declaration :   INT IDENTIFIER ASSIGN param  {printf("int %s = %s;\n",$2,$4);}
                ;

    assignation :   IDENTIFIER ASSIGN param     {printf("%s = %s;\n",$1,$3);}
                ;

    param       :   exp         {$$ = $1;}
                ;

    exp         :   exp PLUS term       {
                                            $$ = malloc(strlen($1)+strlen($3)+4);
                                            sprintf($$, "%s + %s", $1, $3);
                                        }
                |   exp MINUS term      {
                                            $$ = malloc(strlen($1)+strlen($3)+4);
                                            sprintf($$, "%s - %s", $1, $3);
                                        }
                |   term                {$$ = $1;}
                ;

    term        :   term PRODUCT factor     {
                                            $$ = malloc(strlen($1)+strlen($3)+4);
                                            sprintf($$, "%s * %s", $1, $3);
                                            }
                |   term DIVISION factor    {
                                            $$ = malloc(strlen($1)+strlen($3)+4);
                                            sprintf($$, "%s / %s", $1, $3);
                                            }
                |   factor                  {$$ = $1;}
                ;

    factor      :   IDENTIFIER      {$$ = $1;}
                |   NUM             {$$ = $1;}
                ; 

%%

//C Routines

int yywrap(){
    return 1;
}

void yyerror(char const * s){
    fprintf(stderr, "%s\n", s);
}

int main(int argc, char * argv[]){
    printf("#include <stdio.h>\n");
    printf("int main(int argc, char * argv[]) { \n");
    yyparse();
    printf("return 0;\n");
    printf("}\n");
    return 0;
}

