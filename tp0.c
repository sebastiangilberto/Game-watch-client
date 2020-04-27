/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

int main(void)
{
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

	//enviar mensaje
	New* new_pokemon = malloc(sizeof(New));
	Pokemon* pokemon = malloc(sizeof(Pokemon));
	Name* name = malloc(sizeof(Name));
	char* pika = malloc(8);
	pika = "pikachu";
	name->value = pika;
	name->size = 8;
	pokemon->name = name;
	t_list* coordinates = list_create();
	Coordinate* coor = malloc(sizeof(Coordinate));
	coor->pos_x = 1;
	coor->pos_y = 3;
	list_add(coordinates, coor);

	pokemon->coordinates = coordinates;
	new_pokemon->pokemon = pokemon;
	new_pokemon->quantity = 2;
	send_new(new_pokemon, conexion);

	//recibir mensaje
	int id;
	recv(conexion, &id, sizeof(int), MSG_WAITALL);

	printf("%d", id);
	//loguear mensaje recibido
	//log_info(logger,mensaje);

	//libero mensaje
	//free(mensaje);


	terminar_programa(conexion, logger, config);
}

//TODO
t_log* iniciar_logger(void)
{
	printf("\nCreando archivo de log");
	return log_create("game-watch.log", "game-watch", true, LOG_LEVEL_INFO);
}

//TODO
t_config* leer_config(void) {
	printf("\nLeyendo archivo de configuración");
	return config_create("tp0.config");
}

//TODO
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
