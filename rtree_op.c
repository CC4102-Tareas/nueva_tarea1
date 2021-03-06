// activar en casos de debug
#define DEBUG_INSERTAR FALSE

Nodo obtener_raiz() {
    return leer_nodo(rtree_root);
}

/**
    Busca un nodo hoja según el rectángulo rect. 
    Ocupado en @insertar(Nodo, Rectangulo)
*/
Nodo encontrar_hoja(Nodo nodo, Rectangulo rect) {

	// Si el nodo es un nodo hoja (i.e. si sus MBR apuntan a -1),
	// devolver ese nodo
	if (nodo.mbr[0].nodo_hijo == -1) 
	{	
		((DEBUG_INSERTAR) ? printf("Nodo %d es un nodo hoja.\n", nodo.nodo_id):0);
		return nodo;
	}
	// Si no, recorremos los MBR's del nodo e identificamos el de menor
	// incremento de área, para buscar por ese subárbol
	else
	{
		((DEBUG_INSERTAR) ? printf("Nodo %d no es un nodo hoja. Buscamos el que crea MBR mínimo.\n", nodo.nodo_id):0);
	
    	int i;                     	                         // iterador
		float min = incremento_area(nodo.mbr[0].rect, rect); // incremento de area minimo
		float amin = area(nodo.mbr[0].rect);				 // area del mbr que da menor inc de area
		int imin = 0;                                        // indice del MBR con menor incremento de área.
		float curr_inc_area;                                 // inc area actual
		float curr_area;		                             // area actual

		for (i = 1; i<=nodo.ultimo; i++)
		{
			curr_inc_area = incremento_area(nodo.mbr[i].rect, rect);
			curr_area = area(nodo.mbr[i].rect);
            
            // el criterio arbitrario es dejar el último MBR encontrado cuando todas las
            // demás condiciones son iguales.
			if (curr_inc_area < min || (curr_inc_area == min && curr_area <= amin))
			{
				min = curr_inc_area;
				amin = curr_area;
				imin = i;
			}
		}
		// actualizar en disco
        nodo.mbr[imin].rect = calcular_mbr_minimo(nodo.mbr[imin].rect, rect);
        actualizar_nodo(nodo);
        // buscar en el hijo encontrado segun criterio de arriba.
		nodo = leer_nodo(nodo.mbr[imin].nodo_hijo);

		((DEBUG_INSERTAR) ? printf("Nodo %d crea MBR mínimo. Buscamos hoja en sus hijos.\n", nodo.nodo_id):0);

		return encontrar_hoja(nodo, rect);
	}
}

