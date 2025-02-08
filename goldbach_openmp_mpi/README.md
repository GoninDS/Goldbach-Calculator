# **Goldbach_openmp_mpi**

Se presenta una implementación al trabajo de cálculo de sumas de goldbach utilizando las tecnologías de OpenMP y MPI. En caso de querer consultar el propósito se debe leer las secciones de enunciado y manual de uso. Para ver el documento de optimizaciones puede consultar este [README](./report/README.MD).

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

    Total 12 numbers 19 sums

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

La solución de su problema debe utilizar la tecnología de OpenMP y MPI para calcular las sumas de Goldbach de manera concurrente.

De esta manera, se deben utilizar hilos, que se dividan el trabajo, para realizar el cálculo de las sumas de Goldbach divido en diversos procesos. Al realizar el trabajo de manera concurrente, el tiempo de ejecución debería decrementar o al menos ser igual a la solución serial. 

De igual manera, el programa debe poder recibir como parámetro la cantidad de hilos que se desean utilizar. En caso de no insertar un parámetro, se utiliza la cantidad de hilos del sistema operativo. Para un ejemplo de esto consulte el manual de uso.

Para la solución se propone la siguiente forma de trabajarlo:

**En caso de tener un solo proceso:**

1. Realizar una lectura de los datos y guardar los números en la estructura de datos respectiva.

2. Una vez leídos, encuentre el mayor número de los insertados y encuentre todo primo antes de él, para no tener que calcularlos en cada pasada. Para realizar esto se recomienda utilizar algún algoritmo conocido para números primos, como el Sieve of Atkin implementado en esta solución.

3. Divida el trabajo del cálculo de los números obtenidos en diferentes hilos para que se dividan el trabajo, donde cada hilo se encarga del cálculo singular del número. Una vez terminado, si hay más números por ser procesados, el hilo se mueve a calcular el siguiente disponible.

4. Muestre los resultados obtenidos.

**En caso de tener más de un proceso:**

1. El proceso 0 se encarga de realizar una lectura de los datos y los guarda en una estructura de datos.

2. Una vez leídos, el proceso 0, se encarga de realizar la distribución de bloques de trabajo para el resto de procesos.

3. Dentro de cada proceso, se crean hilos que utilizan un mapeo dinámico para realizar los cálculos requeridos.

4. Cada proceso reporta sus resultados, una vez terminados, de vuelta al proceso 0 y este se encarga de imprimir los resultados.

Para tener una representación más visual, consulte este [README](./design/README.md)

### **Manual de uso:**

Para la compilación del programa puede utilizar los siguientes comandos:

    make clean

    make

Ambos requieren tener el makefile en la carpeta common del repositorio y el makefile de esta misma carpeta que lo incluye.

El make clean se utiliza en caso de tener una versión anterior del programa.

Para la ejecución del programa tiene dos opciones. Puede redirigir la entrada por medio de un archivo o puede insertar manualmente los números y cuando desee terminar presionar CTRL + D.

Ejemplo de ejecución con entrada estándar (dentro de la carpeta bin):

    concurrente22b-luis_solano/homework/goldbach_openmp_mpi/bin$ ./goldbach_openmp_mpi

    2
    
    10

    -8

    Total 3 numbers 3 sums

    2: NA

    10: 2 sums

    -8: 1 sums: 3 + 5

Una vez insertado el -8, se presionó CTRL + D para indicarle al programa que calculara las sumas.

Para este ejemplo se utilizó la cantidad de hilos encontrada en su sistema operativo para la solución, en caso de desear una cantidad diferente se debe ejecutar de la siguiente manera:

    ./goldbach_openmp_mpi CANTIDAD_DE_HILOS

Note que aquí se encuentra en la carpeta bin y CANTIDAD_DE_HILOS representa el número de hilos con el que se desea trabajar. Al ejecutar debe ser el número entero deseado.

Ejemplo de redirección estándar:

    luisdss@luisdss-VirtualBox:~/concurrente22b-luis_solano/homework/goldbach_openmp_mpi$ ./bin/goldbach_openmp_mpi < test/input001.txt

    Total 11 numbers 19 sums

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

1. Debe encontrarse dentro de la carpeta goldbach_openmp_mpi.

2. Debe seguir el formato 

    ./bin/nombre_ejecutable < dir_de_txt

En caso de desear ejecutar con MPI, se realiza de la siguiente manera:

    mpiexec -np NUMERO_PROCESOS ./bin/nombre_ejecutable CANTIDAD_DE_HILOS < dir_de_txt

Donde el parámetro NUMERO_PROCESOS representa la cantidad de procesos con la que se quiere trabajar y CANTIDAD_DE_HILOS representa la cantidad de hilos que se desea por proceso.

Se presentan los siguientes errores en caso de un fallo en la ejecución:

1. **ERROR_READ_VALUES:** Se presenta en caso de no insertar de manera correcta los números de para el proceso de Goldbach.

2. **MEMORY_ERROR:** Se presenta en caso de no poder reservar la memoria apropiada para un proceso.

3. **ILLEGAL_ACCESS:** Se presenta en caso de realizar un acceso ilegal de memoria.

4. **INPUT_ERROR:** Se presenta en caso de realizar un input incorrecto.

5. **THREAD_ERROR:** Se presenta en caso de no poder crear los threads de manera exitosa.

6. **MPI_ERROR:** Se presenta en caso de tener problemas con MPI.

### **Sección de créditos:**

**Estudiante:** Luis David Solano Santamaría C17634

**Contacto:** luis.solanosantamaria@ucr.ac.cr

#### **Recursos de terceros:**

[Implementación de Sieve of Atkin utilizada para calcular números primos:](https://www.geeksforgeeks.org/sieve-of-atkin/)

Dentro de prime_container se utiliza calculate primes, este es algoritmo de Sieve of Atkin para calcular números primos. En vez de imprimirlo como en la versión tomada , se guarda dentro del struct en el prime_container.

[Base de array dinámico realizada por el profesor Jeisson Hidalgo en el curso Programación II:](https://jeisson.ecci.ucr.ac.cr/progra2/2022a/ejemplos/#median3)

Este array dinámico es la base para partes dentro de prime_container, numbers_array y number_container. Ya que su estructura se utiliza para guardar los números primos, los number_container y finalmente las sumas de Goldbach dentro de los number_container.

[Base de enum para errores realizada por el profesor Jeisson Hidalgo en el curso Programación II:](https://jeisson.ecci.ucr.ac.cr/progra2/2022a/ejemplos/#arrays)

La enumeración de error_t implementada por el profesor se utilizó durante el código para el manejo de errores. Ha sido modificada para tener errores nuevos, pero la base se mantiene.
