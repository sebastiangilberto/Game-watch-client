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
	enviar_mensaje("how you doing?", conexion);

	//recibir mensaje
	char *mensaje = recibir_mensaje(conexion);

	//loguear mensaje recibido
	log_info(logger, mensaje);

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
