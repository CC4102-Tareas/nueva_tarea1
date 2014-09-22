#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./shapelib-1.2.10/shapefil.h"

#define DEBUG_EXP_GEO FALSE
#define DEBUG_INSERTAR FALSE
#define DEBUG_DISCO FALSE
#define DEBUG_LINEAR_SPLIT FALSE

#include "../estructuras.c"
#include "../dynamic_array.c"
#include "../operadores.c"
#include "../split.c"
#include "../r-tree.c"
#include "../rtree_op.c"

#define RUTA_ARCHIVOS "./data/tl_2011_06_prisecroads"

int main (int arc, char **argv) {
    
    int pnEntities;         // cantidad de figuras a consultar
    int pnShapetype;        // tipo de figura a consultar
    double padfMinBound[4]; // límites inferiores de cada coordenada
    double padfMaxBound[4]; // límites superiores de cada coordenada

    SHPHandle h = SHPOpen(RUTA_ARCHIVOS,"rb");
    SHPGetInfo(h, &pnEntities, &pnShapetype, padfMinBound, padfMaxBound);
    
    if (DEBUG_EXP_GEO) {
        printf("MinX: %f | Min Y: %f\n",  padfMinBound[0], padfMinBound[1]);
        printf("MaxX: %f | Max Y: %f\n",  padfMaxBound[0], padfMaxBound[1]);
        printf("Cantidad de entidades: %d\n", pnEntities);
        printf("Tipo de figuras: %d\n", pnShapetype);
    }

    int i;
    SHPObject *obj;

    Rectangulo rect_aux;

    // se crea el árbol
    init_rtree();

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

    // se cierra el archivo y se liberan recursos.
    SHPClose(h);

    Dynamic_array *resultado;

    // ahora se va ha buscar cada elemento del archivo block
    for(i=0;i<1;i++) {
        
        //TODO: crear rect para buscarlo en el rtree
        //rect_aux = make_rect((float)obj->dfXMin, (float)obj->dfXMax, (float)obj->dfYMin, (float)obj->dfYMax);
        
        if (DEBUG_EXP_GEO)
            printf("Buscar->[%f,%f]X[%f,%f]\n", obj->dfXMin, obj->dfXMax, obj->dfYMin, obj->dfYMax);
    
        resultado = buscar(obtener_raiz(), rect_aux);

    }
    





    return 0; // ok
}
