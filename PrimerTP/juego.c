#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "la_sombra_de_mac.h"

/*
 * Pre: Recibe un 1 o un 0
 * Post: Imprime true o false dependiendo del valor recibido
 */
void imprimir_boolean(int boolean){
	if(boolean == 0){
		printf("false \n");
	}else{
		printf("true \n");
	}
}

/*
 * Pre: Recibe un personaje con ciertos valores que no sean basura
 * Post: Imprime todos los datos del personaje, ordenados y especificados
 */
void imprimir_personaje(personaje_t personaje){
	printf(" - Posición: %i-%i \n", personaje.posicion.col, personaje.posicion.fila);
	printf(" - Vidas: %i \n", personaje.vida);
	printf(" - Puntos: %i \n", personaje.puntos);
	printf(" - Tiene_llave: ");
	imprimir_boolean(personaje.tiene_llave);
	printf(" - Interruptor_apretado: ");
	imprimir_boolean(personaje.interruptor_apretado);
}

/*
 * Pre: Recibe una sombra con ciertos valores que no sean basura
 * Post: Imprime todos los datos de la sombra, ordenados y especificados
 */
void imprimir_sombra(sombra_t sombra){
	printf(" - Posición: %i-%i \n", sombra.posicion.col, sombra.posicion.fila);
	printf(" - Esta_viva: ");
	imprimir_boolean(sombra.esta_viva);
}


/* -------------------------------------------------------------------------- */
/*                              EJEMPLOS CONSIGNA                             */
/* -------------------------------------------------------------------------- */
/*
 * Pre: Ninguna
 * Post: Imprime los ejemplos de la consigna del TP
 */
void test_ejemplos(){
	/* ---------------------------Ejemplo 1 - Coordenadas iguales--------------------------- */
	printf("\n1) Test coordenadas iguales\n");
	coordenada_t primera_coordenada;
	coordenada_t segunda_coordenada;

	primera_coordenada.fila = 3;
	primera_coordenada.col = 2;
	segunda_coordenada.fila = 3;
	segunda_coordenada.col = 2;
	printf("	- %i-%i es igual a %i-%i? ", primera_coordenada.fila, primera_coordenada.col, segunda_coordenada.fila, segunda_coordenada.col);
	imprimir_boolean(coordenadas_iguales(primera_coordenada, segunda_coordenada));

	primera_coordenada.fila = 1;
	primera_coordenada.col = 2;
	segunda_coordenada.fila = 3;
	segunda_coordenada.col = 4;
	printf("	- %i-%i es igual a %i-%i? ", primera_coordenada.fila, primera_coordenada.col, segunda_coordenada.fila, segunda_coordenada.col);
	imprimir_boolean(coordenadas_iguales(primera_coordenada, segunda_coordenada));


	/* ---------------------------Ejemplo 2 - Esta dentro de rango--------------------------- */
	printf("\n2) Test esta dentro de rango\n");
	coordenada_t coordenada_personaje;
	
	coordenada_personaje.fila = -1;
	coordenada_personaje.col = 31;
	printf("	- (%i)-(%i) esta dentro del rango? ", coordenada_personaje.fila, coordenada_personaje.col);
	imprimir_boolean(esta_dentro_rango(coordenada_personaje, MAX_FILAS, MAX_COLUMNAS));
	
	coordenada_personaje.fila = 1;
	coordenada_personaje.col = 1;
	printf("	- %i-%i esta dentro del rango? ", coordenada_personaje.fila, coordenada_personaje.col);
	imprimir_boolean(esta_dentro_rango(coordenada_personaje, MAX_FILAS, MAX_COLUMNAS));


	/* ---------------------------Ejemplo 3 - Es movimiento valido--------------------------- */
	printf("\n3) Test es movimiento válido\n");
	char movimiento = 'W';
	printf("	- %c es un movimiento válido? ", movimiento);
	imprimir_boolean(es_movimiento_valido(movimiento));
	
	movimiento = 'F';
	printf("	- %c es un movimiento válido? ", movimiento);
	imprimir_boolean(es_movimiento_valido(movimiento));

	
	/* ------------------------------Ejemplo 4 - Buscar elemento------------------------------ */
	printf("\n4) Test buscar elemento\n");
	elemento_t elementos [3];
	// elementos[0].posicion = (coordenada_t){1, 1};
	elementos[0].posicion.fila = 1;
	elementos[0].posicion.col = 1;
	elementos[0].tipo = 'A';
	// elementos[1].posicion = (coordenada_t){2, 2};
	elementos[1].posicion.fila = 2;
	elementos[1].posicion.col = 2;
	elementos[1].tipo = 'A';
	// elementos[2].posicion = (coordenada_t){3, 3};
	elementos[2].posicion.fila = 3;
	elementos[2].posicion.col = 3;
	elementos[2].tipo = 'A';
	int tope = 3;

	coordenada_t pos_buscado;
	pos_buscado.fila = 2;
	pos_buscado.col = 2;
	printf("	- El elemento se encuentra en el índice: %i \n", buscar_elemento(elementos, tope, pos_buscado));

	pos_buscado.fila = 5;
	pos_buscado.col = 0;
	printf("	- El elemento se encuentra en el índice: %i \n", buscar_elemento(elementos, tope, pos_buscado));
}

