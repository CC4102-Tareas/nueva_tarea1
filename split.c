/**
    Realiza la división de un nodo en dos en tiempo cuadratico.
*/
Dos_nodos quadratic_split(Nodo nodo, MBR mbr)
{
	int i, j;           // iteradores
    float area_max;     // almacena el incremento de área máximo.
    float area_tmp;     // area calculada en cada iteración
    int rect1, rect2;   // indices del mbr que forman el incremento máximo.
    
    // ajustamos valores por default al primer par de rectángulos.
    // esto siempre es valido cuando T >= 1.
    area_max = incremento_area_split(nodo.mbr[0].rect, nodo.mbr[1].rect);
    rect1 = 0;
    rect2 = 1;

	// calculamos el área producida por cada par
    // esto es (N*(N-1))/2 con N=2*T+1
	for(i=0;i<(2*T);i++) {
		for(j=i+1;j<(2*T+1);j++) {
            // el rectangulo en la posición 2*T es rect. Caso especial.
            if (j == 2*T) {
                area_tmp = incremento_area_split(nodo.mbr[i].rect, mbr.rect);
            } else {
                area_tmp = incremento_area_split(nodo.mbr[i].rect, nodo.mbr[j].rect);
            }
            
            // se guardan los rectangulos que producen el incremento máximo.
            if (area_tmp > area_max) {
                area_max = area_tmp;
                rect1 = i;
                rect2 = j;
            }
        }
	}
    
    // aquí ya conocemos los rectangulos que generan el incremento de área máximo.
    // están en los indices rect1 y rect2

    Nodo nodo1, nodo2;

    nodo1.nodo_id = nodo.nodo_id;
    nodo1.nodo_padre = nodo.nodo_padre;
    nodo1.pos_mbr_padre = nodo.pos_mbr_padre;
    nodo1.ultimo = 0;
    nodo1.mbr[0] = nodo.mbr[rect1];

    // asignamos nuevo id al nuevo nodo
    rtree_ultimo++;
    nodo2.nodo_id = rtree_ultimo;
    // nodo2.nodo_padre = -> se asigna en insert;
    // nodo2.pos_mbr_padre = -> se asigna en insert;
    nodo2.ultimo = 0;
    if (rect2 == 2*T) {
        nodo2.mbr[0] = mbr;
    } else {
        nodo2.mbr[0] = nodo.mbr[rect2];
    }
    
    // mantener un rectangulo que representa el mbr que va en el padre para cada nodo.
    // esto es util para ir calculando el incremento de área en la iteración.
    Rectangulo mbr1, mbr2;
    float area_inc1, area_inc2;

    // en un principio será = al rectangulo inicial.
    mbr1 = nodo1.mbr[nodo1.ultimo].rect;
    mbr2 = nodo2.mbr[nodo2.ultimo].rect;

    // aquí ya tenemos los dos nuevos nodos con su rectangulo inicial. Además tenemos el mbr
    // de cada grupo.
    
    // iteramos sobre los elementos
    for(i=0;i<(2*T+1);i++) {
        // se saltan los escogidos al inicio.
        if (i != rect1 && i != rect2) {
            // si alguno completó su minimo. El indice comienza en 0, sino sería T+1
            if (nodo1.ultimo == T) {
                nodo2.ultimo++;
                if (i==2*T) { 
                    nodo2.mbr[nodo2.ultimo] = mbr;
                    mbr2 = calcular_mbr_minimo(mbr2, mbr.rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = calcular_mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
                continue;
            } else if (nodo2.ultimo == T) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo] = mbr;
                    mbr1 = calcular_mbr_minimo(mbr1, mbr.rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = calcular_mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
                continue;
            }

            // si debo procesar el rectangulo 2*T+1 dado que no fue asignado al inicio => lo proceso.
            if (i == 2*T) {
                area_inc1 = incremento_area_split(mbr1, mbr.rect);
                area_inc2 = incremento_area_split(mbr2, mbr.rect);
            } else {
                area_inc1 = incremento_area_split(mbr1, nodo.mbr[i].rect);
                area_inc2 = incremento_area_split(mbr2, nodo.mbr[i].rect);
            }
        
            if (area_inc1 < area_inc2) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo] = mbr;
                    mbr1 = calcular_mbr_minimo(mbr1, mbr.rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = calcular_mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
            } else {
                nodo2.ultimo++;
                if (i==2*T) {
                    nodo2.mbr[nodo2.ultimo] = mbr;
                    mbr2 = calcular_mbr_minimo(mbr2, mbr.rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = calcular_mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
            }
        }
    }
    
    // en este punto los dos nodos tienen todos los rectangulos asignados.
    // uno con T+1 y el otro con T.
    
    Dos_nodos resp;
    resp.n1 = nodo1;
    resp.n2 = nodo2;
    resp.mbr1 = mbr1;
    resp.mbr2 = mbr2;

    return resp;
}


Dos_nodos linear_split(Nodo nodo, MBR mbr) {
	
    int i; // itearador
    // se asume el primer mbr del nodo como el que tiene 
    // todas las características.
    int id_min_lado_mayor_x=0, id_max_lado_menor_x=0;
    int id_min_lado_mayor_y=0, id_max_lado_menor_y=0;
    
    // se usan valores por default del mbr 0.
    float max_eje_x = nodo.mbr[0].rect.x2, max_eje_y = nodo.mbr[0].rect.y2;

    float min_lado_mayor_x=nodo.mbr[0].rect.x2, max_lado_menor_x=nodo.mbr[0].rect.x1;
    float min_lado_mayor_y=nodo.mbr[0].rect.y2, max_lado_menor_y=nodo.mbr[0].rect.y1;

    float w_x, w_y;

    // indices de los rectángulos R1 y R2
    int rect1, rect2;
    // mbr que contiene a cada uno de los nodos nuevos
    Rectangulo mbr1, mbr2;

    for(i=1;i<(2*T+1);i++) {
        // ocupar rect
        if (i==2*T) {
            // buscamos el lado mayor mínimo en el eje X.
            if (mbr.rect.x2 < nodo.mbr[id_min_lado_mayor_x].rect.x2) {
                id_min_lado_mayor_x = i;
                min_lado_mayor_x = mbr.rect.x2;
            } 
            // buscamos el lado menor maximo en el eje x
            if (mbr.rect.x1 > nodo.mbr[id_max_lado_menor_x].rect.x1) {
                id_max_lado_menor_x = i;
                max_lado_menor_x = mbr.rect.x1;
            }
            // buscamos el lado mayor mínimo en el eje Y.
            if (mbr.rect.y2 < nodo.mbr[id_min_lado_mayor_y].rect.y2) {
                id_min_lado_mayor_y = i;
                min_lado_mayor_y = mbr.rect.y2;
            } 
            // buscamos el lado menor maximo en el eje Y
            if (mbr.rect.y1 > nodo.mbr[id_max_lado_menor_y].rect.y1) {
                id_max_lado_menor_y = i;
                max_lado_menor_y = mbr.rect.y1;
            }
            // buscamos el lado mayor maximo en el eje Y
            if (mbr.rect.y2 > max_eje_y) {
                max_eje_y = mbr.rect.y2;
            }
            // buscamos el lado mayor maximo en el eje X
            if (mbr.rect.x2 > max_eje_x) {
                max_eje_x = mbr.rect.x2;
            }
        } else {
            // buscamos el lado mayor mínimo en el eje X.
            if (nodo.mbr[i].rect.x2 < nodo.mbr[id_min_lado_mayor_x].rect.x2) {
                id_min_lado_mayor_x = i;
                min_lado_mayor_x = nodo.mbr[i].rect.x2;
            } 
            // buscamos el lado menor maximo en el eje x
            if (nodo.mbr[i].rect.x1 > nodo.mbr[id_max_lado_menor_x].rect.x1) {
                id_max_lado_menor_x = i;
                max_lado_menor_x = nodo.mbr[i].rect.x1;
            }
            // buscamos el lado mayor mínimo en el eje Y.
            if (nodo.mbr[i].rect.y2 < nodo.mbr[id_min_lado_mayor_y].rect.y2) {
                id_min_lado_mayor_y = i;
                min_lado_mayor_y = nodo.mbr[i].rect.y2;
            } 
            // buscamos el lado menor maximo en el eje Y
            if (nodo.mbr[i].rect.y1 > nodo.mbr[id_max_lado_menor_y].rect.y1) {
                id_max_lado_menor_y = i;
                max_lado_menor_y = nodo.mbr[i].rect.y1;
            }
            // buscamos el lado mayor maximo en el eje Y
            if (nodo.mbr[i].rect.y2 > max_eje_y) {
                max_eje_y = nodo.mbr[i].rect.y2;
            }
            // buscamos el lado mayor maximo en el eje X
            if (nodo.mbr[i].rect.x2 > max_eje_x) {
                max_eje_x = nodo.mbr[i].rect.x2;
            }
        }
    }

    // aquí tenemos todos los rectángulos. Calculamos w_x y w_y
    w_x = abs(max_lado_menor_x - min_lado_mayor_x)/max_eje_x;
    w_y = abs(max_lado_menor_y - min_lado_mayor_y)/max_eje_y;

    if(w_x > w_y) {
        rect1 = id_min_lado_mayor_x;
        rect2 = id_max_lado_menor_x;
    } else {
        rect1 = id_min_lado_mayor_y;
        rect2 = id_max_lado_menor_y;
    }
    // se hace cuando todos todos los rectangulos son iguales. En ese caso hay que hacerlos
    // distintos
    if(rect1 == rect2) {
        rect2++;
    }

    if(DEBUG_LINEAR_SPLIT) {
        printf("max_lado_menor_x: %f | min_lado_mayor_x: %f\n", max_lado_menor_x, min_lado_mayor_x);
        printf("max_lado_menor_y: %f | min_lado_mayor_y: %f\n", max_lado_menor_y, min_lado_mayor_y);
        printf("max_eje_x: %f | max_eje_y: %f\n", max_eje_x, max_eje_y);
        printf("w_x: %f | w_y: %f\n", w_x, w_y);
    
        printf("id_min_lado_mayor_x: %d | id_max_lado_menor_x: %d\n", id_min_lado_mayor_x, id_max_lado_menor_x);
        printf("id_min_lado_mayor_y: %d | id_max_lado_menor_y: %d\n", id_min_lado_mayor_y, id_max_lado_menor_y);
        printf("rect1: %d |rect2: %d\n", rect1, rect2);
    }
    // aquí ya conocemos los rectangulos que generan el incremento de área máximo.
    // están en los indices rect1 y rect2

    Nodo nodo1, nodo2;

    nodo1.nodo_id = nodo.nodo_id;
    nodo1.nodo_padre = nodo.nodo_padre;
    nodo1.pos_mbr_padre = nodo.pos_mbr_padre;
    nodo1.ultimo = 0;
    if (rect1 == 2*T) {
        nodo1.mbr[0] = mbr;
    } else {
        nodo1.mbr[0] = nodo.mbr[rect1];
    }

    rtree_ultimo++;
    nodo2.nodo_id = rtree_ultimo;
    //nodo2.nodo_padre = -> se asigna después
    //nodo2.pos_mbr_padre = -> se asigna después
    nodo2.ultimo = 0;
    if (rect2 == 2*T) {
        nodo2.mbr[0] = mbr;
    } else {
        nodo2.mbr[0] = nodo.mbr[rect2];
    }
    
    // mantener un rectangulo que representa el mbr que va en el padre para cada nodo.
    // en un principio será = al rectangulo inicial.
    mbr1 = nodo1.mbr[nodo1.ultimo].rect;
    mbr2 = nodo2.mbr[nodo2.ultimo].rect;

    // aquí ya tenemos los dos nuevos nodos con su rectangulo inicial. Además tenemos el mbr
    // de cada grupo.
    
    // iteramos sobre los elementos
    for(i=0;i<(2*T+1);i++) {
        // se saltan los escogidos al inicio.
        if (i != rect1 && i != rect2) {
            // si alguno completó su minimo. Se iguala a T pq el índice comienza en 0. sino sería T+1
            if (nodo1.ultimo == T) {
                nodo2.ultimo++;
                if (i==2*T) {           
                    nodo2.mbr[nodo2.ultimo] = mbr;
                    mbr2 = calcular_mbr_minimo(mbr2, mbr.rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = calcular_mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
                continue;
            } else if (nodo2.ultimo == T) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo] = mbr;
                    mbr1 = calcular_mbr_minimo(mbr1, mbr.rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = calcular_mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
                continue;
            }

            float area_inc1, area_inc2;

            // si debo procesar el rectangulo 2*T+1 dado que no fue asignado al inicio => lo proceso.
            if (i == 2*T) {
                area_inc1 = incremento_area_split(mbr1, mbr.rect);
                area_inc2 = incremento_area_split(mbr2, mbr.rect);
            } else {
                area_inc1 = incremento_area_split(mbr1, nodo.mbr[i].rect);
                area_inc2 = incremento_area_split(mbr2, nodo.mbr[i].rect);
            }
        
            //TODO: verificar si falta una condición aquí.
            if (area_inc1 < area_inc2) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo] = mbr;
                    mbr1 = calcular_mbr_minimo(mbr1, mbr.rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = calcular_mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
            }else if (area_inc1 > area_inc2) {
                nodo2.ultimo++;
                if (i==2*T) {
                    nodo2.mbr[nodo2.ultimo] = mbr;
                    mbr2 = calcular_mbr_minimo(mbr2, mbr.rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = calcular_mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
            // son iguales. Se lleva al grupo con menor área.
            } else if (area(mbr1) < area(mbr2)) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo] = mbr;
                    mbr1 = calcular_mbr_minimo(mbr1, mbr.rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = calcular_mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
            } else if (area(mbr1) > area(mbr2)) {
                nodo2.ultimo++;
                if (i==2*T) {
                    nodo2.mbr[nodo2.ultimo] = mbr;
                    mbr2 = calcular_mbr_minimo(mbr2, mbr.rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = calcular_mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
            // son iguales. Se lleva al con menor n° de rectángulos.
            } else if(nodo1.ultimo < nodo2.ultimo) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo] = mbr;
                    mbr1 = calcular_mbr_minimo(mbr1, mbr.rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = calcular_mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
            } else if(nodo1.ultimo > nodo2.ultimo) { 
                nodo2.ultimo++;
                if (i==2*T) {
                    nodo2.mbr[nodo2.ultimo] = mbr;
                    mbr2 = calcular_mbr_minimo(mbr2, mbr.rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = calcular_mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
            // decisión arbitraria. Insertar en primer nodo
            } else {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo] = mbr;
                    mbr1 = calcular_mbr_minimo(mbr1, mbr.rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = calcular_mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
            }
        }
    }

    // en este punto los dos nodos tienen todos los rectangulos asignados.
    // uno con T+1 y el otro con T.
    
    Dos_nodos resp;
    resp.n1 = nodo1;
    resp.n2 = nodo2;
    resp.mbr1 = mbr1;
    resp.mbr2 = mbr2;

    return resp;
}

