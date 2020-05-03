/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

int main(int argc, char **argv) {
	if (argc < 3) {
		puts("Ups...faltan parametros");
		puts("Uso: ./tp0 {OPERACION} {POKEMON} {CANTIDAD}");
		exit(1);
	}

	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	log_info(logger, "Soy un log!");

	config = leer_config();

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO: %s", puerto);

	//crear conexion
	conexion = crear_conexion(ip, puerto);

	char* operation = argv[1];
	char* pokemon_name_value = argv[2];
	log_info(logger, "Operation is: %s", operation);
	log_info(logger, "Pokemon is: %s", pokemon_name_value);
	if (argc == 4) {
		log_info(logger, "Cantidad: %d", atoi(argv[3]));
	}

	Pokemon* pokemon = malloc(sizeof(Pokemon));

	Name* name = malloc(sizeof(Name));
	char* pika = malloc(strlen(pokemon_name_value) + 1);
	pika = strcpy(pika, pokemon_name_value);
	name->value = pika;
	name->size = strlen(pokemon_name_value) + 1;

	pokemon->name = name;
	t_list* coordinates = list_create();
	Coordinate* coor = malloc(sizeof(Coordinate));
	coor->pos_x = 1;
	coor->pos_y = 3;
	list_add(coordinates, coor);
	pokemon->coordinates = coordinates;

	if (strcasecmp(operation, "NEW") == 0) {
		New* new_pokemon = malloc(sizeof(New));
		new_pokemon->pokemon = pokemon;
		new_pokemon->quantity = atoi(argv[3]);
		send_new(new_pokemon, conexion);
	} else if (strcasecmp(operation, "GET") == 0) {
		Get* get_pokemon = malloc(sizeof(Get));
		get_pokemon->name = name;
		send_get(get_pokemon, conexion);
	} else if (strcasecmp(operation, "APPEARED") == 0) {
		send_pokemon(pokemon, APPEARED, conexion);
	} else if (strcasecmp(operation, "CATCH") == 0) {
		send_pokemon(pokemon, CATCH, conexion);
	} else if (strcasecmp(operation, "LOCALIZED") == 0) {
		Localized* localized_pokemon = malloc(sizeof(Localized));

		/*
		Coordinate* coor2 = malloc(sizeof(Coordinate));
		coor2->pos_x = 1;
		coor2->pos_y = 3;

		list_add(coordinates, coor2);

		pokemon->coordinates = coordinates;
		*/
		localized_pokemon->pokemon = pokemon;
		localized_pokemon->coordinates_quantity = coordinates->elements_count;

		send_localized(localized_pokemon, conexion);
	} else {
		log_info(logger, "Operacion invalida: %s", operation);
		exit(1);
	}

	//recibir mensaje
	int id;
	recv(conexion, &id, sizeof(int), MSG_WAITALL);

	log_info(logger, "Recibi: %d\n", id);
	//loguear mensaje recibido
	//log_info(logger,mensaje);

	//libero mensaje
	//free(mensaje);

	terminar_programa(conexion, logger, config);
}

//TODO
t_log* iniciar_logger(void) {
	printf("\nCreando archivo de log");
	return log_create("game-watch.log", "game-watch", true, LOG_LEVEL_INFO);
}

//TODO
t_config* leer_config(void) {
	printf("\nLeyendo archivo de configuración");
	return config_create("tp0.config");
}

//TODO
void terminar_programa(int conexion, t_log* logger, t_config* config) {
//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
