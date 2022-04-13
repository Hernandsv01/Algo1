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

#define FUN_AGREGAR "agregar_partida"
#define FUN_ELIMINAR "eliminar_partida"
#define FUN_ORDENAR "ordenar_partidas"
#define FUN_JUGAR "config_juego"

#define ARGS_AGREGAR 3
#define ARGS_ELIMINAR 4
#define ARGS_ORDENAR 3
#define ARGS_JUGAR 2

#define SI_GANO "Si"
#define NO_GANO "No"

#define DIVISOR_DATOS_CSV ";"
#define FORMATO_LECTOR_LINEA_COMPLETA "%[^\n]\n"
#define FORMATO_LECTOR_PARTIDA "%[^;];%[^;];%[^;];%[^;];%s\n"
#define FORMATO_ESCRITOR_LINEA_COMPLETA "%s\n"
#define MENSAJE_ERROR_ARGUMENTOS "Numero incorrecto de argumentos, se esperaban %i y se recibieron %i\n"
#define MENSAJE_ERROR_ARCHIVO "No se pudo abrir el archivo\n"
#define MENSAJE_ERROR_ARCHIVO_AUX "No se pudo abrir el archivo auxiliar\n"
#define MENSAJE_ERROR_ARGUMENTOS_AUSENTES "Faltan argumentos, este archivo se ejecuta de la forma: ./mansion_foster <<funcionalidad>> <<argumentos de funcionalidad>>\n"
#define MENSAJE_ERROR_FUNCIONALIDAD_INEXISTENTE "La funcionalidad ingresada no existe\n"

#define MAX_NOMBRE 100
#define MAX_LINEA 100
#define MAX_ELEMENTOS 500

#define MODO_WRITE "w"
#define MODO_READ "r"

#define NOMBRE_ARCHIVO_AUX "archivo_aux.txt"

typedef struct partida {
    char jugador[MAX_NOMBRE];
    int nivel_llegado;
    int puntos;
    int vidas_restantes;
    bool gano;
} partida_t;

/*
 * Pre: Cada elemento del vector tiene valores válidos y el nombre del archivo debe tener la extensión
 * Post: Si el archivo no existe, se crea, y si no se puede abrir no se hace nada
 */
