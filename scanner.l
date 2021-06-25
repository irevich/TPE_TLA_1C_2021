%{
    //C declarations

    #include "y.tab.h"

%}

 //Definitions Section

letter [a-zA-Z]
digit  [0-9]

%%

 //Rules section


"start"                         return START;
"end"                           return END;
";"                             return SEMICOLON;
"repeat while"                  return WHILE;
"if"                            return IF;
"otherwise"                     return OTHERWISE;
"print"                         return PRINT;
"("				                return OPEN_PARENTHESES;
")"				                return CLOSE_PARENTHESES;
"{"				                return OPEN_BRACES;
"}"				                return CLOSE_BRACES;
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
"create_circle"                 return CREATE_C;
"create_rectangle"              return CREATE_R;
"create_triangle"               return CREATE_T;
"get_circle_perimeter"          return PERIMETER_C;
"get_rectangle_perimeter"       return PERIMETER_R;
"get_triangle_perimeter"        return PERIMETER_T;
"get_circle_area"               return AREA_C;
"get_rectangle_area"            return AREA_R;
"get_triangle_area"             return AREA_T;
\"                              return QM;
{letter}({letter}|{digit}|[_])* {strcpy(yylval.string, yytext); return IDENTIFIER;}
(-)?{digit}+                    {yylval.num = atoi(yytext); return NUM;}


[ \t\r\n]+		;		//Ignore white space

%%

//User code section
 