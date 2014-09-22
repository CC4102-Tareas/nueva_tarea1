
/**
	Estructura que almacena un arreglo dinámico de Rectangulos.
	Obtenido desde: http://stackoverflow.com/questions/3536153/c-dynamically-growing-array
*/
typedef struct {
  Rectangulo *array; // puntero a un arreglo de rectangulos.
  size_t used;       // cantidad de elementos presentes.
  size_t size;       // tamaño.
} Dynamic_array;

void initArray(Dynamic_array *a, size_t initialSize) {
  a->array = (Rectangulo *)malloc(initialSize * sizeof(Rectangulo));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(Dynamic_array *a, Rectangulo element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (Rectangulo *)realloc(a->array, a->size * sizeof(Rectangulo));
  }
  a->array[a->used++] = element;
}

void freeArray(Dynamic_array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}
