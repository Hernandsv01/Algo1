#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "la_sombra_de_mac.h"
#include "utiles.h"

#define MAX_MENSAJES 1000

char ganaste[MAX_MENSAJES] = " \
 ######      ###    ##    ##    ###     ######  ######## ######## \n \
##    ##    ## ##   ###   ##   ## ##   ##    ##    ##    ##       \n \
##         ##   ##  ####  ##  ##   ##  ##          ##    ##       \n \
##   #### ##     ## ## ## ## ##     ##  ######     ##    ######   \n \
##    ##  ######### ##  #### #########       ##    ##    ##       \n \
##    ##  ##     ## ##   ### ##     ## ##    ##    ##    ##       \n \
 ######   ##     ## ##    ## ##     ##  ######     ##    ######## \n";

char perdiste[MAX_MENSAJES] = " \
########  ######## ########  ########  ####  ######  ######## ######## \n \
##     ## ##       ##     ## ##     ##  ##  ##    ##    ##    ##       \n \
##     ## ##       ##     ## ##     ##  ##  ##          ##    ##       \n \
########  ######   ########  ##     ##  ##   ######     ##    ######   \n \
##        ##       ##   ##   ##     ##  ##        ##    ##    ##       \n \
##        ##       ##    ##  ##     ##  ##  ##    ##    ##    ##       \n \
##        ######## ##     ## ########  ####  ######     ##    ######## \n";

/*
 * Pre: Recibe un booleano
 * Post: Imprime true o false dependiendo de lo recibido
 */
void imprimir_boolean(bool booleano){
	if(booleano == true){
		printf("true \n");
	}else{
		printf("false \n");
	}
}

/*
 * Pre: Recibe un personaje ya inicializado
 * Post: Imprime todos los datos del personaje
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
 * Pre: Recibe una sombra ya inicializada
 * Post: Imprime todos los datos de la sombra
 */
void imprimir_sombra(sombra_t sombra){
	printf(" - Posición: %i-%i \n", sombra.posicion.col, sombra.posicion.fila);
	printf(" - Esta_viva: ");
	imprimir_boolean(sombra.esta_viva);
}

/*
 * Pre: Recibe un vector de niveles ya inicializados
 * Post: Imprime todos los datos de de cada nivel
 */
void imprimir_niveles(nivel_t niveles[MAX_NIVELES], bool con_detalles){
	for(int i = 0; i < MAX_NIVELES; i++){
		printf("\nNumero de nivel: %i\n", niveles[i].numero_nivel);
		
		printf(" - Paredes(%i):\n", niveles[i].tope_paredes);
		if(con_detalles){
			for(int j = 0; j < niveles[i].tope_paredes; j++){
				printf("    - Pared en %i-%i\n", niveles[i].paredes[j].fila, niveles[i].paredes[j].col);
			}
		}

		printf(" - Obstaculos(%i):\n", niveles[i].tope_obstaculos);
		if(con_detalles){
			for(int j = 0; j < niveles[i].tope_obstaculos; j++){
				printf("    - %c en %i-%i\n", niveles[i].obstaculos[j].tipo, niveles[i].obstaculos[j].coordenada.fila, niveles[i].obstaculos[j].coordenada.col);
			}
		}

		printf(" - Herramientas(%i):\n", niveles[i].tope_herramientas);
		if(con_detalles){
			for(int j = 0; j < niveles[i].tope_herramientas; j++){
				printf("    - %c en %i-%i\n", niveles[i].herramientas[j].tipo, niveles[i].herramientas[j].coordenada.fila, niveles[i].herramientas[j].coordenada.col);
			}
		}
	}
}

/*
 * Pre: Recibe un juego ya inicializado
 * Post: Imprime todos los datos del juego
 */
void imprimir_juego(juego_t juego, bool con_detalles){
	printf("Personaje:\n");
	imprimir_personaje(juego.personaje);
	printf("\nSombra:\n");
	imprimir_sombra(juego.sombra);
	printf("\n\nNiveles:\n");
	imprimir_niveles(juego.niveles, con_detalles);
	printf("\nNivel actual:\n");
	printf(" - %i\n", juego.nivel_actual);
}

/*
 * Pre: Recibe un juego ya inicializado
 * Post: Ejecuta el juego hasta terminarlo o quedarse sin vidas
 */
void correr_juego(juego_t juego){

	while(estado_juego(juego) == 0){
		
		while(estado_nivel(juego) == 0){
			system("clear");
			imprimir_terreno(juego);
			realizar_jugada(&juego);

			if(estado_juego(juego) == -1){
				break;
			}
		}
	}

	system("clear");
	if(estado_juego(juego) == 1){
		printf("%s\n", ganaste);
	}else{
		printf("%s\n", perdiste);
	}
}

int main(){
	srand ((unsigned)time(NULL));

	juego_t juego;
	inicializar_juego(&juego);

	printf("Elija una opción:\n");
	printf("	1. Ver info del juego\n");
	printf("	2. Ver info del juego en detalle (No recomendado)\n");
	printf("	3. Jugar\n");
	int opcion;
	scanf("%i", &opcion);

	switch(opcion){
		case 1:
			imprimir_juego(juego, false);
			break;
		case 2:
			imprimir_juego(juego, true);
			break;
		case 3:
			correr_juego(juego);
			break;
		default:
			main();
			break;
	}

	return 0;
}