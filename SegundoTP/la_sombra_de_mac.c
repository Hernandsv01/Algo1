#include "la_sombra_de_mac.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utiles.h"

const char ARRIBA = 'W';
const char IZQUIERDA = 'A';
const char ABAJO = 'S';
const char DERECHA = 'D';
const char VIDA = 'V';

const char MAC = 'M';
const char BLUE = 'B';
const char VACIO = '-';
const char PUERTA = 'D';
const char ESCALERA = 'E';
const char LLAVE = 'L';
const char MONEDA = 'C';
const char POZO = 'W';
const char INTERRUPTOR = 'O';
const char PORTAL = 'P';
const char VELA = 'V';
const char PARED = 'X';

const int NADA_ENCONTRADO = -1;

#define COLOR_PERSONAJE "\e[31m" //Rojo
#define COLOR_SOMBRA "\e[34m" //Azul
#define COLOR_PUERTA "\e[35m" //Violeta
#define COLOR_LLAVE "\e[35m" //Violeta
#define COLOR_MONEDAS "\e[33m" //Amarillo
#define COLOR_DEFAULT "\e[37m" //Blanco

const int VIDA_INICIAL_PERSONAJE = 3;
const int PUNTOS_INICIALES_PERSONAJE = 0;
const int NIVEL_INICIAL = 1;

const int COSTO_REVIVIR_SOMBRA = 50;

const int MIN_VALOR_MONEDA = 10;
const int RANGO_VALOR_MONEDA = 11;

//Valores nivel 1
const int MAX_VELAS_NVL_1 = 5;
const int MAX_POZOS_NVL_1 = 15;
const int MAX_INTERRUPTORES_NVL_1 = 1;
const int MAX_PORTALES_NVL_1 = 0;
const int MAX_ESCALERAS_NVL_1 = 10;
const int MAX_MONEDAS_NVL_1 = 10;
const int MAX_LLAVES_NVL_1 = 0;

//Valores nivel 2
const int MAX_VELAS_NVL_2 = 10;
const int MAX_POZOS_NVL_2 = 20;
const int MAX_INTERRUPTORES_NVL_2 = 2;
const int MAX_PORTALES_NVL_2 = 2;
const int MAX_ESCALERAS_NVL_2 = 15;
const int MAX_MONEDAS_NVL_2 = 15;
const int MAX_LLAVES_NVL_2 = 1;

//Valores nivel 3
const int MAX_VELAS_NVL_3 = 12;
const int MAX_POZOS_NVL_3 = 30;
const int MAX_INTERRUPTORES_NVL_3 = 4;
const int MAX_PORTALES_NVL_3 = 4;
const int MAX_ESCALERAS_NVL_3 = 15;
const int MAX_MONEDAS_NVL_3 = 15;
const int MAX_LLAVES_NVL_3 = 1;

/*
 * Pre: Recibe un alto y ancho mayores a 0
 * Post: Devuelve una coordenada_t con valores aleatorios, mayores a 0 y menores a los límites dados
 */
coordenada_t coordenada_aleatoria(int max_alto, int max_ancho){
	coordenada_t coordenada;
	coordenada.fila = rand() % max_alto;
	coordenada.col = rand() % max_ancho;
	return coordenada;
}

/*
 * Pre: Recibe dos coordenadas válidas
 * Post: Devuelve true si son iguales o false i son diferentes
 */
bool coordenadas_iguales(coordenada_t a, coordenada_t b){
	if(a.fila == b.fila && a.col == b.col){
		return true;
	}else{
		return false;
	}
}

/*
 * Pre: Recibe un vector de elementos, su respectivo tope y una coordenada valida
 * Post: devuelve la posición del elemento que coincida con la coordenada dada, caso contrario devuelve -1
 */
int buscar_elemento(elemento_t elementos[MAX_ELEMENTOS], int tope, coordenada_t posicion){
	int i = 0;
	int indice_elemento = NADA_ENCONTRADO;

	while(indice_elemento == NADA_ENCONTRADO && i < tope){
		if(coordenadas_iguales(elementos[i].coordenada, posicion)){
			indice_elemento = i;
		}
		i++;
	}

	return indice_elemento;
}

/*
 * Pre: Recibe un nivel inicializado o en proceso de inicialización
 * Post: Devuelve un espacio vacío aleatorio del nivel
 */
