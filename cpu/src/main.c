#include <stdlib.h>
#include <stdio.h>
#include <utils/configs.h>
#include <utils/conexion.h>
#include "opCPU.h"

void handle_client_dispatch(void* server_fd_ptr);
void handle_client_interrupt(void* server_int);
void handleConnection(void* client_socket_ptr);
void handleConnectionInt(void* client_socket_ptr);

config_cpu config;
t_log* logger;
sem_t sem_instruccion;
int interrupt_pid = -1;
pthread_mutex_t interrupt_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[]) {
	logger = iniciar_logger("cpu.log","CPU");

	config = cpu_cargar_config("cpu.config");
	
	log_info(logger,"CPU Iniciado! \n");

	// Inicializar el semáforos
    sem_init(&sem_instruccion, 0, 1); // sincro fetch y execute

	// Escucho solicitudes de Memoria y Kernel	
	int server_fd = crear_socket(logger,SERVER,IP_GENERICA,config.puerto_dispatch);
	log_info(logger, "SERVIDOR CPU DISPATCH\n");

	int* server_fd_ptr = malloc(sizeof(int));
	*server_fd_ptr = server_fd;

	pthread_t manejo_recepcion;
	pthread_create(&manejo_recepcion, NULL, (void*) handle_client_dispatch, server_fd_ptr);
	pthread_detach(manejo_recepcion); 
	
	// Escucho interrupciones
	int server_int = crear_socket(logger,SERVER,IP_GENERICA,config.puerto_interrupt);
	log_info(logger, "SERVIDOR CPU INTERRUPT\n");

	int* server_int_ptr = malloc(sizeof(int));
	*server_int_ptr = server_int;

	pthread_t manejo_recepcion_int;
	pthread_create(&manejo_recepcion_int, NULL, (void*) handle_client_interrupt, server_int_ptr);
	pthread_detach(manejo_recepcion_int); 
	
	while(1)
	{
		
	}
	
	log_info(logger,"El modulo CPU Finalizo.");

	close(server_fd); 
	log_destroy(logger);

    return 0;
}

void handle_client_dispatch(void* server_fd_ptr) //Thread para esperar clientes
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

void handle_client_interrupt(void* server_int)
{
	int server_fd = *(int*)server_int;
	free(server_int);

	while(1)
	{
		t_conexiones conexiones;
		conexiones.socket = esperar_cliente(logger,"",server_fd);
		//Threads para recepcion / envio de info a clientes
		pthread_t t;
		int* client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = conexiones.socket;
		pthread_create(&t, NULL, (void*) handleConnectionInt, client_socket_ptr);
		pthread_detach(t);
	}
}

void handleConnection(void* client_socket_ptr)
{
	int client_socket = *(int*)client_socket_ptr;
    free(client_socket_ptr);

	T_PACKET cod_op = recibir_operacion(client_socket);
	t_process* proceso = malloc(sizeof(t_process));

	switch(cod_op)
	{
		case MSG:
			//recibir_mensaje(socket);
			break;
		case PCKT_PROCESO:
			
			recibir_proceso(client_socket,proceso);
			
			log_info(logger,"\n PID:%d - Estado: EXECUTE\n",proceso->PID);
			
			ciclo_instruccion_cpu(client_socket,proceso);
			
		case PCKT_FIN_QUANTUM:
			enviar_proceso(client_socket,proceso);
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

void handleConnectionInt(void* client_socket_ptr)
{
	int client_socket = *(int*)client_socket_ptr;
    free(client_socket_ptr);

	T_PACKET cod_op = recibir_operacion(client_socket);

	switch(cod_op)
	{
		case MSG:
			//recibir_mensaje(socket);
			break;
		case INTERRUPT:
			pthread_mutex_lock(&interrupt_mutex);
			recibir_interrupcion(client_socket,&interrupt_pid);
			pthread_mutex_unlock(&interrupt_mutex);
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