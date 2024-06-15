#include <stdio.h>
#include <stdlib.h>
#include <utils/configs.h>
#include <utils/conexion.h>
#include "opMemoria.h"

void handle_client(void* server_fd_ptr);
void handleConnection(void* client_socket_ptr);

t_log* logger;
config_memoria config;
t_list memoria_procesos;

int main(int argc, char* argv[]) {
	logger = iniciar_logger("memoria.log","memoria");

	config = memoria_cargar_config("memoria.config");
	
	log_info(logger,"### LOG MEMORIA ### \n");

	int server_fd = crear_socket(logger,SERVER,IP_GENERICA,config.puerto_escucha);
	log_info(logger, "Servidor %s listo para recibir al cliente\n","MEMORIA");
	//int server_fd = iniciar_servidor(config_valores.ip_propia,config_valores.puerto_propio);
	//server_detach(logger,"MEMORIA",server_fd);

	int* server_fd_ptr = malloc(sizeof(int));
	*server_fd_ptr = server_fd;

	pthread_t manejo_recepcion;
	pthread_create(&manejo_recepcion, NULL, (void*) handle_client, server_fd_ptr);
	pthread_detach(manejo_recepcion); 

	while(1)
	{
		
	}
	
	log_info(logger,"Modulo memoria finalizado.\n");

	close(server_fd); 

	log_destroy(logger);
	
	return 0;
}

void handle_client(void* server_fd_ptr) //Thread para esperar clientes
{
	int server_fd = *(int*)server_fd_ptr;
	free(server_fd_ptr);

	while(1)
	{
		t_conexiones conexiones;
		conexiones.socket = esperar_cliente(logger,"",server_fd);
		//Threads para recepcion / envio de info a clientes
		pthread_t t;
		int* client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = conexiones.socket;
		pthread_create(&t, NULL, (void*) handleConnection, client_socket_ptr);
		pthread_detach(t);
	}
}

void handleConnection(void* client_socket_ptr)
{
	int client_socket = *(int*)client_socket_ptr;
    free(client_socket_ptr);

	T_PACKET cod_op = recibir_operacion(client_socket);

	switch(cod_op)
	{
		case MSG:
			//recibir_mensaje(socket);
			break;
		case PCKT_START_PROCESS:
			iniciar_proceso(client_socket);
			break;
		case PCKT_FINISH_PROCESS:
			finalizar_proceso(client_socket);
		case PCKT_INSTRUCTION:
			// RETARDO ANTES DE MANDAR LA INSTRUCCION
			sleep(config.retardo_respuesta / 1000);
			enviar_instruccion_cpu(client_socket);
		case -1:
			printf("se cerro la conexion / error\n");
			//disconnected = 1;
			break;
		default:
			printf("\nDESCONOCIDO OP_CODE: %d\n",cod_op);
			//log_warning(logger, "Operacion desconocida.");
			//exit(EXIT_FAILURE);
			break;
	}
}