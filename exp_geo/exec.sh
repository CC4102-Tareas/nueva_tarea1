#!/bin/bash
# compilar
gcc -o experimento experimento_geografico.c ./shapelib-1.2.10/shpopen.o ./shapelib-1.2.10/dbfopen.o
#ejecutar con linear split. 1 vez como dice el enunciado
./experimento 1 1 1 > out1.txt
#ejecutar con quadratic split. 1 vez como dice el enunciado
./experimento 0 1 1 >> out1.txt
#ejecutar con linear split. 1 vez al reves de lo que dice el enunciado.
./experimento 1 1 0 >> out1.txt
#ejecutar con quadratic split. 1 vez al reves de lo que dice el enunciado.
./experimento 0 1 0 >> out1.txt
