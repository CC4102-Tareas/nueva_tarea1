#define TRUE 1
#define FALSE 0
/**
	Procedimiento basado en http://stackoverflow.com/questions/13390333/two-rectangles-intersection	
	verifica si existe una intersección entre 2 rectángulos dados.
*/

float flmin(float a, float b)
{
	return (((a) < (b)) ? (a) : (b));
}

float flmax(float a, float b)
{
	return (((a) > (b)) ? (a) : (b));
}


int interseccion (Rectangulo r1, Rectangulo r2) 
{
	if  (r2.x1 >= r1.x2 || 
		  r2.x2 <= r1.x1 || 
		  r2.y2 <= r1.y1 ||
		  r2.y1 >= r1.y2)
		return 0;
	return 1;
}

/**
	Entrega el área de un rectángulo.
*/
float area(Rectangulo rect) 
{
	float ancho = rect.x2-rect.x1;
	float alto = rect.y2 - rect.y1;
	return ancho*alto;
}

/**
	Calcula el incremento de área de un mbr.
*/

Rectangulo calculate_mbr(Rectangulo r1, Rectangulo r2)
{
	Rectangulo r;

	r.x1 = flmin(r1.x1,r2.x1);
	r.x2 = flmax(r1.x2,r2.x2);
	r.y1 = flmin(r1.y1,r2.y1);
	r.y2 = flmax(r1.y2,r2.y2);
	return r;
}

float incremento_area(Rectangulo r1, Rectangulo r2) {
	Rectangulo mbrect = calculate_mbr(r1,r2);
	return area(mbrect) - area(r1);
}


float incremento_area_qs(Rectangulo r1, Rectangulo r2) {
	Rectangulo mbrect = calculate_mbr(r1,r2);
	return area(mbrect) - area(r1) - area(r2);
}