coordenada_t buscar_vacio_nivel(nivel_t nivel){
	coordenada_t coordenada;
	bool vacio_encontrado = false;
	
	while(!vacio_encontrado){
		vacio_encontrado = true;
		//Generar nueva coordenada
		coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);

		//Buscar si alguna pared ocupa ese espacio
		int i = 0;
		while(vacio_encontrado && i < nivel.tope_paredes){
			if(coordenadas_iguales(nivel.paredes[i], coordenada)){
				vacio_encontrado = false;
			}
			i++;
		}

		//Buscar si algún obstáculo ocupa ese espacio
		if(buscar_elemento(nivel.obstaculos, nivel.tope_obstaculos, coordenada) != NADA_ENCONTRADO){
			vacio_encontrado = false;
		}

		//Buscar si alguna herramienta ocupa ese espacio
		if(buscar_elemento(nivel.herramientas, nivel.tope_herramientas, coordenada) != NADA_ENCONTRADO){
			vacio_encontrado = false;
		}


	}

	return coordenada;
}

/*
 * Pre: Recibe la referencia a un personaje_t y la coordenada en la que quiere iniciar, siendo ésta una válida
 * Post: Inicializa al personaje_t con sus respectivos valores
 */
void inicializar_personaje(personaje_t* ref_personaje, coordenada_t arranque_personaje){
	ref_personaje->posicion = arranque_personaje;
	ref_personaje->vida = VIDA_INICIAL_PERSONAJE;
	ref_personaje->puntos = PUNTOS_INICIALES_PERSONAJE;
	ref_personaje->tiene_llave = false;
	ref_personaje->interruptor_apretado = false;
}

/*
 * Pre: Recibe la referencia a un sombra_t y la coordenada en la que quiere iniciar
 * Post: Inicializa al sombra_t con sus respectivos valores
 */
void inicializar_sombra(sombra_t* ref_sombra, coordenada_t arranque_sombra){
	ref_sombra->posicion = arranque_sombra;
	ref_sombra->esta_viva = true;
}

/*
 * Pre: Ninguna
 * Post: Devuelve true si el movimiento concuerda con la convención propuesta, sino false
 */
bool es_movimiento_valido(char movimiento){
	if(movimiento == ARRIBA || movimiento == IZQUIERDA || movimiento == ABAJO || movimiento == DERECHA || movimiento == VIDA){
		return true;
	}else{
		return false;
	}
}

/*
 * Pre: Ninguna
 * Post: Pide un movimiento al usuario hasta que éste sea válido
 */
void pedir_movimiento(char* ref_movimiento){
	do{
		scanf("%c", ref_movimiento);
	}while(!es_movimiento_valido(*ref_movimiento));
}

/*
 * Pre: Recibe una coordenada válida y sus limites máximos
 * Post: Devuelve true si la coordenada está dentro de las dimensiones dadas, sino false
 */
bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho){
	if(posicion.fila >= max_alto || posicion.fila < 0 || posicion.col >= max_ancho || posicion.col < 0){
		return false;
	}else{
		return true;
	}
}

/*
 * Pre: Recibe un vector de coordenada_t con su tope y el elemento a encontrar
 * Post: Devuelve true si el elemento se encuentra en el vector o false si no se encuentre en él
 */
int buscar_coordenada(coordenada_t coordenadas[MAX_ELEMENTOS], int tope, coordenada_t coordenada){
	int indice_elemento = NADA_ENCONTRADO;

	int i = 0;
	while(i < tope && indice_elemento == NADA_ENCONTRADO){
		if(coordenadas_iguales(coordenadas[i], coordenada)){
			indice_elemento = i;
		}
		i++;
	}

	return indice_elemento;
}

/*
 * Pre: Recibe la referencia de un juego y un booleano que determina la forma en que se van a inicializar los valores
 * Post: Se inicializan los valores del personaje y su sombra dependiendo del booleano recibido
 */
