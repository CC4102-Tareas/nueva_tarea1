#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./shapelib-1.2.10/shapefil.h"
#include "../estructuras.c"
#include "../dynamic_array.c"
#include "../operadores.c"
#include "../split.c"
#include "../r-tree.c"
#include "../rtree_op.c"
#include "../tiempo.c"

#define DEBUG_EXP_GEO FALSE

#define RUTA_ARCHIVOS_RUTAS "./data/tl_2011_06_prisecroads"
// OJO: es un path absoluto.
#define RUTA_ARCHIVOS_BLOQUES "/home/cephei/Desktop/tl_2011_06_tabblock/tl_2011_06_tabblock"

int main (int arc, char **argv) {
    
    int pnEntities;         // cantidad de figuras a consultar
    int pnShapetype;        // tipo de figura a consultar
    double padfMinBound[4]; // límites inferiores de cada coordenada
    double padfMaxBound[4]; // límites superiores de cada coordenada
    SHPHandle h;
    int i, j, k;            // iterador
    SHPObject *obj;
    Rectangulo rect_aux;
    Dynamic_array resultado;
    
    double dif;
    struct timeval antes , despues;
    usar_linear_split = atoi(argv[1]);    // 1 en el primer parámetro indica que debemos ocupar linear_split. 0 = quadratic_split.
    int num_repeticiones = atoi(argv[2]); // indica el número de repeticiones que se deben realizar.

// calcula la cantidad de repeticiones
for(k=0;k<num_repeticiones;k++) {
 
    h = SHPOpen(RUTA_ARCHIVOS_RUTAS,"rb");
    SHPGetInfo(h, &pnEntities, &pnShapetype, padfMinBound, padfMaxBound);
    
    if (DEBUG_EXP_GEO) {
        printf("Archivo de Rutas.\n");
        printf("MinX: %f | Min Y: %f\n",  padfMinBound[0], padfMinBound[1]);
        printf("MaxX: %f | Max Y: %f\n",  padfMaxBound[0], padfMaxBound[1]);
        printf("Cantidad de entidades: %d\n", pnEntities);
        printf("Tipo de figuras: %d\n", pnShapetype);
    }

    // se crea el árbol
    init_rtree();
    
    // comenzamos a medir la construcción del árbol.
    gettimeofday(&antes , NULL);

    for(i=0;i<pnEntities;i++) {
        // desde el archivo queremos leer la figura i
        obj = SHPReadObject(h, i);

        if (DEBUG_EXP_GEO)
            printf("[%f,%f]X[%f,%f] | id: %d | tipo: %d\n", obj->dfXMin, obj->dfXMax, obj->dfYMin, obj->dfYMax, obj->nShapeId, obj->nSHPType);
        
        //construir arbol
        rect_aux = make_rect((float)obj->dfXMin, (float)obj->dfXMax, (float)obj->dfYMin, (float)obj->dfYMax);
        insertar(obtener_raiz(), rect_aux);

        // se libera la memoria.
        SHPDestroyObject(obj);
    }
    // terminamos de medir la construcción del árbol.
    gettimeofday(&despues , NULL);
    dif = time_diff(antes , despues);
    printf("Construcción de R-tree duró: %f segundos duermiendo.\n", dif);

    // se cierra el archivo y se liberan recursos.
    SHPClose(h);
    
    // se abre el archivo de bloques para buscarlos.
    h = SHPOpen(RUTA_ARCHIVOS_BLOQUES,"rb");
    SHPGetInfo(h, &pnEntities, &pnShapetype, padfMinBound, padfMaxBound);

    if (DEBUG_EXP_GEO) {
        printf("Archivo de bloques.\n");
        printf("MinX: %f | Min Y: %f\n",  padfMinBound[0], padfMinBound[1]);
        printf("MaxX: %f | Max Y: %f\n",  padfMaxBound[0], padfMaxBound[1]);
        printf("Cantidad de entidades: %d\n", pnEntities);
        printf("Tipo de figuras: %d\n", pnShapetype);
    }

    // comenzamos a medir la construcción del árbol.
    gettimeofday(&antes , NULL);

    // ahora se va ha buscar cada elemento del archivo block
    for(i=0;i<pnEntities;i++) {
        obj = SHPReadObject(h, i);
        
        // crear rect para buscarlo en el rtree
        rect_aux = make_rect((float)obj->dfXMin, (float)obj->dfXMax, (float)obj->dfYMin, (float)obj->dfYMax);
        
        if (DEBUG_EXP_GEO)
            printf("Buscar->[%f,%f]X[%f,%f]\n", obj->dfXMin, obj->dfXMax, obj->dfYMin, obj->dfYMax);
    
        resultado = buscar(obtener_raiz(), rect_aux);
        
        if (DEBUG_EXP_GEO)
            for(j=0;j<resultado.used;j++) {
                printf("   Encontrado ->[%f,%f]X[%f,%f]\n", resultado.array[j].x1, resultado.array[j].x2, 
                                                            resultado.array[j].y1, resultado.array[j].y2);
            }
        
        freeArray(&resultado);
        // se libera la memoria.
        SHPDestroyObject(obj);
    }
    // terminamos de medir la construcción del árbol.
    gettimeofday(&despues , NULL);
    dif = time_diff(antes , despues);
    printf("Búsqueda de todos los rectángulos duró: %f segundos.\n", dif);

    // se cierra el archivo y se liberan recursos.
    SHPClose(h);
}

    return 0; // ok
}
