#ifndef UTILS_PCK_H_
#define UTILS_PCK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/log.h>
#include <pthread.h>
#include <semaphore.h>
#include "cpu.h"

extern sem_t mutexOk;

typedef enum
{ // packet type
    PCKT_NONE = -1,
    MSG=0,
    OK,
    FAIL,
    PCKT_HANDSHAKE = 100,
    PCKT_MESSAGE = 101,
    PCKT_ACK = 102,
    PCKT_EJECUTAR_SCRIPT=103,
    PCKT_START_PROCESS = 104,
    PCKT_FINISH_PROCESS = 105,
    PCKT_START_PLANIFICACION=106,
    PCKT_STOP_PLANIFICACION=107,
    PCKT_PROCESO,
    PCKT_PROCESO_KERNEL,
    PCKT_INSTRUCTION,
    PCKT_INSTRUCTION_MEM,
    INTERRUPT,
    PCKT_FIN_QUANTUM,
    PCKT_ERROR = 400,
} T_PACKET;
/*
typedef struct {
    int64_t op;
    int64_t size;
    void* buffer;
    int offset;
} T_PCKT;
*/

typedef enum{
	PAGINACION,
	SEGMENTACION
}esquema;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	T_PACKET codigo_operacion;
	t_buffer* buffer;
} t_paquete;

t_paquete* crear_paquete_con_codigo_op(T_PACKET codigo_op);
void agregar_entero_a_paquete(t_paquete* paquete, int x);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void crear_buffer(t_paquete* paquete);

T_PACKET recibir_operacion(int socket_cliente);
//int recibir_operacion(int socket_cliente);
void* recibir_buffer(int* size, int socket_cliente);
void recibir_mensaje(int socket_cliente);

void* serializar_paquete(t_paquete* paquete, int bytes);

void eliminar_paquete(t_paquete* paquete);

int leer_entero(char*buffer, int* desplazamiento);
char* leer_string(char* buffer, int* desplazamiento);

/*
    * @NAME: enviar_proceso
    * @DESC: Envia proceso serializado a un socket cliente
    * @PARAMS:
    *  - socket_cliente: int
    *  - proceso: t_process*
*/
void enviar_proceso(int socket_client, t_process* proceso);

/*
    * @NAME: enviar_proceso
    * @DESC: Recibe proceso de un socket y lo deserializa
    * @PARAMS:
    *  - socket_cliente: int
    *  - proceso: t_process*
    * @RETURN: 
*/
void recibir_proceso(int socket_client, t_process* proceso);

void recibir_interrupcion(int skt_client, int* interrupt_pid);

void enviarOK(int socket);
void enviarOK_PID(int socket, int pid);
int get_pid(int socket);
void enviarFail(int socket);




#endif /*UTILS_PCK_H_*/