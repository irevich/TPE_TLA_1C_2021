%{
    //C declarations

    #include "node_definitions.h"
    #include "y.tab.h"

%}

%option nounput
%option noinput

 //Definitions Section

letter [a-zA-Z]
digit  [0-9]
string \"[^\"]*\"

%%

 //Rules section


"start"                         return START;
"end"                           return END;
";"                             return SEMICOLON;
"repeat while"                  return WHILE;
"if"                            return IF;
"otherwise"                     return OTHERWISE;
"print"                         return PRINT;
"read_num"                        return READ_NUM;
"("				                return OPEN_PARENTHESES;
")"				                return CLOSE_PARENTHESES;
"{"				                return OPEN_BRACES;
"}"				                return CLOSE_BRACES;
","                             return COMA;
"="                             return ASSIGN;
">"				                return GT;
">="			                return GE;
"<"				                return LT;
"<="			                return LE;
"and"			                return AND;
"or"			                return OR;
"eq"			                return EQ;
"not_eq"		                return NEQ;
"+"                             return PLUS;
"-"                             return MINUS;
"*"                             return PRODUCT;
"/"                             return DIVISION;
"int"                           return INT; 
"string"                        return STRING;
"circle"                        return CIRCLE;
"rectangle"                     return RECTANGLE;
"triangle"                      return TRIANGLE;
"->"                            return ARROW;
"radius"                        return RADIUS;
"side_1"                        return SIDE_1;
"side_2"                        return SIDE_2;
"side_3"                        return SIDE_3;
"base"                          return BASE;
"height"                        return HEIGHT;
"create_circle"                 {strcpy(yylval.string, yytext); return CREATE_C;}
"create_rectangle"              {strcpy(yylval.string, yytext); return CREATE_R;}
"create_triangle"               {strcpy(yylval.string, yytext); return CREATE_T;}
"get_perimeter"                 {strcpy(yylval.string, yytext); return PERIMETER;}
"get_area"                      {strcpy(yylval.string, yytext); return AREA;}
\"                              return QM;
{letter}({letter}|{digit}|[_])* {strcpy(yylval.string, yytext); return IDENTIFIER;}
{string}                       {strcpy(yylval.string, yytext); return TEXT;}
{digit}+                       {yylval.num = atoi(yytext); return NUM;}

\n              {yylineno++;}
[ \t\r]+		;		                //Ignore white space, syntax error for any other word we don't support
.               return SYNTAX_ERROR;
%%

//User code section
 