#define TAMANO_PAGINA 4096 // tamaño de página del disco duro.
#define T 2              // mínimo de hijos que puede tener un nodo (salvo la raíz)
int rtree_ultimo = -1;
int rtree_root = -1;
/**
	Definición de punto (x,y)
*/
typedef struct
{
	float d1;
	float d2;
} Par;

/**

	*-------------*
	|             |
	| Rectángulo  |
	|             |
	*-------------*
	
	[x1,x2] x [y1,y2]

*/
typedef struct
{
	float x1; 
	float x2;
	float y1;
	float y2;

} Rectangulo;

/**
	Minimun Bounding Rectangule
*/
typedef struct 
{
	Rectangulo rect; // MBR
	int nodo_hijo;   // posición del nodo en el archivo.
} MBR;

/**
	Nodo del R-Tree.
*/
typedef struct 
{
	int nodo_id;			// identificador de nodo. Es equivalente a la posición del nodo en el archivo.
	int nodo_padre;   	// posición del nodo padre en el archivo.
	int pos_mbr_padre;	// posición del MBR padre en el nodo padre(índice del arreglo MBR).
	int ultimo;       	// último indice ocupado en mbr.
	MBR mbr[4];			// arreglo de MBR's
	MBR mbr_[2*102-4];	// arreglo de MBR's relleno
	//MBR mbr[2*T];	      // arreglo de MBR's
} Nodo;

typedef struct 
{
	Nodo n1;
	Nodo n2;
	Rectangulo mbr1;
	Rectangulo mbr2;

} Dos_nodos;

Rectangulo make_rect(float x1, float x2, float y1, float y2)
{
	Rectangulo r;

	r.x1 = x1;
	r.x2 = x2;
	r.y1 = y1;
	r.y2 = y2;

	return r;
}

MBR make_mbr(float x1, float x2, float y1, float y2, int hijo)
{
	Rectangulo r;
	MBR m;
	r = make_rect(x1,x2,y1,y2);
	m.rect = r;
	m.nodo_hijo = hijo;
	return m;
}


MBR make_mbr_2(Rectangulo rect, int hijo)
{
	MBR m;
	m.rect = rect;
	m.nodo_hijo = hijo;
	return m;
}