void inicializar_personaje_y_sombra(juego_t* juego, bool de_cero){
	bool espacio_encontrado = false;
	nivel_t info_nivel_actual = juego->niveles[juego->nivel_actual-1];
 
	if(de_cero){
		while(!espacio_encontrado){
			coordenada_t pos_inicial_per = buscar_vacio_nivel(info_nivel_actual);
			inicializar_personaje(&(juego->personaje), pos_inicial_per);

			coordenada_t pos_inicial_som;
			pos_inicial_som.fila = pos_inicial_per.fila;
			pos_inicial_som.col = MAX_COLUMNAS - pos_inicial_per.col - 1;
			inicializar_sombra(&(juego->sombra), pos_inicial_som);
			
			int herramienta_encontrada = buscar_elemento(info_nivel_actual.herramientas, info_nivel_actual.tope_herramientas, juego->sombra.posicion);
			int obstaculo_encontrado = buscar_elemento(info_nivel_actual.herramientas, info_nivel_actual.tope_herramientas, juego->sombra.posicion);
			int pared_encontrada = buscar_coordenada(info_nivel_actual.paredes, info_nivel_actual.tope_paredes, juego->sombra.posicion);

			if(herramienta_encontrada == NADA_ENCONTRADO && obstaculo_encontrado == NADA_ENCONTRADO && pared_encontrada == NADA_ENCONTRADO){
				espacio_encontrado = true;
			}
		}
	}else{
		while(!espacio_encontrado){
			coordenada_t pos_inicial_per = buscar_vacio_nivel(info_nivel_actual);
			juego->personaje.posicion = pos_inicial_per;
			juego->personaje.interruptor_apretado = false;

			juego->sombra.posicion.fila = pos_inicial_per.fila;
			juego->sombra.posicion.col = MAX_COLUMNAS - pos_inicial_per.col - 1;
			
			int herramienta_encontrada = buscar_elemento(info_nivel_actual.herramientas, info_nivel_actual.tope_herramientas, juego->sombra.posicion);
			int obstaculo_encontrado = buscar_elemento(info_nivel_actual.herramientas, info_nivel_actual.tope_herramientas, juego->sombra.posicion);
			int pared_encontrada = buscar_coordenada(info_nivel_actual.paredes, info_nivel_actual.tope_paredes, juego->sombra.posicion);

			if(herramienta_encontrada == NADA_ENCONTRADO && obstaculo_encontrado == NADA_ENCONTRADO && pared_encontrada == NADA_ENCONTRADO){
				espacio_encontrado = true;
			}
		}
	}
}

/*
 * Pre: Recibe un movimiento que cumple con la convención establecida
 * Post: Devuelve el movimiento opuesto al recibido
 */
char movimiento_opuesto(char movimiento){
	if(movimiento == ARRIBA){
		return ABAJO;
	}else if(movimiento == IZQUIERDA){
		return DERECHA;
	}else if(movimiento == ABAJO){
		return ARRIBA;
	}else{
		return IZQUIERDA;
	}
}

/*
 * Pre: Recibe un movimiento ya validadoy una sombra por referencia
 * Post: Mueve la sombra dependiendo de las condiciones dadas
 */
void mover_sombra(sombra_t* ref_sombra, char movimiento, bool interruptor_apretado, bool esta_viva){
	if(!esta_viva){
		return;
	}
	if(movimiento == ARRIBA){
		ref_sombra->posicion.fila--;
	}else if(movimiento == ABAJO){
		ref_sombra->posicion.fila++;
	}else if(movimiento == IZQUIERDA){
		if(interruptor_apretado){
			ref_sombra->posicion.col--;
		}else{
			ref_sombra->posicion.col++;
		}
	}else if(movimiento == DERECHA){
		if(interruptor_apretado){
			ref_sombra->posicion.col++;
		}else{
			ref_sombra->posicion.col--;
		}
	}
}

/*
 * Pre: Recibe un movimiento válido y un personaje por referencia
 * Post: Realiza el movimiento dado
 */
void mover_personaje(personaje_t* ref_personaje, char movimiento){
	if(movimiento == ARRIBA){
		ref_personaje->posicion.fila--;
	}else if(movimiento == ABAJO){
		ref_personaje->posicion.fila++;
	}else if(movimiento == IZQUIERDA){
		ref_personaje->posicion.col--;
	}else if(movimiento == DERECHA){
		ref_personaje->posicion.col++;
	}
}

/*
 * Pre: Recibe un vector de elemento_t y la posición del elemento a eliminar
 * Post: Elimina el elemento de la posición dada
 */
void eliminar_elemento(elemento_t elementos[MAX_ELEMENTOS], int* tope, int posicion_elemento){
	elementos[posicion_elemento] = elementos[*tope-2];
	elementos[*tope-2] = elementos[*tope-1];
	(*tope)--;
}

/*
 * Pre: Recibe un vector de integers con su tope y el elemento a encontrar
 * Post: Devuelve true si el elemento se encuentra en el vector o false si no se encuentre en él
 */
bool int_esta_en_vector(int vector[MAX_ELEMENTOS], int tope, int elemento){
	bool elemento_encontrado = false;

	int i = 0;
	while(i < tope && !elemento_encontrado){
		if(vector[i] == elemento){
			elemento_encontrado = true;
		}
		i++;
	}

	return elemento_encontrado;
}

