# AprendiendoAprogramarElKernel
Ejercicio basico de programacion en el modulo de linux kernel
Consigna.
Modulo Char Device
Elaborar un kernel module para un char device
Material de lectura: https://www.tldp.org/LDP/lkmpg/2.6/lkmpg.pdf.
1. Definir funciones init module y cleanup module
 ¿Que necesito que hagan en un char device?
2. Definir funciones device open y device release
3. Hacer que nuestro char device cuando le escribimos imprima en el kernel
 Tip 1: Para escribir a un device en Bash nano /dev/device
 Tip 2: Ojo con los permisos de escritura!
 Tip 3: Al de-registrar y volver a registrar el driver repetir el proceso de crear el
archivo, sino esto trae problemas. Mismo no olvidar hacer make clean
4. Hacer que nuestro char device cuando lo lea me devuelva lo ultimo que fue escrito
 Tip 4: cat /dev/device me tiene que devolver lo que puse con echo
5. Hacer que ahora devuelva el mensaje al reves caracter por caracter, por ej si el mensaje
es hola: aloh.

Para empezar el material de lectura tiene mas de 15 años, y esta en ingles. Por lo que no es dificil encontrarse con
varias trabas a la hora de ejecutar los codigos de ejemplo, y seguir adelante con el trabajo. Por lo que recomiendo comenzar la 
lectura del libro a partir de esta pagina https://sysprog21.github.io/lkmpg/ que tiene el material actualizado, y es mas
facil de traducir con un solo click en el navegador.

Para ejecutar el codigo:

1. Descargar los dos archivos.
2. Abrir la terminal en la ubicacion del archivo y ejecutar los  comandos
3. ```make```
4. ```sudo insmod ./impchar.ko```
5. OPCIONAL :```sudo chmod a+w /dev/impchar```
6. ```sudo sh -c "echo "palabra_que_quieras" > /dev/impchar"```
7. ```sudo cat /dev/impchar```

Despues de este ultimo comando, deberia aparecer en la ubicacion var/log/syslog a lo ultimo de la hoja,
Lo que hemos escrito adicionalmente con la cantidad de letras de la palabra y en la ultima linea la palabra 
escrita desde atras hacia delante. El 90% del codigo y como funciona, se encuentra en el libro.

Por ultimo ejecute el comando sudo rmmod impchar
Y eso es todo por el momento.


