/**
	Estructura que permite persistir un R-tree. 
*/

// activar para casos de debug.
#define DEBUG_DISCO FALSE
#define NAME_FILE "r-tree.bin"

int lecturas_disco = 0;
int inserciones_disco = 0;
int actualizaciones_disco = 0;

/**
    Indica la cantidad de accesos a disco que se han realizado.
*/
int accesos_disco() {
    return lecturas_disco + inserciones_disco + actualizaciones_disco;
}

/**
	Leer nodo de un r-tree
*/

Nodo leer_nodo(int numero_nodo) {
	FILE *fptr;
	
    lecturas_disco++;

    // se abre el archivo para lectura
	fptr = fopen(NAME_FILE, "rb");

	if(fseek(fptr, numero_nodo*TAMANO_PAGINA, SEEK_SET)) {
		printf("Error al intentar posicionarse en la página.\n");	
	}

	Nodo nodo;

	//crear nodo con los datos leidos
	fread(&(nodo), sizeof(Nodo), 1, fptr);

	fclose(fptr);
    
   	((DEBUG_DISCO) ? printf("El nodo %d ha sido leido desde disco.\n", nodo.nodo_id):0);

	return nodo;
}

/**
	inserta un nodo en el R-tree
*/
void insertar_nodo(Nodo nodo) {
	FILE *fptr;
    
    inserciones_disco++;

	// se abre el archivo para lectura
	fptr = fopen(NAME_FILE, "a+b");

	fwrite(&(nodo), sizeof(Nodo), 1, fptr);
	fclose(fptr);

   	((DEBUG_DISCO) ? printf("El nodo %d ha sido insertado en disco.\n", nodo.nodo_id):0);
}

/**
	actualiza un nodo
*/
actualizar_nodo(Nodo nodo) {
	FILE *fptr;

    actualizaciones_disco++;

	// se abre el archivo para lectura
	fptr = fopen(NAME_FILE, "r+b");

	if(fseek(fptr, nodo.nodo_id*TAMANO_PAGINA, SEEK_SET)) {
		printf("Error al intentar posicionarse en la página.");
	}

	fwrite(&(nodo), sizeof(Nodo), 1, fptr);

	fclose(fptr);

   	((DEBUG_DISCO) ? printf("El nodo %d ha sido actualizado en disco.\n", nodo.nodo_id):0);
}


/**
	Inicializa un r-tree.
	Crea un archivo de nombre 'r-tree.bin' con un nodo raíz
*/
void init_rtree() {
	FILE *fptr;

	// crea el archivo
	fptr = fopen(NAME_FILE, "wb");
	fclose(fptr);
	
	Nodo nodo;
	nodo.nodo_id = 0;
	nodo.nodo_padre = -1;    // indica que es la raiz
	nodo.pos_mbr_padre = -1; // también indica que es la raíz
	nodo.ultimo = -1;        // no tiene MBRs
	nodo.mbr[0].nodo_hijo = -1; // como es el único, es una hoja.

    // se actualiza el indice de la raiz y el ultimo identificador usado.
	rtree_root = 0;     
	rtree_ultimo = 0;
	
	insertar_nodo(nodo);
}