/*
 * Pre: Recibe dos coordenada válidas
 * Post: Devuelve la distancia manhattan entre estas 2 coordenadas
 */
int distancia_manhattan(coordenada_t coor1, coordenada_t coor2){
	int diferencia_filas = coor1.fila - coor2.fila;
	if(diferencia_filas < 0){
		diferencia_filas *= -1;
	}
	int diferencia_columnas = coor1.col - coor2.col;
	if(diferencia_columnas < 0){
		diferencia_columnas *= -1;
	}

	return diferencia_filas + diferencia_columnas;
}

/*
 * Pre: Recibe el numero de obstaculos de cada tipo, estos siendo mayor o igual a cero y la suma de ellos menor al maximo
 * Post: Llena el vector de obstaculos del nivel recibido
 */
void generar_obstaculos(nivel_t* nivel, int n_de_velas, int n_de_pozos, int n_de_interruptores, int n_de_portales){
	elemento_t elemento;

	int tope_con_velas = (nivel->tope_obstaculos)+n_de_velas;
	for(int i = nivel->tope_obstaculos; i < tope_con_velas; i++){
		elemento.tipo = VELA;
		coordenada_t coor_libre = buscar_vacio_nivel(*nivel);
		elemento.coordenada.fila = coor_libre.fila;
		elemento.coordenada.col = coor_libre.col;

		(*nivel).obstaculos[i] = elemento;
		nivel->tope_obstaculos++;
	}

	int tope_con_pozos = (nivel->tope_obstaculos)+n_de_pozos;
	for(int i = (nivel->tope_obstaculos); i < tope_con_pozos; i++){
		elemento.tipo = POZO;
		coordenada_t coor_libre = buscar_vacio_nivel(*nivel);
		elemento.coordenada.fila = coor_libre.fila;
		elemento.coordenada.col = coor_libre.col;
		
		(*nivel).obstaculos[i] = elemento;
		(nivel->tope_obstaculos)++;
	}

	int tope_con_interruptores = (nivel->tope_obstaculos)+n_de_interruptores;
	for(int i = (nivel->tope_obstaculos); i < tope_con_interruptores; i++){
		elemento.tipo = INTERRUPTOR;
		coordenada_t coor_libre = buscar_vacio_nivel(*nivel);
		elemento.coordenada.fila = coor_libre.fila;
		elemento.coordenada.col = coor_libre.col;
		
		(*nivel).obstaculos[i] = elemento;
		(nivel->tope_obstaculos)++;
	}

	int tope_con_portales = (nivel->tope_obstaculos)+n_de_portales;
	for(int i = (nivel->tope_obstaculos); i < tope_con_portales; i++){
		elemento.tipo = PORTAL;
		coordenada_t coor_libre = buscar_vacio_nivel(*nivel);
		elemento.coordenada.fila = coor_libre.fila;
		elemento.coordenada.col = coor_libre.col;
		
		(*nivel).obstaculos[i] = elemento;
		(nivel->tope_obstaculos)++;
	}
}

/*
 * Pre: Recibe el numero de herramientas de cada tipo, estos siendo mayor o igual a cero y la suma de ellos menor al maximo
 * Post: Llena el vector de herramientas del nivel recibido
 */
void generar_herramientas(nivel_t* nivel, int n_de_monedas, int n_de_llaves){
	int tope_con_monedas = (nivel->tope_herramientas)+n_de_monedas;
	elemento_t elemento;
	for(int i = (nivel->tope_herramientas); i < tope_con_monedas; i++){
		elemento.tipo = MONEDA;
		coordenada_t coor_libre = buscar_vacio_nivel(*nivel);
		elemento.coordenada.fila = coor_libre.fila;
		elemento.coordenada.col = coor_libre.col;
		
		(*nivel).herramientas[i] = elemento;
		(nivel->tope_herramientas)++;
	}

	int tope_con_llaves = (nivel->tope_herramientas)+n_de_llaves;
	for(int i = (nivel->tope_herramientas); i < tope_con_llaves; i++){
		elemento.tipo = LLAVE;
		coordenada_t coor_libre = buscar_vacio_nivel(*nivel);
		elemento.coordenada.fila = coor_libre.fila;
		elemento.coordenada.col = coor_libre.col;
		
		(*nivel).herramientas[i] = elemento;
		(nivel->tope_herramientas)++;
	}

	elemento.tipo = PUERTA;
	coordenada_t coor_libre = buscar_vacio_nivel(*nivel);
	elemento.coordenada.fila = coor_libre.fila;
	elemento.coordenada.col = coor_libre.col;
	(*nivel).herramientas[nivel->tope_herramientas] = elemento;
	(nivel->tope_herramientas)++;

}

