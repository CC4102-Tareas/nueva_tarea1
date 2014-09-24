#!/bin/bash
# compilar
gcc -o experimento experimento_geografico.c ./shapelib-1.2.10/shpopen.o ./shapelib-1.2.10/dbfopen.o
#ejecutar con linear split 10 veces
./experimento 1 1 > out1.txt
#ejecutar con quadratic split 10 veces
./experimento 0 1 >> out1.txt
