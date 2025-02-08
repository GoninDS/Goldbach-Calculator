# **Goldbach_serial**

### **Enunciado:**

Se planetea un programa que consiga las sumas de Goldbach de ciertos números insertados. La conjetura de Goldbach expresa lo siguiente:

Todo número entero mayor que 5:

1. Si es par se puede escribir como una suma de dos números primos (conjetura fuerte).

2. Si es impar se puede escribir como suma de tres números primos (conjetura débil).

Realice un programa que reciba los números en un rango de 0 a 2^63-1. En caso de insertar un número negativo, imprima las sumas de Goldbach encontradas para ese número junto a su cantidad. Si se inserta un número positivo, muestre únicamente la cantidad de sumas.

En caso de presentar un número fuera del rango o una cadena que no sea un número, detenga la ejecución de su programa. Si un número está en el rango de -5 a 5, debe mostrarse como NA. 

Se va a tomar el 4 como un caso especial debido a los tests que se encuentran. 

    -4: 1 sums: 2 + 2

Esto debido a los casos de prueba.

Puede usar el siguiente ejemplo de salida como guía de la salida:

    Total: 12 numbers 19 sums

    1: NA

    2: NA

    6: 1 sums

    7: 1 sums

    -8: 1 sums: 3 + 5

    -9: 2 sums: 2 + 2 + 5, 3 + 3 + 3

    10: 2 sums

    11: 2 sums

    12: 1 sums

    13: 2 sums

    -14: 2 sums: 3 + 11, 7 + 7

    -21: 5 sums: 2 + 2 + 17, 3 + 5 + 13, 3 + 7 + 11, 5 + 5 + 11, 7 + 7 + 7

Se debe imprimir la cantidad de números y la cantidad de sumas calculadas al inicio, por esta razón no se puede calcular las sumas de un número y soltarlas. Debe guardarlas en alguna estructura dinámica para poder consultarlas y mostrarlas al final. Las sumas deben seguir un orden de menor a mayor y no se deben de repetir.

 Es lo mismo decir 3 + 11 que 11 + 3 para representar 14.

Para la solución se propone la siguiente forma de trabajarlo:

1. Realizar una lectura de los datos y guardar los números en la estructura de datos respectiva. 

2. Una vez leído, encuentre el mayor número de los insertados y encuentre todo primo antes de él, para no tener que calcularos en cada pasada. Para realizar esto se recomienda utilizar algún algoritmo conocido para números primos, como el Sieve of Atkin implementado en esta solución.

3. Calcule las sumas y guárdelas en su estructura de datos, divida los casos en uno par y en uno impar para modularizar el código en procesos adaptados a cada caso.

4. Muestre los resultados obtenidos.

Para tener una representación más visual, consulte este [README](./design/README.md)


### **Manual de uso:**

Para la compilación del programa puede utilizar los siguientes comandos:

    make clean

    make

Ambos requieren tener el makefile en la carpeta common del repositorio y el makefile de esta misma carpeta que lo incluye.

El make clean se utiliza en caso de tener una versión anterior del programa.

Para la ejecución del programa tiene dos opciones. Puede redirigir la entrada por medio de un archivo o puede insertar manualmente los números y cuando desee terminar presionar CTRL + D.

Ejemplo de ejecución con entrada estándar (dentro de la carpeta bin):

    luisdss@luisdss-VirtualBox:~/concurrente22b-luis_solano/homework/goldback_serial/bin$ ./goldback_serial 

    2
    
    10

    -8

    Total: 3 numbers 3 sums

    2: NA

    10: 2 sums 

    -8: 1 sums: 3 + 5

Una vez insertado el -8, se presionó CTRL + D para indicarle al programa que calculara las sumas.

Ejemplo de redirección estándar:

    luisdss@luisdss-VirtualBox:~/concurrente22b-luis_solano/homework/goldback_serial$ ./bin/goldback_serial < test/input001.txt

    Total: 11 numbers 19 sums

    2: NA

    6: 1 sums 

    7: 1 sums 

    8: 1 sums 

    9: 2 sums 

    10: 2 sums 

    11: 2 sums 

    12: 1 sums 

    13: 2 sums 

    14: 2 sums 

    -21: 5 sums: 2 + 2 + 17, 3 + 5 + 13, 3 + 7 + 11, 5 + 5 + 11, 7 + 7 + 7

Nota que: 

1. Debe encontrarse dentro de la carpeta goldback_serial.

2. Debe seguir el formato 

    ./bin/nombre_ejecutable < dir_de_txt

### **Sección de créditos:**

**Estudiante:** Luis David Solano Santamaría C17634

**Contacto:** luis.solanosantamaria@ucr.ac.cr

#### **Recursos de terceros:**

[Implementación de Sieve of Atkin utilizada para calcular números primos:](https://www.geeksforgeeks.org/sieve-of-atkin/)

Dentro de prime_container se utiliza calculate primes, este es algoritmo de Sieve of Atkin para calcular números primos. En vez de imprimirlo como en la versión tomada , se guarda dentro del struct en el prime_container.

[Base de array dinámico realizada por el profesor Jeisson Hidalgo en el curso Programación II:](https://jeisson.ecci.ucr.ac.cr/progra2/2022a/ejemplos/#median3)

Este array dinámico es la base para partes dentro de prime_container, numbers_array y number_container. Ya que su estructura se utiliza para guardar los números primos, los number_container y finalmente las sumas de Goldbach dentro de los number_container.

[Base de enum para errores realizada por el profesor Jeisson Hidalgo en el curso Programación II:](https://jeisson.ecci.ucr.ac.cr/progra2/2022a/ejemplos/#arrays)

La enumeración de error_t implementada por el profesor se utilizó durante el código para el manejo de errores.
