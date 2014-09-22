#include <stdio.h>
#include "./shapelib-1.2.10/shapefil.h"

#define RUTA_ARCHIVOS "./Data/tl_2011_us_primaryroads.shp"

int main (int arc, char **argv) {
    
    int *pnEntities;      // cantidad de figuras a consultar
    int *pnShapetype;     // tipo de figura a consultar
    double *padfMinBound;
    double *padfMaxBound;

    SHPHandle h = SHPOpen(RUTA_ARCHIVOS,"rb");
    SHPGetInfo(h, pnEntities, pnShapetype, padfMinBound, padfMaxBound);

    int i;
    SHPObject *obj;
    for(i=0;i<*pnEntities;i++) {
        // desde el archivo queremos leer la figura i
        obj = SHPReadObject(h, i);

        printf("[%f,%f]X[%f,%f]\n", obj->dfXMin, obj->dfXMax, obj->dfYMin, obj->dfYMax);

        // se libera la memoria.
        SHPDestroyObject(obj);
    }

    HPClose(h);
    return 0; // ok
}
