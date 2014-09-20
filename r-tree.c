/**
	Estructura que permite persistir un R-tree. 
*/

#define NAME_FILE "r-tree.bin"

/**
	Leer nodo de un r-tree
*/

Nodo leer_nodo(int numero_nodo) {
	FILE *fptr;
	
	// se abre el archivo para lectura
	fptr = fopen(NAME_FILE, "rb");

	if(fseek(fptr, numero_nodo*TAMANO_PAGINA, SEEK_SET)) {
		printf("Error al intentar posicionarse en la página.");	
	}

	Nodo nodo;

	//crear nodo con los datos leidos
	fread(&(nodo), sizeof(Nodo), 1, fptr);

	fclose(fptr);

	return nodo;
}

/**
	inserta un nodo en el R-tree
*/
void insertar_nodo(Nodo nodo) {
	FILE *fptr;

	// se abre el archivo para lectura
	fptr = fopen(NAME_FILE, "a+b");

	// se posiciona al final del archivo
	//if(fseek(fptr, 0, SEEK_END)) {
	//	printf("Error al intentar posicionarse en la página.");

	//}
	fwrite(&(nodo), sizeof(Nodo), 1, fptr);
	fclose(fptr);
}

/**
	actualiza un nodo
*/
actualizar_nodo(Nodo nodo) {
	FILE *fptr;

	// se abre el archivo para lectura
	fptr = fopen(NAME_FILE, "r+b");

	if(fseek(fptr, nodo.nodo_id*TAMANO_PAGINA, SEEK_SET)) {
		printf("Error al intentar posicionarse en la página.");
	}


	fwrite(&(nodo), sizeof(Nodo), 1, fptr);

	fclose(fptr);
}


/**
	Inicializa un r-tree.
	Crea un archivo de nombre 'r-tree.bin' y retorna el
	nodo raiz del r.tree
*/
void init_rtree() {
	FILE *fptr;
	int status;
 
	status = remove(NAME_FILE);
 
	if(status == 0)
		printf("%s archivo eliminado.\n", NAME_FILE);
	else {	
		//printf("Incapaz de eliminar archivo.\n");
		//perror("Error");
	}

	// crea el archivo
	fptr = fopen(NAME_FILE, "wb");
	fclose(fptr);

	
	Nodo nodo;
	nodo.nodo_id = 0;
	nodo.nodo_padre = -1;
	nodo.pos_mbr_padre = -1;
	nodo.ultimo = -1;
	nodo.mbr[0].nodo_hijo = -1;
	rtree_root = 0;
	rtree_ultimo = 0;
	
	insertar_nodo(nodo);

}
