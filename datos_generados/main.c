#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int usar_linear_split;

#define TAMANO_RANGO 500000.0
#define TAMANO_AREA 100.0

#define DEBUG_INSERTAR FALSE
#define DEBUG_DISCO FALSE
#define DEBUG_LINEAR_SPLIT FALSE

#include "../estructuras.c"
#include "../dynamic_array.c"
#include "../operadores.c"
#include "../split.c"
#include "../r-tree.c"
#include "../rtree_op.c"
#include "../tiempo.c"

Rectangulo rectangulo_aleatorio(){

    float var_aux, ancho_x, ancho_y, x2_, y2_;
    float x1 = float_aleatorio(TAMANO_RANGO);
    float y1 = float_aleatorio(TAMANO_RANGO);
    float area = float_aleatorio(TAMANO_AREA);
    float ratio = 0.1+float_aleatorio(0.9); 
                            
    ancho_x = sqrt(area * ratio);
    ancho_y = sqrt(area / ratio);
    x2_ = x1 + ancho_x;
    y2_ = y1 + ancho_y;
    if ((y2_-y1)*(x2_-x1)<0.1 || x2_ >= TAMANO_RANGO || y2_ >= TAMANO_RANGO){
        return rectangulo_aleatorio();
    }
    else {
        return make_rect(x1, x1+ancho_x, y1, y1+ancho_y);
    }
}

/**
    ===================================================================
       MAIN
    ===================================================================
*/

int main (int arc, char **argv) 
{   
    int i, j;
    double dif;
    struct timeval before , after;
    Rectangulo rect;
    Nodo raiz;
    usar_linear_split = atoi(argv[1]);
    int num_rects = atoi(argv[2]);
    int num_repeticiones = atoi(argv[3]);
                                                   
    srand((unsigned int)time(NULL));


    // int n_rects[] = { 512, 4096, 32768, 262144, 2097152, 16777216};
    // int n_exps[] = { 64, 32, 16, 8, 4, 2};
    // int n_exps[] = { 1, 1, 1, 1, 1, 1};

    gettimeofday(&before , NULL);
    for (j=0; j<num_repeticiones;j++){

        // insertar en arbol
        init_rtree();
        for (i=0; i<num_rects; i++){
            rect = rectangulo_aleatorio();
            raiz = obtener_raiz();
            insertar(raiz, rect);
                                                                                                                                                        }

         /*
            raiz = obtener_raiz();
            // buscar en arbol
            for (i=0; i<num_rects/10; i++){
                rect = rectangulo_aleatorio();
                buscar(raiz, rect);
            }
         */

     }
     
     gettimeofday(&after , NULL);
     dif = time_diff(before , after);
     printf("Estuve %f segundos duermiendo\n", dif);

     return 0;
}

/*
    for (i=0; i<100000; i++){
        rect = rectangulo_aleatorio();
        insertar();
        //printf("[%f, %f] x [%f, %f], area: %f\n",rect.x1, rect.x2, rect.y1, rect.y2, (rect.x2-rect.x1)*(rect.y2-rect.y1));
        //printf("%f %f %f %f %f\n",rect.x1, rect.x2, rect.y1, rect.y2, (rect.x2-rect.x1)*(rect.y2-rect.y1));   
    }
*/
