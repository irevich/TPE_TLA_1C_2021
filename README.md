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

mkdir generated_tests (En caso de no tenerla, crear esta carpeta para guardar los .c generados por los tests)

mkdir executable_tests (En caso de no tenerla, crear esta carpeta para guardar los ejecutables generados a partir de los .c recientemente mencionados)

make clean_test

make test (Para esta instruccion se requiere previamente haber realizado un "make all")

Luego de esta ultima instruccion, en la carpeta "generated_tests" se generaran los tests traducidos a lenguaje .c (test1.c, test2.c , test3.c, test4.c y test5.c), y en la carpeta "executable_tests" se generaran los ejecutables de dichos programas (test1, test2, test3, test4 y test5)

Finalmente, para ejecutar los mismos se debe dirigir a la carpeta de los tests ejecutables mediante la instruccion :

cd executables_tests

Y alli ejecutar la instruccion :

./test_ejecutable

DESCRIPCION DE LOS TESTS

test1.geome : 
    - Crea un triangulo, un rectangulo y un circulo, y se imprimen sus propiedades
    - Verifica con un bloque condicional si alguno de los lados del triangulo es mayor o igual que el radio del circulo y que la base del rectangulo sea menor que la altura del mismo
    - Con un bloque de "repeat while" se incrementa el radio en 10

test2.geome : 
    - Crea un triangulo, un rectangulo y un circulo, y se imprimen sus propiedades
    - Imprime los perimetros y areas de cada uno

test3.geome : 
    - Crea 2 triangulos no iguales, 2 rectangulos iguales y 2 circulos no iguales, y se imprimen sus propiedades
    - Se compara cada dupla de figuras entre si
    - Se igualan los circulos y se cambian las propiedades de los triangulos para que queden iguales
    - Se vuelven a comparar los circulos y los triangulos, y esta vez se indica que son iguales

test4.geome :

    - Se calcula el factorial de la variable int "num" en caso de ser un valor mayor o igual a 0. Caso contrario se indica que no es posible el calculo

test5.geome :

    - Se pide el ingreso por entrada estandar de un numero para que sea el radio de un circulo. Si el numero es negativo corta el test, sino se realiza el mismo proceso para la base y la altura de un rectangulo ( Tener en cuenta que en caso de ingresarse un dato no numerico, como las variables donde se almacenan los datos ingresados estan inicializadas con 0, estas permaneceran con el mismo valor y por ende el test terminara como si se hubiera ingresado un numero negativo )
    - Con los datos correctos, se crean el circulo y el rectangulo correspondientes, y se imprimen sus propiedades.
    - Mediante bloques condicionales anidados, se checkea si el perimetro de alguna de las figuras es mayor o igual al promedio de los perimetros. De ser asi, se imprime que figura fue con el perimetro correspondiente y el promedio, y sino se imprime que ninguna de las figuras cumple dicha condicion y se imprime el promedio correspondiente.