void vector_a_archivo(partida_t vector[MAX_ELEMENTOS], int tope, char nombre_archivo[MAX_NOMBRE]){
    FILE* archivo = fopen(nombre_archivo, MODO_WRITE);
    if(!archivo){
        printf("No se pudo escribir sobre el archivo: %s", nombre_archivo);
        return;
    }

    for(int i = 0; i < tope; i++){
        fprintf(archivo, "%s;%i;%i;%i;", vector[i].jugador, vector[i].nivel_llegado, vector[i].puntos, vector[i].vidas_restantes);
        if(vector[i].gano){
            fprintf(archivo, "%s", SI_GANO);
        }else{
            fprintf(archivo, "%s", NO_GANO);
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);

}

/*
 * Pre: El archivo deber ser un CSV que siga la convención establecida de partidas
 * Post: Se guarda en el vector cada uno de los elementos en el archivo, si es que hay
 */
void archivo_a_vector(partida_t vector[MAX_ELEMENTOS], int* tope, char nombre_archivo[MAX_NOMBRE]){
    FILE* archivo = fopen(nombre_archivo, MODO_READ);
    if(!archivo){
        printf(MENSAJE_ERROR_ARCHIVO);
        return;
    }
    char nivel_llegado[MAX_NOMBRE];
    char puntos[MAX_NOMBRE];
    char vidas_restantes[MAX_NOMBRE];
    char gano_str[MAX_NOMBRE];

    while(!feof(archivo) && (*tope) != MAX_ELEMENTOS){
        fscanf(archivo, FORMATO_LECTOR_PARTIDA, vector[(*tope)].jugador, nivel_llegado, puntos, vidas_restantes, gano_str);

        vector[(*tope)].nivel_llegado = atoi(nivel_llegado);
        vector[(*tope)].puntos = atoi(puntos);
        vector[(*tope)].vidas_restantes = atoi(vidas_restantes);
        vector[(*tope)].gano = strcmp(gano_str, NO_GANO);

        (*tope)++;
    }
    fclose(archivo);
}

/*
 * Pre: La partida debe tener todos sus valores inicializados
 * Post: Crea un string con los valores de la partida separados por el elemento divisor de CSV DIVISOR_DATOS_CSV
 */
void partida_a_string(char str_partida[MAX_LINEA], partida_t partida){
    char aux[MAX_LINEA];
    strcat(str_partida, partida.jugador);
    strcat(str_partida, DIVISOR_DATOS_CSV);

    sprintf(aux, "%i", partida.nivel_llegado);
    strcat(str_partida, aux);
    strcat(str_partida, DIVISOR_DATOS_CSV);

    sprintf(aux, "%i", partida.puntos);
    strcat(str_partida, aux);
    strcat(str_partida, DIVISOR_DATOS_CSV);

    sprintf(aux, "%i", partida.vidas_restantes);
    strcat(str_partida, aux);
    strcat(str_partida, DIVISOR_DATOS_CSV);

    if(partida.gano){
        strcat(str_partida, SI_GANO);
    }else{
        strcat(str_partida, NO_GANO);
    }
}

/*
 * Pre: Ninguna
 * Post: Le pide al usuario por consola los valores de una partida y los retorna en un elemento partida_t
 */
partida_t obtener_partida(){
    partida_t partida;

    printf("Nombre del jugador:");
    scanf("%[^\n]", partida.jugador);

    printf("Nivel llegado: ");
    scanf("%i", &partida.nivel_llegado);

    printf("Puntos conseguidos: ");
    scanf("%i", &partida.puntos);

    printf("Vidas restantes: ");
    scanf("%i", &partida.vidas_restantes);

    printf("Logró terminar el juego? (Si/No)");
    char gano[MAX_NOMBRE];
    scanf("%s", gano);
    partida.gano = strcmp(gano, NO_GANO);

    return partida;
}

/*
 * Pre: El archivo tiene que existir
 * Post: Le pide al usuario los datos de una partida y los guarda de forma ordenada en el archivo
 */
void agregar_partida(char nombre_archivo[MAX_NOMBRE]){
    
    FILE* archivo = fopen(nombre_archivo, MODO_READ);
    if(!archivo){
        printf(MENSAJE_ERROR_ARCHIVO);
        return;
    }
    FILE* archivo_aux = fopen(NOMBRE_ARCHIVO_AUX, MODO_WRITE);
    if(!archivo_aux){
        printf(MENSAJE_ERROR_ARCHIVO_AUX);
        fclose(archivo);
        return;
    }

    partida_t partida = obtener_partida();

    char str_partida[MAX_LINEA] = "";
    partida_a_string(str_partida, partida);

    char linea[MAX_LINEA];
    char nombre[MAX_NOMBRE];
    bool agregado = false;
    while(!feof(archivo)){
        fscanf(archivo, FORMATO_LECTOR_LINEA_COMPLETA, linea);
        strcpy(nombre, linea);
        strtok(nombre, DIVISOR_DATOS_CSV);

        if(strcmp(nombre, partida.jugador) > 0 && !agregado){
            fprintf(archivo_aux, FORMATO_ESCRITOR_LINEA_COMPLETA, str_partida);
            agregado = true;
        }
        fprintf(archivo_aux, FORMATO_ESCRITOR_LINEA_COMPLETA, linea);
    }
    if(!agregado){
        fprintf(archivo_aux, FORMATO_ESCRITOR_LINEA_COMPLETA, str_partida);
        agregado = true;
    }

    fclose(archivo);
    fclose(archivo_aux);

    rename(NOMBRE_ARCHIVO_AUX, nombre_archivo);
}

/*
 * Pre: El archivo y el jugador deben existir
 * Post: Se elimina la partida del jugador recibido y se mantiene el orden del resto en el archivo
 */
void eliminar_partida(char nombre_archivo[MAX_NOMBRE], char nombre_jugador[MAX_NOMBRE]){
    FILE* archivo = fopen(nombre_archivo, MODO_READ);
    if(!archivo){
        printf(MENSAJE_ERROR_ARCHIVO);
        return;
    }
    FILE* archivo_aux = fopen(NOMBRE_ARCHIVO_AUX, MODO_WRITE);
    if(!archivo_aux){
        printf(MENSAJE_ERROR_ARCHIVO_AUX);
        fclose(archivo);
        return;
    }

    char linea[MAX_LINEA];
    char nombre[MAX_NOMBRE];
    while(!feof(archivo)){
        fscanf(archivo, FORMATO_LECTOR_LINEA_COMPLETA, linea);
        strcpy(nombre, linea);
        strtok(nombre, DIVISOR_DATOS_CSV);
        if(strcmp(nombre, nombre_jugador) != 0){
            fprintf(archivo_aux, FORMATO_ESCRITOR_LINEA_COMPLETA, linea);
        }
    }

    fclose(archivo);
    fclose(archivo_aux);

    rename(NOMBRE_ARCHIVO_AUX, nombre_archivo);
}

/*
 * Pre: El archivo debe existir
 * Post: El ordena las partidas del archivo por nombre del jugador
 */
void ordenar_partidas(char nombre_archivo[MAX_NOMBRE]){
    if(!fopen(nombre_archivo, MODO_READ)){
        printf(MENSAJE_ERROR_ARCHIVO);
        return;
    }

    partida_t partidas[MAX_ELEMENTOS];
    int tope = 0;
    archivo_a_vector(partidas, &tope, nombre_archivo);

    partida_t aux;
    for(int i = 0; i < tope; i++){
        for(int j = 0; j < tope - i - 1; j++){
            if(strcmp(partidas[j].jugador, partidas[j+1].jugador) > 0){
                aux = partidas[j];
                partidas[j] = partidas[j+1];
                partidas[j+1] = aux;
            }
        }
    }

    vector_a_archivo(partidas, tope, nombre_archivo);
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


int main(int argc, char* argv[]){
    srand ((unsigned)time(NULL));
    
    if(argc == 1){
        printf(MENSAJE_ERROR_ARGUMENTOS_AUSENTES);
        return -1;
    }

    if(strcmp(argv[1], FUN_AGREGAR) == 0){
        if(argc != ARGS_AGREGAR){
            printf(MENSAJE_ERROR_ARGUMENTOS, ARGS_AGREGAR, argc);
            return -1;
        }
        agregar_partida(argv[2]);

    }else if(strcmp(argv[1], FUN_ELIMINAR) == 0){
        if(argc != ARGS_ELIMINAR){
            printf(MENSAJE_ERROR_ARGUMENTOS, ARGS_ELIMINAR, argc);
            return -1;
        }
        eliminar_partida(argv[2], argv[3]);

    }else if(strcmp(argv[1], FUN_ORDENAR) == 0){
        if(argc != ARGS_ORDENAR){
            printf(MENSAJE_ERROR_ARGUMENTOS, ARGS_ORDENAR, argc);
            return -1;
        }
        ordenar_partidas(argv[2]);
    
    }else if(strcmp(argv[1], FUN_JUGAR) == 0){
        if(argc != ARGS_JUGAR){
            printf(MENSAJE_ERROR_ARGUMENTOS, ARGS_JUGAR, argc);
        }
        juego_t juego;
        inicializar_juego(&juego);
        correr_juego(juego);
    }else{
        printf(MENSAJE_ERROR_FUNCIONALIDAD_INEXISTENTE);
    }


    return 0;
}
