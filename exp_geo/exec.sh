#!/bin/bash
# compilar
gcc -o ejemplo leer_datos.c ./shapelib-1.2.10/shpopen.o ./shapelib-1.2.10/dbfopen.o
#ejecutar
./ejemplo
