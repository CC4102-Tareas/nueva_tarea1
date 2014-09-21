#include <stdio.h>
#include <stdlib.h>

#define DEBUG_INSERTAR FALSE
#define DEBUG_DISCO FALSE

#include "estructuras.c"
#include "dynamic_array.c"
#include "operadores.c"
#include "split.c"
#include "r-tree.c"
#include "rtree_op.c"

/**
	===================================================================
	MAIN
	===================================================================
*/
Nodo obtener_raiz(){
    return leer_nodo(rtree_root);
}

void imprimir_nodo(Nodo r){
	int i;
	Rectangulo mbrect;
	
    printf("---> nodo ID: %d\n", r.nodo_id);
	printf("|    Padre       : %d\n",r.nodo_padre);
	printf("|    Pos en padre: %d\n",r.pos_mbr_padre);
	printf("|    N° hijos    : %d (ultimo: %d)\n",r.ultimo+1, r.ultimo);
    printf("|\n");
	for (i = 0; i <= r.ultimo; i++) {
		printf("|---> hijo %d\n", i);
		mbrect = r.mbr[i].rect;
		printf("|     MBR      : [%f,%f]x[%f,%f]\n", mbrect.x1, mbrect.x2, mbrect.y1, mbrect.y2);
		printf("|     nodo hijo: %d\n",r.mbr[i].nodo_hijo);
	}
		printf("----------------------------------------------------- fin\n");
}

void imprimir_arbol(Nodo n, int pos_mbr, int space) {
    int i;
    for(i=0;i<space;i++)
        printf(" ");
    if (space>0)
        printf("|->");

    if (pos_mbr == -1)
        printf(" Nodo %d\n", n.nodo_id);
    else
        printf(" %d:Nodo %d\n", pos_mbr, n.nodo_id);
    
    // tabulo en espacios de 4
    space = space + 4;

    for(i=0;i<=n.ultimo;i++)
        if (n.mbr[i].nodo_hijo != -1)
            imprimir_arbol(leer_nodo(n.mbr[i].nodo_hijo), i, space);
        else {
            //for(i=0;i<space;i++) {printf(" ");} 
            //printf("|->");
            //printf("[%f,%f]x[%f,%f] | nodo_hijo: %d\n",n.mbr[i].rect.x1, n.mbr[i].rect.x2, n.mbr[i].rect.y1, n.mbr[i].rect.y2, n.mbr[i].nodo_hijo);
        }
}

