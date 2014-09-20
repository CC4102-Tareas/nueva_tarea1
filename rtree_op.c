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
		int i;                     	// iterador
		float min = incremento_area(nodo.mbr[0].rect, rect); // incremento de area minimo
		float amin = area(nodo.mbr[0].rect);					  // area del mbr que da menor inc de area
		int imin = 0; // indice del MBR con menor incremento de área.
		float curr_inc_area; // inc area actual
		float curr_area;		// area actual

		for (i = 1; i < i<=nodo.ultimo; i++)
		{
			curr_inc_area = incremento_area(nodo.mbr[i].rect, rect);
			curr_area = area(nodo.mbr[i].rect);

			if (curr_inc_area < min || (curr_inc_area == min && curr_area <= amin))
			{
				min = curr_inc_area;
				amin = curr_area;
				imin = i;
			}
		}
		// TODO: actualizar en disco
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
		mbr1 = calculate_mbr(mbr1, nodo.mbr[i].rect);
		dn.n2.mbr[i] = nodo.mbr[T+i];
		mbr2 = calculate_mbr(mbr2, nodo.mbr[T+i].rect);

	}		

	dn.n1.mbr[i] = m;
	mbr1 = calculate_mbr(mbr1, m.rect);

	dn.mbr1 = mbr1;
	dn.mbr2 = mbr2;

	return dn;
}

void ajustar1(int id_padre, int pos_mbr_padre, Rectangulo rect){

	Nodo n = leer_nodo(id_padre);
	Rectangulo nuevo = calculate_mbr(n.mbr[pos_mbr_padre].rect, rect);
	n.mbr[pos_mbr_padre].rect = nuevo;
	actualizar_nodo(n);
	if (n.nodo_padre != -1)
		ajustar1(n.nodo_padre, n.pos_mbr_padre, nuevo);
}

void ajustar2(Dos_nodos dn)
{	 
	// si es raiz
	if (dn.n1.nodo_padre == -1){

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
		rtree_ultimo++;
		rtree_root = rtree_ultimo;
		// Antigua raiz 
		dn.n1.nodo_padre = rtree_ultimo;
		dn.n1.pos_mbr_padre = 0;
		actualizar_nodo(dn.n1);
		//
		dn.n2.nodo_padre = rtree_ultimo;
		dn.n2.pos_mbr_padre = 1;
		insertar_nodo(dn.n2);
		// Raiz
		Nodo aux;
		aux.nodo_id = rtree_ultimo;
		aux.nodo_padre = -1;
		aux.pos_mbr_padre = -1;
		aux.ultimo = 1;
		aux.mbr[0] = make_mbr_2(dn.mbr1, dn.n1.nodo_id);
		aux.mbr[1] = make_mbr_2(dn.mbr2, dn.n2.nodo_id);
		insertar_nodo(aux);
		
	}
	// si no es raíz
	else
	{	
		Nodo p = leer_nodo(dn.n1.nodo_padre);

		// si está llena
		if (p.ultimo == 2*T-1)
		{	int i;
			Nodo aux;
			p.mbr[dn.n1.pos_mbr_padre].rect = dn.mbr1;
			Dos_nodos dn_ = split(p, make_mbr_2(dn.mbr2,dn.n2.nodo_id));

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

			ajustar2(dn_);
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
			if (p.nodo_padre != -1)
				ajustar1(p.nodo_padre, p.pos_mbr_padre, calculate_mbr(dn.mbr1, dn.mbr2));
		}
	}
}


void insertar(Nodo nodo, Rectangulo rect)
{
	Nodo n = encontrar_hoja(nodo, rect);
	if (n.ultimo < 2*T-1)
	{
		n.ultimo++;
		n.mbr[n.ultimo] = make_mbr_2(rect,-1);
		actualizar_nodo(n);
		if (n.nodo_padre != -1)
			ajustar1(nodo.nodo_padre, nodo.pos_mbr_padre,rect);
	}
	else
	{	
		Dos_nodos dn = split(nodo, make_mbr_2(rect,-1));
		ajustar2(dn);
	}
}

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

