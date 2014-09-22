/**
    Este archivo contiene funciones para revisar el 
    estado interno de un r-tree.
*/

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
 
        for(i=0;i<=n.ultimo;i++) {
            if (n.mbr[i].nodo_hijo != -1) {
                // si no quiere mostrar los mbr. Comentar aquí.
                printf("===============================================\n");
                printf("[%f,%f]x[%f,%f] | nodo_hijo: %d\n",
                        n.mbr[i].rect.x1, n.mbr[i].rect.x2, n.mbr[i].rect.y1, n.mbr[i].rect.y2, n.mbr[i].nodo_hijo);
                imprimir_arbol(leer_nodo(n.mbr[i].nodo_hijo), i, space);
            } else {
                // si no quiere mostrar los rectangulos insertados. Comentar aquí.
                printf("[%f,%f]x[%f,%f] | nodo_hijo: %d\n",
                        n.mbr[i].rect.x1, n.mbr[i].rect.x2, n.mbr[i].rect.y1, n.mbr[i].rect.y2, n.mbr[i].nodo_hijo);
            }
        }
}