/* -------------------------------------------------------------------------- */
/*                          TEST FUNCIONES RESTANTES                          */
/* -------------------------------------------------------------------------- */
/*
 * Pre: Ninguna
 * Post: Imprime test de las funciones que no son probadas en los ejemplos
 */
void test_funciones(){
	/* ----------------------- TEST INICIALIZAR PERSONAJE ----------------------- */
	printf("\nTest inicializar personaje\n");
	personaje_t personaje;
	coordenada_t coordenada_personaje = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
	inicializar_personaje(&personaje, coordenada_personaje);
	imprimir_personaje(personaje);

	/* ------------------------- TEST INICIALIZAR SOMBRA ------------------------ */
	printf("\nTest inicializar sombra\n");
	sombra_t sombra;
	coordenada_t coordenada_sombra = {coordenada_personaje.fila, (MAX_COLUMNAS - 1) - coordenada_personaje.col};
	inicializar_sombra(&sombra, coordenada_sombra);
	imprimir_sombra(sombra);

	/* ------------------------ TEST COORDENADA ALEATORIA ----------------------- */
	printf("\nTest coordenada aleatoria\n");
	coordenada_t coor_random1 = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
	printf(" - Coordenada aleatoria 1: %i-%i \n", coor_random1.fila, coor_random1.col);
	coordenada_t coor_random2 = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
	printf(" - Coordenada aleatoria 2: %i-%i \n", coor_random2.fila, coor_random2.col);

	/* -------------------------- TEST PEDIR MOVIMIENTO ------------------------- */
	printf("\nTest pedir movimiento\n");
	char movimiento;
	pedir_movimiento(&movimiento);
	printf("Movimiento válido elegido: %c \n", movimiento);

	/* ----------------------------- TEST MOVER PERSONAJE ---------------------------- */
	printf("\nTest mover personaje\n");
	printf(" - Posiciones antes de mover: \n    - Personaje: %i-%i\n    - Sombra: %i-%i \n", 
		personaje.posicion.fila, 
		personaje.posicion.col, 
		sombra.posicion.fila, 
		sombra.posicion.col
	);
	mover_personaje(&personaje, &sombra, movimiento);
	printf(" - Nuevas posiciones: \n    - Personaje: %i-%i\n    - Sombra: %i-%i \n", 
		personaje.posicion.fila, 
		personaje.posicion.col, 
		sombra.posicion.fila, 
		sombra.posicion.col
	);


	/* -------------------------- TEST IMPRIMIR TERRENO ------------------------- */
	printf("\nTest imprimir terreno\n");
	juego_t juego = {personaje, sombra};
	imprimir_terreno(juego);

}

/* -------------------------------------------------------------------------- */
/*                                 TEST JUEGO                                 */
/* -------------------------------------------------------------------------- */
/*
 * Pre: Ninguna
 * Post: Inicializa un juego funcional
 */
void jugar(){
	//Declarar variables del juego
	juego_t juego;
	personaje_t personaje;
	sombra_t sombra;

	//Generar coordenadas de personajes
	coordenada_t coordenada_inicial_mac = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
	coordenada_t coordenada_inicial_boo = {coordenada_inicial_mac.fila, (MAX_COLUMNAS - 1) - coordenada_inicial_mac.col};

	//inicializar variables de personajes
	inicializar_personaje(&personaje, coordenada_inicial_mac);
	inicializar_sombra(&sombra, coordenada_inicial_boo);
	juego.personaje = personaje;
	juego.sombra = sombra;

	//Bucle de juego (Usar Ctrl+C para terminar el programa)
	while(true){
		//Limpiar e imprimir cosas
		system("clear");
		imprimir_terreno(juego);
		printf("Ingrese un movimiento (W, A, S, D): ");

		//Pedir movimiento y mover
		char movimiento_elegido;
		pedir_movimiento(&movimiento_elegido);
		mover_personaje(&juego.personaje, &juego.sombra, movimiento_elegido);
	}
}


int main(){
	srand ((unsigned)time(NULL));

	int opcion;
	printf("Elija una opción (ingresar solo el número)\n");
	printf("	1. Probar ejemplos de la consigna\n");
	printf("	2. Probar funciones que no están en los ejemplos\n");
	printf("	3. Probar intento de juego\n");
	scanf("%i", &opcion);
	switch(opcion){
		case 1:
			test_ejemplos();
			break;
		case 2:
			test_funciones();
			break;
		case 3:
			jugar();
			break;
		default:
			main();
	}

	return 0;
}