/*
 * Pre: Recibe el vector de herramientas con su tope, el vector de paredes con su tope y el numero de escaleras a generar, este siendo mayor o igual a cero y menor al maximo
 * Post: Llena el vector de herramientas del nivel recibido
 */
void generar_escaleras(elemento_t herramientas[MAX_ELEMENTOS], int* tope_herramientas, coordenada_t paredes[MAX_PAREDES], int tope_paredes, int n_de_escaleras){
	int tope_con_paredes = (*tope_herramientas)+n_de_escaleras;
	int paredes_usadas[MAX_ELEMENTOS];
	int contador_paredes_usadas = 0;
	for(int i = (*tope_herramientas); i < tope_con_paredes; i++){
		int pared_random;
		do{
			pared_random = rand() % tope_paredes;
		}while(int_esta_en_vector(paredes_usadas, contador_paredes_usadas, pared_random));

		paredes_usadas[contador_paredes_usadas] = pared_random;
		contador_paredes_usadas++;

		elemento_t escalera = {ESCALERA, paredes[pared_random]};
		herramientas[i] = escalera;
		(*tope_herramientas)++;
	}
}

/*
 * Pre: Recibe un struct nivel_t vacío y el numero de nivel
 * Post: Llena el struct con elementos acordes al numero de nivel recibido
 */
void generar_nivel(nivel_t* nivel, int n_de_nivel){

	//Cargar numero de nivel
	nivel->numero_nivel = n_de_nivel+1;

	//Cargar topes
	nivel->tope_herramientas = 0;
	nivel->tope_obstaculos = 0;
	nivel->tope_paredes = 0;

	//Cargar paredes
	obtener_mapa(nivel->paredes, &(nivel->tope_paredes));

	//Cargar obstaculos y herramientas
	if(n_de_nivel == 0){
		generar_obstaculos(nivel, MAX_VELAS_NVL_1, MAX_POZOS_NVL_1, MAX_INTERRUPTORES_NVL_1, MAX_PORTALES_NVL_1);
		generar_escaleras(nivel->herramientas, &(nivel->tope_herramientas), nivel->paredes, nivel->tope_paredes, MAX_ESCALERAS_NVL_1);
		generar_herramientas(nivel, MAX_MONEDAS_NVL_1, MAX_LLAVES_NVL_1);
	}else if(n_de_nivel == 1){
		generar_obstaculos(nivel, MAX_VELAS_NVL_2, MAX_POZOS_NVL_2, MAX_INTERRUPTORES_NVL_2, MAX_PORTALES_NVL_2);
		generar_escaleras(nivel->herramientas, &(nivel->tope_herramientas), nivel->paredes, nivel->tope_paredes, MAX_ESCALERAS_NVL_2);
		generar_herramientas(nivel, MAX_MONEDAS_NVL_2, MAX_LLAVES_NVL_2);
	}else{
		generar_obstaculos(nivel, MAX_VELAS_NVL_3, MAX_POZOS_NVL_3, MAX_INTERRUPTORES_NVL_3, MAX_PORTALES_NVL_3);
		generar_escaleras(nivel->herramientas, &(nivel->tope_herramientas), nivel->paredes, nivel->tope_paredes, MAX_ESCALERAS_NVL_3);
		generar_herramientas(nivel, MAX_MONEDAS_NVL_3, MAX_LLAVES_NVL_3);
	}
	
}

/*
 * Pre: Recibe un juego inicializado
 * Post: Si el personaje no tiene vidas, no hace nada, caso contrario devuelve los personajes a un estado inicial pero en el mismo nivel
 */
void reiniciar_jugadores(juego_t* juego){
	if(juego->personaje.vida == 0){
		return;
	}else{
		inicializar_personaje_y_sombra(juego, false);
	}
}

/*
 * Pre: Recibe un nivel inicializado y una posicion válida
 * Post: Devuelve true si la posicion esta en rango de una vela o false en el caso contrario
 */
bool esta_en_rango_vela(coordenada_t posicion, nivel_t nivel){
	bool vela_encontrada = false;

	int i = posicion.fila-1;
	while(i <= posicion.fila+1 && !vela_encontrada){
		int j = posicion.col-1;
		while(j <= posicion.col+1 && !vela_encontrada){
			coordenada_t coordenada = {i, j};
			int pos_elemento = buscar_elemento(nivel.obstaculos, nivel.tope_obstaculos, coordenada);

			if(pos_elemento != NADA_ENCONTRADO){
				if(nivel.obstaculos[pos_elemento].tipo == VELA){
					vela_encontrada = true;
				}
			}
			j++;
		}
		i++;
	}

	return vela_encontrada;
}

