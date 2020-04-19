/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	int size = sizeof(int) * 2 + strlen((char *) paquete->buffer->stream) + 1;
	void *paquete_enviar = malloc(size);
	*bytes = size;
	int desplazamiento = 0;
	memcpy(paquete_enviar + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(paquete_enviar + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(paquete_enviar + desplazamiento, paquete->buffer->stream, strlen((char *) paquete->buffer->stream) + 1);
	return paquete_enviar;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente)
{
	printf("\nEnviando mensaje");
	t_paquete *paquete = malloc(sizeof(t_paquete));
	t_buffer *buffer = malloc(sizeof(t_buffer));
	paquete->codigo_operacion = MENSAJE;
	buffer->size = strlen(mensaje) + 1;
	void *stream = malloc(strlen(mensaje) + 1);
	stream = mensaje;
	buffer->stream = stream;
	paquete->buffer = buffer;
	int size;
	void *paquete_enviar = serializar_paquete(paquete, &size);
	send(socket_cliente, paquete_enviar, size, 0);
	printf("\nMensaje enviado");
	free(paquete->buffer);
	free(paquete);
	free(paquete_enviar);
}

//TODO
char* recibir_mensaje(int socket_cliente)
{
	printf("\nRecibiendo mensaje");
	op_code cod_op;
	recv(socket_cliente, &cod_op, sizeof(int), 0);
	int size;
	recv(socket_cliente, &size, sizeof(int), 0);
	char *mensaje = malloc(size);
	recv(socket_cliente, mensaje, size, 0);
	printf("\nMensaje recibido");
	return mensaje;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