void ajustar_split(Dos_nodos dn)
{	 
    Nodo aux, p;
    int i;    // iterador
    
    if (DEBUG_INSERTAR) {
        printf("Ajusto árbol porque se splitteó el nodo %d.\n",dn.n1.nodo_id);
    	printf("|-> nodo_padre: %d\n",dn.n1.nodo_padre);
	    printf("|-> posicion_mbr_padre: %d\n",dn.n1.pos_mbr_padre);
    	printf("|-> ultimo: %d\n",(dn.n1.ultimo + dn.n2.ultimo -1));
    
	    printf("Resultado de split\n");
        for(i=0;i<=dn.n1.ultimo;i++) {            
            printf("n1: [%f,%f]x[%f,%f] | nodo_hijo: %d\n",
                        dn.n1.mbr[i].rect.x1, dn.n1.mbr[i].rect.x2, dn.n1.mbr[i].rect.y1, dn.n1.mbr[i].rect.y2, dn.n1.mbr[i].nodo_hijo);
        }
        printf("============\n");
        for(i=0;i<=dn.n2.ultimo;i++) {
            printf("n2: [%f,%f]x[%f,%f] | nodo_hijo: %d\n",
                        dn.n2.mbr[i].rect.x1, dn.n2.mbr[i].rect.x2, dn.n2.mbr[i].rect.y1, dn.n2.mbr[i].rect.y2, dn.n2.mbr[i].nodo_hijo);
        }
    }
	// si es raiz
	if (dn.n1.nodo_padre == -1){
		((DEBUG_INSERTAR) ? printf("El nodo %d espliteado es una raíz por lo que debo crear un nodo adicional.\n", dn.n1.nodo_id):0);
        // ===================================================================
		// TODO: crear un nuevo nodo NN y una nueva raíz NR; pasos:
		// repartir rectángulos entre nuevo nodo NN y antigua raíz AR (split)
		// rtree_ultimo++
		// NN.id = ultimo
		// crear nueva raíz AR apuntando a Antigua Raíz AR y a Nuevo Nodo NN
		// ultimo++
		// rtree_root = ultimo + 1;
		// asignar NR como padre a AR y NN
		// insertar NN
		// insertar NR
		// actualizar AR
        // ===================================================================

        // se asigna un identificador único al nuevo nodo y se asigna este como root
		rtree_ultimo++;
		rtree_root = rtree_ultimo;
		// Antigua raiz ahora apunta a la nueva raíz en la 1° posición del mbr
		dn.n1.nodo_padre = rtree_ultimo;
		dn.n1.pos_mbr_padre = 0;
		actualizar_nodo(dn.n1);
		// nodo creado en el split apunta a la nueva raíz en la 2° posiciín del mbr
		dn.n2.nodo_padre = rtree_ultimo;
		dn.n2.pos_mbr_padre = 1;
		insertar_nodo(dn.n2);
		// Se crea la raiz
		aux.nodo_id = rtree_ultimo;
		aux.nodo_padre = -1;
		aux.pos_mbr_padre = -1;
		aux.ultimo = 1; // tiene dos MBR
        // se crean sus MBRs asociados a los nodos n1 y n2
		aux.mbr[0] = make_mbr_2(dn.mbr1, dn.n1.nodo_id);
		aux.mbr[1] = make_mbr_2(dn.mbr2, dn.n2.nodo_id);
		insertar_nodo(aux);		
	}
	// si no es raíz
	else
	{		
		p = leer_nodo(dn.n1.nodo_padre);
        Dos_nodos dn_;
		// si está lleno
		if (p.ultimo == 2*T-1)
		{	
		    ((DEBUG_INSERTAR) ? printf("El nodo %d spliteado tiene un nodo padre que también está lleno.\n", dn.n1.nodo_id):0);

            // actualizar mbr del nodo spliteado
			p.mbr[dn.n1.pos_mbr_padre].rect = dn.mbr1;
            // volver a hacer split sobre el nodo padre insertando el nuevo nodo creado
            // OJO: que el split no asigna nodo_padre ni pos_mbr_padre a dn.n2 porque no sabe quien será.

            if(usar_linear_split)
    			dn_ = linear_split(p, make_mbr_2(dn.mbr2, dn.n2.nodo_id));
            else
                dn_ = quadratic_split(p, make_mbr_2(dn.mbr2, dn.n2.nodo_id));
            // actualizamos las posiciones dentro del mbr del padre en los nodos hijos.
            // En este punto los nodos hijos siempre existen.
		    for(i=0; i <= dn_.n1.ultimo; i++) {
                if (dn_.n1.mbr[i].nodo_hijo == dn.n2.nodo_id) {
                    dn.n2.nodo_padre = dn_.n1.nodo_id;
                    dn.n2.pos_mbr_padre = i;
                    insertar_nodo(dn.n2);
                } 
                else if (dn_.n1.mbr[i].nodo_hijo == dn.n1.nodo_id) {
                    dn.n1.nodo_padre = dn_.n1.nodo_id;
                    dn.n1.pos_mbr_padre = i;
                    actualizar_nodo(dn.n1);
				}
				else {
                    aux = leer_nodo(dn_.n1.mbr[i].nodo_hijo);
         			aux.nodo_padre = dn_.n1.nodo_id;
	        		aux.pos_mbr_padre = i;
		        	actualizar_nodo(aux);
                }
            }

		    for(i=0; i <= dn_.n2.ultimo; i++) {
                if (dn_.n2.mbr[i].nodo_hijo == dn.n2.nodo_id) {
                    dn.n2.nodo_padre = dn_.n2.nodo_id;
                    dn.n2.pos_mbr_padre = i;
                    insertar_nodo(dn.n2);
                }
                else if (dn_.n2.mbr[i].nodo_hijo == dn.n1.nodo_id) {
                    dn.n1.nodo_padre = dn_.n2.nodo_id;
                    dn.n1.pos_mbr_padre = i;
                    actualizar_nodo(dn.n1);
				}
				else {
       				aux = leer_nodo(dn_.n2.mbr[i].nodo_hijo);
   	    			aux.nodo_padre = dn_.n2.nodo_id;
    	    		aux.pos_mbr_padre = i;
	    		    actualizar_nodo(aux);
                }
            }
			
            // se actualiza el nodo esplitiado
            //actualizar_nodo(dn.n1);

            ajustar_split(dn_);
		}
		// si hay espacio
		else
		{	
		    ((DEBUG_INSERTAR) ? printf("El nodo %d spliteado tiene un nodo padre con espacio.\n", dn.n1.nodo_id):0);
			
            p.ultimo++;
			p.mbr[p.ultimo] = make_mbr_2(dn.mbr2, dn.n2.nodo_id);
            dn.n2.nodo_padre = p.nodo_id;
			dn.n2.pos_mbr_padre = p.ultimo;
			p.mbr[dn.n1.pos_mbr_padre].rect = dn.mbr1;
			actualizar_nodo(p);
			actualizar_nodo(dn.n1);
			insertar_nodo(dn.n2);
		}
	}
}