/*
 * Pre: Recibe la referencia a un personaje y un nivel ya inicializados y un movimiento ya validado
 * Post: Ejecuta la acción de una escalera siempre y cuando se pueda
 */
void accionar_escalera(personaje_t* personaje, nivel_t* nivel, char movimiento){
	coordenada_t pos_inicial = personaje->posicion;
	int pos_escaleras[MAX_ELEMENTOS];
	pos_escaleras[0] = buscar_elemento(nivel->herramientas, nivel->tope_herramientas, personaje->posicion);
	int tope = 1;
	int herramienta_encontrada;
	int pared_encontrada;
	bool escaleras_usadas = true;

	do{
		mover_personaje(personaje, movimiento);
		herramienta_encontrada = buscar_elemento(nivel->herramientas, nivel->tope_herramientas, personaje->posicion);
		pared_encontrada = buscar_coordenada(nivel->paredes, nivel->tope_paredes, personaje->posicion);

		//Si se encuentra con otra escalera
		if(nivel->herramientas[herramienta_encontrada].tipo == ESCALERA){
			pos_escaleras[tope] = herramienta_encontrada;
			tope++;
		//Si se encuentra con una pared o los limites del terreno
		}else if(pared_encontrada != NADA_ENCONTRADO || !esta_dentro_rango(personaje->posicion, MAX_FILAS, MAX_COLUMNAS)){
			personaje->posicion = pos_inicial;
			mover_personaje(personaje, movimiento_opuesto(movimiento));
			escaleras_usadas = false;
		}
	}
	while(herramienta_encontrada != NADA_ENCONTRADO && nivel->herramientas[herramienta_encontrada].tipo == ESCALERA);

	if(escaleras_usadas){
		for(int i = 0; i < tope; i++){
			eliminar_elemento(nivel->herramientas, &(nivel->tope_herramientas), pos_escaleras[i]);
		}
	}
}

/* -------------------------------------------------------------------------- */
/*                             Funciones SegundoTP                            */
/* -------------------------------------------------------------------------- */

void imprimir_terreno(juego_t juego){
	printf("Nivel: %i\n", juego.nivel_actual);
	for(int i = 0; i < MAX_FILAS; i++){
		for(int j = 0; j < MAX_COLUMNAS; j++){
			coordenada_t coordenada_a_imprimir = {i, j};
			int herramienta_encontrada = buscar_elemento(juego.niveles[juego.nivel_actual-1].herramientas, juego.niveles[juego.nivel_actual-1].tope_herramientas, coordenada_a_imprimir);
			int obstaculo_encontrado = buscar_elemento(juego.niveles[juego.nivel_actual-1].obstaculos, juego.niveles[juego.nivel_actual-1].tope_obstaculos, coordenada_a_imprimir);
			int pared_encontrada = buscar_coordenada(juego.niveles[juego.nivel_actual-1].paredes, juego.niveles[juego.nivel_actual-1].tope_paredes, coordenada_a_imprimir);
			if(coordenadas_iguales(coordenada_a_imprimir, juego.personaje.posicion)){
				printf(COLOR_PERSONAJE "%c ", MAC);
			}else if(coordenadas_iguales(coordenada_a_imprimir, juego.sombra.posicion)){
				printf(COLOR_SOMBRA "%c ", BLUE);
			}else if(herramienta_encontrada != -1){
				char herramienta = juego.niveles[juego.nivel_actual-1].herramientas[herramienta_encontrada].tipo;
				if(herramienta == MONEDA){
					printf(COLOR_MONEDAS "%c ", herramienta);
				}else if(herramienta == PUERTA){
					printf(COLOR_PUERTA "%c ", herramienta);
				}else if(herramienta == LLAVE){
					printf(COLOR_LLAVE "%c ", herramienta);
				}else{
					printf(COLOR_DEFAULT "%c ", herramienta);
				}
			}else if(obstaculo_encontrado != -1){
				printf(COLOR_DEFAULT "%c ", juego.niveles[juego.nivel_actual-1].obstaculos[obstaculo_encontrado].tipo);
			}else if(pared_encontrada != -1){
				printf(COLOR_DEFAULT "%c ", PARED);
			}else{
				printf(COLOR_DEFAULT "%c ", VACIO);
			}
		}
		printf("\n");
	}
	printf(COLOR_DEFAULT);
	printf("Puntos: %i\n", juego.personaje.puntos);
	printf("Vidas Restantes: %i\n", juego.personaje.vida);
}

