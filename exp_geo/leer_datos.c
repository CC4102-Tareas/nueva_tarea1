#include <stdio.h>
#include <time.h>
#include "./shapelib-1.2.10/shapefil.h"

#define RUTA_ARCHIVOS "./data/tl_2011_us_primaryroads"

int main (int arc, char **argv) {
    
    int pnEntities;         // cantidad de figuras a consultar
    int pnShapetype;        // tipo de figura a consultar
    double padfMinBound[4]; // límites inferiores de cada coordenada
    double padfMaxBound[4]; // límites superiores de cada coordenada

    SHPHandle h = SHPOpen(RUTA_ARCHIVOS,"rb");
    SHPGetInfo(h, &pnEntities, &pnShapetype, padfMinBound, padfMaxBound);
    
    printf("MinX: %f | Min Y: %f\n",  padfMinBound[0], padfMinBound[1]);
    printf("MaxX: %f | Max Y: %f\n",  padfMaxBound[0], padfMaxBound[1]);
    printf("Cantidad de entidades: %d\n", pnEntities);
    printf("Tipo de figuras: %d\n", pnShapetype);

    int i;
    SHPObject *obj;

    for(i=0;i<pnEntities;i++) {
        // desde el archivo queremos leer la figura i
        obj = SHPReadObject(h, i);

        printf("[%f,%f]X[%f,%f] | id: %d | tipo: %d\n", obj->dfXMin, obj->dfXMax, obj->dfYMin, obj->dfYMax, obj->nShapeId, obj->nSHPType);
        
        //construir arbol
        

        // se libera la memoria.
        SHPDestroyObject(obj);
    }

    SHPClose(h);
    return 0; // ok
}
