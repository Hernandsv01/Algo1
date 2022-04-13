#include "la_sombra_de_mac.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

const char ARRIBA = 'W';
const char IZQUIERDA = 'A';
const char ABAJO = 'S';
const char DERECHA = 'D';

const char MAC = 'M';
const char BLUE = 'B';
const char VACIO = '-';

bool coordenadas_iguales(coordenada_t a, coordenada_t b){
	if(a.fila == b.fila && a.col == b.col){
		return true;
	}else{
		return false;
	}
}

void inicializar_personaje(personaje_t* ref_personaje, coordenada_t arranque_personaje){
	ref_personaje->posicion = arranque_personaje;
	ref_personaje->vida = 3;
	ref_personaje->puntos = 0;
	ref_personaje->tiene_llave = false;
	ref_personaje->interruptor_apretado = false;
}

void inicializar_sombra(sombra_t* ref_sombra, coordenada_t arranque_sombra){
	ref_sombra->posicion = arranque_sombra;
	ref_sombra->esta_viva = true;
}

bool es_movimiento_valido(char movimiento){
	if(movimiento == ARRIBA || movimiento == IZQUIERDA || movimiento == ABAJO || movimiento == DERECHA){
		return true;
	}else{
		return false;
	}
}

void pedir_movimiento(char* ref_movimiento){
	do{
		scanf("%c", ref_movimiento);
	}while(!es_movimiento_valido(*ref_movimiento));
}

bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho){
	if(posicion.fila >= max_alto || posicion.fila < 0 || posicion.col >= max_ancho || posicion.col < 0){
		return false;
	}else{
		return true;
	}
}

void mover_personaje(personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento){
	coordenada_t pos_inicial_personaje = ref_personaje->posicion;
	coordenada_t pos_inicial_sombra = ref_sombra->posicion;

	if(movimiento == ARRIBA){
		ref_personaje->posicion.fila--;
		ref_sombra->posicion.fila--;
	}else if(movimiento == ABAJO){
		ref_personaje->posicion.fila++;
		ref_sombra->posicion.fila++;
	}else if(movimiento == IZQUIERDA){
		ref_personaje->posicion.col--;
		ref_sombra->posicion.col++;
	}else if(movimiento == DERECHA){
		ref_personaje->posicion.col++;
		ref_sombra->posicion.col--;
	}

	if(!esta_dentro_rango(ref_personaje->posicion, MAX_FILAS, MAX_COLUMNAS)){
		ref_personaje->posicion = pos_inicial_personaje;
	}
	if(!esta_dentro_rango(ref_sombra->posicion, MAX_FILAS, MAX_COLUMNAS)){
		ref_sombra->posicion = pos_inicial_sombra;
	}
}

coordenada_t coordenada_aleatoria(int max_alto, int max_ancho){
	coordenada_t coordenada;
	coordenada.fila = rand() % max_alto;
	coordenada.col = rand() % max_ancho;
	return coordenada;
}

int buscar_elemento(elemento_t elementos[MAX_ELEMENTOS], int tope, coordenada_t posicion){
	int i = 0;
	int indice_elemento = -1;

	while(indice_elemento == -1 && i < tope){
		if(coordenadas_iguales(elementos[i].posicion, posicion)){
			indice_elemento = i;
		}
		i++;
	}

	return indice_elemento;
}

void imprimir_terreno(juego_t juego){
	for(int i = 0; i < MAX_FILAS; i++){
		for(int j = 0; j < MAX_COLUMNAS; j++){
			coordenada_t coordenada_a_imprimir = {i, j};
			if(coordenadas_iguales(coordenada_a_imprimir, juego.personaje.posicion)){
				printf("%c ", MAC);
			}else if(coordenadas_iguales(coordenada_a_imprimir, juego.sombra.posicion)){
				printf("%c ", BLUE);
			}else{
				printf("%c ", VACIO);
			}
		}
		printf("\n");
	}
}