void inicializar_juego(juego_t* juego){
	cargar_mapas();
	
	juego->nivel_actual = NIVEL_INICIAL;

	for(int i = 0; i < MAX_NIVELES; i++){
		generar_nivel(&(juego->niveles[i]), i);
	}
	
	inicializar_personaje_y_sombra(juego, true);

}

int estado_juego(juego_t juego){
	if(estado_nivel(juego) == 1 && juego.nivel_actual == MAX_NIVELES){
		return 1;
	}
	if(juego.personaje.vida == 0){
		return -1;
	}
	return 0;
}

int estado_nivel(juego_t juego){
	int estado;
	//La puerta siempre es el ultimo elemento del vector herramientas
	nivel_t info_nivel_actual = juego.niveles[juego.nivel_actual-1];
	elemento_t puerta = info_nivel_actual.herramientas[info_nivel_actual.tope_herramientas-1];
	if(juego.nivel_actual == 1){
		if(distancia_manhattan(juego.personaje.posicion, puerta.coordenada) <= 1 && distancia_manhattan(juego.sombra.posicion, puerta.coordenada) <= 1 && juego.sombra.esta_viva){
			estado = 1;
		}else{
			estado = 0;
		}
	}else{
		if(distancia_manhattan(juego.personaje.posicion, puerta.coordenada) <= 1 && distancia_manhattan(juego.sombra.posicion, puerta.coordenada) <= 1 && juego.personaje.tiene_llave && juego.sombra.esta_viva){
			estado = 1;
		}else{
			estado = 0;
		}
	}

	return estado;
}