int main (int arc, char **argv) 
{	int i;

	init_rtree();
	insertar(obtener_raiz(),make_rect(1.0,3.0,2.0,2.5));
	insertar(obtener_raiz(),make_rect(1.0,2.0,3.0,5.0));
	insertar(obtener_raiz(),make_rect(2.0,3.0,1.0,2.0));
	insertar(obtener_raiz(),make_rect(4.0,5.0,4.0,6.0));
	insertar(obtener_raiz(),make_rect(4.0,6.0,3.0,6.0));

	insertar(obtener_raiz(),make_rect(6.0,7.0,9.0,10.0));
	insertar(obtener_raiz(),make_rect(7.0,8.0,10.0,11.0));
	insertar(obtener_raiz(),make_rect(6.0,7.0,4.0,10.0));
	insertar(obtener_raiz(),make_rect(9.0,9.5,5.0,10.0));
	insertar(obtener_raiz(),make_rect(9.0,9.5,5.0,10.0));
	insertar(obtener_raiz(),make_rect(9.0,9.5,5.0,10.0));
	insertar(obtener_raiz(),make_rect(9.0,9.5,5.0,10.0));
	insertar(obtener_raiz(),make_rect(9.0,9.5,5.0,10.0));
	insertar(obtener_raiz(),make_rect(9.0,9.5,5.0,10.0));
	insertar(obtener_raiz(),make_rect(9.0,9.5,5.0,10.0));
	insertar(obtener_raiz(),make_rect(9.0,9.5,5.0,10.0));
	insertar(obtener_raiz(),make_rect(9.0,9.5,5.0,10.0));

	imprimir_arbol(obtener_raiz(), -1, 0);

//	imprimir_nodo(obtener_raiz());
	printf("--------------------------\n");
	printf("--------------------------\n");

	//for (i=0;i <= rtree_ultimo; i++)
		//imprimir_nodo(leer_nodo(i));



	return 0; // todo ok

	//imprimir_nodo(obtener_raiz());
	//imprimir_nodo(leer_nodo(0));
	//imprimir_nodo(leer_nodo(1));
	//imprimir_nodo(leer_nodo(2));
	
    return 0; // todo ok


}
	/*	Rectangulo rect;

	MBR mbr;

	printf("Nodo: %d\n", sizeof(nodo));
	printf("Rectangulo: %d\n", sizeof(rect));
	printf("MBR: %d\n", sizeof(mbr));
	*/
	//prueba 1: crear r-tree, insertar dos nodos y leerlos: OK

	/*
	init_rtree();
	Nodo n; 
	n.nodo_id=0;
	n.nodo_padre=-1;
	n.pos_mbr_padre=-1;
	n.ultimo=0;
	insertar_nodo(n);

	Nodo p = leer_nodo(0);

	printf("ID: %d\n",p.nodo_id);
	printf("Padre: %d\n",p.nodo_padre);
	printf("Pos: %d\n",p.pos_mbr_padre);
	printf("Ultimo: %d\n",p.ultimo);

	n.nodo_id=1;
	n.nodo_padre=-1;
	n.pos_mbr_padre=-1;
	n.ultimo=666;
	insertar_nodo(n);

	p = leer_nodo(1);
	printf("ID: %d\n",p.nodo_id);
	printf("Padre: %d\n",p.nodo_padre);
	printf("Pos: %d\n",p.pos_mbr_padre);
	printf("Ultimo: %d\n",p.ultimo);

	n.nodo_id=0;
	n.nodo_padre=-1;
	n.pos_mbr_padre=95;
	n.ultimo=666;
	actualizar_nodo(n);
	p = leer_nodo(0);
	printf("ID: %d\n",p.nodo_id);
	printf("Padre: %d\n",p.nodo_padre);
	printf("Pos: %d\n",p.pos_mbr_padre);
	printf("Ultimo: %d\n",p.ultimo);

	*/

	//prueba 2: probar cosas de rectángulos
	/*
	Rectangulo r1, r2;
	r1 = make_rect(1.0,5.0,1.0,5.0);
	r2 = make_rect(0.0,0.5,0.0,0.5);
	printf("Intersecta: %d\n",interseccion(r2,r1));
	printf("Area r1: %f\n",area(r1));

	Rectangulo mbrect = calculate_mbr (r1,r2);
	printf("MBR: %f %f %f %f\n", mbrect.x1, mbrect.x2, mbrect.y1, mbrect.y2);
	printf("Aumento area: %f\n",incremento_area(r1,r2));
	*/


	/*
	float min1 = a[0];
	float min2 = a[1];

	int imin1 = 0;
	int imin2 = 1;

	for (i = 1; i < 10; i++)
	{
		 if (a[i] < min1)
		 {
		     min2 = min1;
		     min1 = a[i];
		     imin2 = imin1;
		     imin1 = i;

		 }
	
		if (a[i] < min2 && (min1 != a[i] || imin1!=i) )
		{
		min2 = a[i];
		imin2 = i;
		}
	}	
	*/

	// CODIGO UTILISIMO	

	/*
	float inc_area[10] = { 0.1, 0.02, 5.1, 1.3, 2.5, 5.8, 1.3, 2.9, 4.6, 0.02 };
	float 	 area[10] = { 10.0, 3.1, 15.1, 2.3, 2.5, 15.8, 2.3, 33.9, 34.6, 2.0 };

	int i;
	float min = inc_area[0];
	float amin = area[0];
	int imin = 0;

	float curr_inc_area;
	float curr_area;

	for (i = 1; i < 10; i++)
	{
		curr_inc_area = inc_area[i];
		curr_area = area[i];
		
		if (curr_inc_area < min || (curr_inc_area == min && curr_area <= amin))
		{
			min = curr_inc_area;
			amin = curr_area;
			imin = i;
		}
	}

	printf("Ind min: %d\n",imin);
	printf("Inc area min: %f\n",min);
	printf("Area min: %f\n",amin);
	*/
