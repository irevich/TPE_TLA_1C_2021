# TPE_TLA_1C_2021

INSTRUCCIONES DE COMPILACION Y EJECUCION DE PROGRAMAS EN LENGUAJE GEOME

Para compilar y ejecutar un programa en lenguaje "geome", luego de clonarse el repositorio de github https://github.com/irevich/TPE_TLA_1C_2021, adentro de la carpeta del repositorio, y en el mismo directorio donde se encuentra el archivo Makefile, deben ejecutarse las instrucciones :

    make clean

    make all

Se generara el archivo "compiler" que es el ejecutable correspondiente al compilador, junto con los .c y .h generados por Lex y Yacc. Una vez obtenido dicho archivo, para compilar un programa .geome y ejecutarlo se deben realizar las siguientes instrucciones dentro del mismo directorio donde se encuentra el archivo ejecutable del compilador :

./compiler < path_del_programa.geome/programa.geome > path_destino_c/programa.c (Compilacion de Geome a C)


gcc path_destino_c/programa.c -o path_ejecutable/program -lm 

(Generacion del programa ejecutable a partir del .c ) . 

En esta ultima instruccion es de suma importancia utilizar el flag "-lm" al final, dado que el lenguaje Geome utiliza internamente librerias matematicas de C para distintas funciones, con lo cual se requiere dicho flag para el correcto funcionamiento del mismo.


./path_ejecutable/program  (Ejecucion del programa)


INSTRUCCIONES DE COMPILACION Y EJECUCION DE LOS PROGRAMAS DE PRUEBA (test1.geome, test2.geome, test3.geome, test4.geome y test5.geome)

Para compilar y ejecutar los casos de prueba otorgados que se encuentran en la carpeta "tests", se deben ejecutar las siguientes instrucciones adentro de la carpeta del repositorio y en el mismo directorio que el Makefile :

make clean_test

make test (Para esta instruccion se requiere previamente haber realizado un "make all")

Luego de esta ultima instruccion, en la carpeta "generated_tests" se generaran los tests traducidos a lenguaje .c (test1.c, test2.c , test3.c, test4.c y test5.c), y en la carpeta "executable_tests" se generaran los ejecutables de dichos programas (test1, test2, test3, test4 y test5)

Finalmente, para ejecutar los mismos se debe dirigir a la carpeta de los tests ejecutables mediante la instruccion :

cd executables_tests

Y alli ejecutar la instruccion :

./test_ejecutable