/**
    Inserta un rectángulo dentro de un r-tree
*/
void insertar(Nodo nodo, Rectangulo rect)
{
    ((DEBUG_INSERTAR) ? printf("Insertar rectangulo [%f,%f]x[%f,%f].\n", rect.x1, rect.x2, rect.y1, rect.y2):0);

    // se busca el nodo donde debe ser insertado. Siempre retorna un nodo hoja.
	Nodo n = encontrar_hoja(nodo, rect);

    ((DEBUG_INSERTAR) ? printf("encontrar_hoja me devolvio nodo %d\n", n.nodo_id):0);

    // si no está lleno
	if (n.ultimo < 2*T-1)
	{
		((DEBUG_INSERTAR) ? printf("Nodo %d no está lleno. Vamos a insertar aquí.\n", n.nodo_id):0);

        n.ultimo++;
		n.mbr[n.ultimo] = make_mbr_2(rect,-1);
		
        actualizar_nodo(n);
	}
    // de lo contrario dividir el nodo
	else
	{	
		((DEBUG_INSERTAR) ? printf("Nodo %d está lleno. Requiere hacer split.\n", n.nodo_id):0);
        Dos_nodos dn;
        
        if(usar_linear_split)
    	    dn = linear_split(n, make_mbr_2(rect, -1));
        else
            dn = quadratic_split(n, make_mbr_2(rect, -1));
		ajustar_split(dn);
	}
}

/**
    Entrega un arreglo de rectángulos que intersectan con el rectágulo dado.
*/
Dynamic_array buscar(Nodo nodo, Rectangulo rect)
{
	int i, j;
	Dynamic_array rects;
	initArray(&rects, 1);
	/*
	* Si nodo es hoja, incorporo a mi arreglo los rectangulos que intersectan
	* con mi input
	*/ 
	if (nodo.mbr[0].nodo_hijo == -1)
	{
		for(i=0;i<=nodo.ultimo;i++)
			if (interseccion(rect, nodo.mbr[i].rect))
				insertArray(&rects, nodo.mbr[i].rect);
	}
	/*
	* Si nodo es interno, hago busqueda en profundidad solo por los nodos que
	* intersectan con mi input
	*/
	else
	{
		for(i=0;i<=nodo.ultimo;i++)
		{
			if (interseccion(rect, nodo.mbr[i].rect))
			{
				Nodo nodo_hijo = leer_nodo(nodo.mbr[i].nodo_hijo);
				Dynamic_array rects_hijos = buscar(nodo_hijo, rect);

				// se insertan en el grupo general
				for(j=0;j<rects_hijos.used;j++)
				{
					insertArray(&rects, rects_hijos.array[j]);
				}
				
				// se libera la memoria.
				freeArray(&rects_hijos);
			}
		}
	}	

	return rects;
}