void realizar_jugada(juego_t* juego){
	char movimiento;

	printf("Ingrese un movimiento(W, A, S, D):");
	pedir_movimiento(&movimiento);

	if(movimiento == VIDA){
		if(juego->personaje.puntos >= 200 && juego->personaje.vida < 3){
			juego->personaje.puntos -= 200;
			juego->personaje.vida++;
		}
	}else{
		mover_personaje(&(juego->personaje), movimiento);
		mover_sombra(&(juego->sombra), movimiento, juego->personaje.interruptor_apretado, juego->sombra.esta_viva);

		//Si el movimiento hace que el juego o el nivel terminen, salir
		if(estado_nivel(*juego) == 1){
			if(estado_juego(*juego) == 1){
				return;
			}else{
				juego->nivel_actual++;
				reiniciar_jugadores(juego);
				return;
			}
		}

		//Si personaje pisa una escalera
		if(buscar_elemento(juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->personaje.posicion) != -1){
			int pos_herramienta = buscar_elemento(juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->personaje.posicion);
			if(juego->niveles[juego->nivel_actual-1].herramientas[pos_herramienta].tipo == ESCALERA){
				accionar_escalera(&(juego->personaje), &(juego->niveles[juego->nivel_actual-1]), movimiento/*, juego->personaje.posicion*/);
			}
		}


		//Si alguno esta sobre una pared
		if(buscar_coordenada(juego->niveles[juego->nivel_actual-1].paredes, juego->niveles[juego->nivel_actual-1].tope_paredes, juego->personaje.posicion) != NADA_ENCONTRADO){
			mover_personaje(&(juego->personaje), movimiento_opuesto(movimiento));
		}
		if(buscar_coordenada(juego->niveles[juego->nivel_actual-1].paredes, juego->niveles[juego->nivel_actual-1].tope_paredes, juego->sombra.posicion) != NADA_ENCONTRADO){
			mover_sombra(&(juego->sombra), movimiento_opuesto(movimiento), juego->personaje.interruptor_apretado, juego->sombra.esta_viva);
		}

		//Si alguno se sale del terreno
		if(!esta_dentro_rango(juego->personaje.posicion, MAX_FILAS, MAX_COLUMNAS)){
			mover_personaje(&(juego->personaje), movimiento_opuesto(movimiento));
		}
		if(!esta_dentro_rango(juego->sombra.posicion, MAX_FILAS, MAX_COLUMNAS)){
			mover_sombra(&(juego->sombra), movimiento_opuesto(movimiento), juego->personaje.interruptor_apretado, juego->sombra.esta_viva);
		}

		//Si personaje está sobre la sombra
		if(coordenadas_iguales(juego->personaje.posicion, juego->sombra.posicion) && !juego->sombra.esta_viva){
			juego->sombra.esta_viva = true;
			juego->personaje.puntos -= COSTO_REVIVIR_SOMBRA;
		}
		
		/* ----------------------- Chequear posicion personaje ---------------------- */
		// Si es una herramienta
		if(buscar_elemento(juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->personaje.posicion) != NADA_ENCONTRADO){
			int pos_elemento_encontrado = buscar_elemento(juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->personaje.posicion);
			//Caso MONEDA
			if(juego->niveles[juego->nivel_actual-1].herramientas[pos_elemento_encontrado].tipo == MONEDA){
				eliminar_elemento(juego->niveles[juego->nivel_actual-1].herramientas, &(juego->niveles[juego->nivel_actual-1].tope_herramientas), pos_elemento_encontrado);
				juego->personaje.puntos += rand() % RANGO_VALOR_MONEDA + MIN_VALOR_MONEDA;
			//Caso LLAVE
			}else if(juego->niveles[juego->nivel_actual-1].herramientas[pos_elemento_encontrado].tipo == LLAVE){
				juego->personaje.tiene_llave = true;
				eliminar_elemento(juego->niveles[juego->nivel_actual-1].herramientas, &(juego->niveles[juego->nivel_actual-1].tope_herramientas), pos_elemento_encontrado);
			}
		//Si es un obstáculo
		}else if(buscar_elemento(juego->niveles[juego->nivel_actual-1].obstaculos, juego->niveles[juego->nivel_actual-1].tope_obstaculos, juego->personaje.posicion) != NADA_ENCONTRADO){
			int pos_elemento_encontrado = buscar_elemento(juego->niveles[juego->nivel_actual-1].obstaculos, juego->niveles[juego->nivel_actual-1].tope_obstaculos, juego->personaje.posicion);
			//Caso POZO
			if(juego->niveles[juego->nivel_actual-1].obstaculos[pos_elemento_encontrado].tipo == POZO){
				juego->personaje.vida--;
				reiniciar_jugadores(juego);
			//Caso INTERRUPTOR
			}else if(juego->niveles[juego->nivel_actual-1].obstaculos[pos_elemento_encontrado].tipo == INTERRUPTOR){
				if(juego->personaje.interruptor_apretado){
					juego->personaje.interruptor_apretado = false;
				}else{
					juego->personaje.interruptor_apretado = true;
				}
			//Caso VELA
			}else if(juego->niveles[juego->nivel_actual-1].obstaculos[pos_elemento_encontrado].tipo == VELA){
				eliminar_elemento(juego->niveles[juego->nivel_actual-1].obstaculos, &(juego->niveles[juego->nivel_actual-1].tope_obstaculos), pos_elemento_encontrado);
			}
		}

		/* ------------------------ Chequear posicion sombra ------------------------ */
		// Si es una herramienta
		if(buscar_elemento(juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->sombra.posicion) != NADA_ENCONTRADO){
			int pos_elemento_encontrado = buscar_elemento(juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->sombra.posicion);
			//Caso MONEDA
			if(juego->niveles[juego->nivel_actual-1].herramientas[pos_elemento_encontrado].tipo == MONEDA){
				eliminar_elemento(juego->niveles[juego->nivel_actual-1].herramientas, &(juego->niveles[juego->nivel_actual-1].tope_herramientas), pos_elemento_encontrado);
				juego->personaje.puntos += rand() % RANGO_VALOR_MONEDA + MIN_VALOR_MONEDA;
			}
		//Si es un obstáculo
		}else if(buscar_elemento(juego->niveles[juego->nivel_actual-1].obstaculos, juego->niveles[juego->nivel_actual-1].tope_obstaculos, juego->sombra.posicion) != NADA_ENCONTRADO){
			int pos_elemento_encontrado = buscar_elemento(juego->niveles[juego->nivel_actual-1].obstaculos, juego->niveles[juego->nivel_actual-1].tope_obstaculos, juego->sombra.posicion);
			//Caso PORTAL
			if(juego->niveles[juego->nivel_actual-1].obstaculos[pos_elemento_encontrado].tipo == PORTAL){
				coordenada_t coor_aux;

				coor_aux = juego->personaje.posicion;
				juego->personaje.posicion = juego->sombra.posicion;
				juego->sombra.posicion = coor_aux;
			}
		//Caso VELA
		}else if(esta_en_rango_vela(juego->sombra.posicion, juego->niveles[juego->nivel_actual-1])){
			juego->sombra.esta_viva = false;
		}

	}
}