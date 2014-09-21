/**
    Busca un nodo hoja según el rectángulo rect. 
    Ocupado en @insertar(Nodo, Rectangulo)
*/
Nodo encontrar_hoja(Nodo nodo, Rectangulo rect) {

	// Si el nodo es un nodo hoja (i.e. si sus MBR apuntan a -1),
	// devolver ese nodo
	if (nodo.mbr[0].nodo_hijo == -1) 
	{	
		return nodo;
	}
	// Si no, recorremos los MBR's del nodo e identificamos el de menor
	// incremento de área, para buscar por ese subárbol
	else
	{
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
		// TODO: actualizar en disco
        //nodo.mbr[imin].rect = calcular_mbr_minimo(nodo.mbr[imin].rect, rect);
        //actualizar_en_disco(nodo);

        // buscar en el hijo encontrado segun criterio de arriba.
		nodo = leer_nodo(nodo.mbr[imin].nodo_hijo);
		return encontrar_hoja(nodo, rect);
	}
}

Dos_nodos split(Nodo nodo, MBR m)
{
	int i;
	Dos_nodos dn;
	// Nodo 1
	dn.n1.nodo_id = nodo.nodo_id;
	dn.n1.ultimo = T;
	dn.n1.pos_mbr_padre = nodo.pos_mbr_padre;
	dn.n1.nodo_padre = nodo.nodo_padre;

	// Nodo 2
	rtree_ultimo++;
	dn.n2.nodo_id = rtree_ultimo;
	dn.n2.ultimo = T-1;
	dn.n2.nodo_padre = nodo.nodo_padre;

	Rectangulo mbr1;
	Rectangulo mbr2;
	mbr1 = nodo.mbr[0].rect;
	mbr2 = nodo.mbr[T].rect;
	dn.n1.mbr[0] = nodo.mbr[0];
	dn.n2.mbr[0] = nodo.mbr[T];

	// repartición
	for (i = 1; i < T; i++)
	{	
		dn.n1.mbr[i] = nodo.mbr[i];
		mbr1 = calcular_mbr_minimo(mbr1, nodo.mbr[i].rect);
		dn.n2.mbr[i] = nodo.mbr[T+i];
		mbr2 = calcular_mbr_minimo(mbr2, nodo.mbr[T+i].rect);

	}		

	dn.n1.mbr[i] = m;
	mbr1 = calcular_mbr_minimo(mbr1, m.rect);

	dn.mbr1 = mbr1;
	dn.mbr2 = mbr2;

	return dn;
}

/**
    Actualiza el mbr mínimo en el padre.
    TODO: Eliminar ya que esto se hace en insertar_hoja.
*/
void ajustar_mbr_padres(int id_padre, int pos_mbr_padre, Rectangulo rect){

	Nodo n = leer_nodo(id_padre);
	Rectangulo nuevo_mbr = calcular_mbr_minimo(n.mbr[pos_mbr_padre].rect, rect);
	n.mbr[pos_mbr_padre].rect = nuevo_mbr;

	actualizar_nodo(n);
	
    // se hace recursivamente hasta la raíz
    if (n.nodo_padre != -1)
		ajustar_mbr_padres(n.nodo_padre, n.pos_mbr_padre, nuevo_mbr);
}

/**
    ajusta el árbol cuando se produce un split.
*/
void ajustar_split(Dos_nodos dn)
{	 
    Nodo aux, p;
    int i;    // iterador

	// si es raiz
	if (dn.n1.nodo_padre == -1){

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

		// si está lleno
		if (p.ultimo == 2*T-1)
		{	
            // actualizar mbr del nodo spliteado
			p.mbr[dn.n1.pos_mbr_padre].rect = dn.mbr1;
            // volver a hacer split sobre el nodo padre insertanto en nuevo nodo creado
			Dos_nodos dn_ = split(p, make_mbr_2(dn.mbr2,dn.n2.nodo_id));
            
            // actualizamos las posiciones dentro del mbr del padre en los nodos hijos.
			for(i=0; i < dn_.n1.ultimo; i++)
			{
				aux = leer_nodo(dn_.n1.mbr[i].nodo_hijo);
				aux.nodo_padre = dn_.n1.nodo_id;
				aux.pos_mbr_padre = i;
				actualizar_nodo(aux);
			}

			for(i=0; i < dn_.n2.ultimo; i++)
			{
				aux = leer_nodo(dn_.n2.mbr[i].nodo_hijo);
				aux.nodo_padre = dn_.n2.nodo_id;
				aux.pos_mbr_padre = i;
				actualizar_nodo(aux);
			}

			ajustar_split(dn_);
		}
		// si hay espacio
		else
		{	
			p.ultimo++;
			p.mbr[p.ultimo] = make_mbr_2(dn.mbr2, dn.n2.nodo_id);
			dn.n2.pos_mbr_padre = p.ultimo;
			p.mbr[dn.n1.pos_mbr_padre].rect = dn.mbr1;
			actualizar_nodo(p);
			actualizar_nodo(dn.n1);
			insertar_nodo(dn.n2);
            // si no es la raíz, ajustar los mbr de los ancestros.
			if (p.nodo_padre != -1)
				ajustar_mbr_padres(p.nodo_padre, p.pos_mbr_padre, calcular_mbr_minimo(dn.mbr1, dn.mbr2));
		}
	}
}

/**
    Inserta un rectángulo dentro de un r-tree
*/
void insertar(Nodo nodo, Rectangulo rect)
{
    // se busca el nodo donde debe ser insertado.
	Nodo n = encontrar_hoja(nodo, rect);

    // si no está lleno
	if (n.ultimo < 2*T-1)
	{
		n.ultimo++;
		n.mbr[n.ultimo] = make_mbr_2(rect,-1);
		
        actualizar_nodo(n);
		
        // si el nodo n no es la raíz
        if (n.nodo_padre != -1)
			ajustar_mbr_padres(nodo.nodo_padre, nodo.pos_mbr_padre,rect);
	}
    // de lo contrario dividir el nodo
	else
	{	
		Dos_nodos dn = split(nodo, make_mbr_2(rect,-1));
		ajustar_split(dn);
	}
}

/**
    Entrega un arreglo de rectángulos que intersectan con el rectágulo dado.
*/
Dynamic_array* buscar(Nodo nodo, Rectangulo rect)
{
	int i, j;
	Dynamic_array * rects;
	initArray(rects, 1);

	/*
	* Si nodo es hoja, incorporo a mi arreglo los rectangulos que intersectan
	* con mi input
	*/ 
	if (nodo.mbr[0].nodo_hijo == -1)
	{
		for(i=0;i<=nodo.ultimo;i++)
			if (interseccion(rect, nodo.mbr[i].rect))
				insertArray(rects, nodo.mbr[i].rect);
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
				Dynamic_array *rects_hijos = buscar(nodo_hijo, rect);

				// se insertan en el grupo general
				for(j=0;j<rects_hijos->used;j++)
				{
					insertArray(rects, rects_hijos->array[j]);
				}
				
				// se libera la memoria.
				freeArray(rects_hijos);
			}
		}
	}	

	return rects;
}

