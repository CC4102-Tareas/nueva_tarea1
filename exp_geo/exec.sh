#!/bin/bash
# compilar
gcc -o experimento experimento_geografico.c ./shapelib-1.2.10/shpopen.o ./shapelib-1.2.10/dbfopen.o
#ejecutar con linear split 1 vez
./experimento 1 1
