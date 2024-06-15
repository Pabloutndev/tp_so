#ifndef _OP_MEMORIA_H_
#define _OP_MEMORIA_H_

#include <stdlib.h>
#include <stdbool.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <utils/configs.h>
#include <utils/conexion.h>

typedef struct {
    int pid;
    char* path;
} proceso_mem;

void iniciar_proceso(int skt_client);

void finalizar_proceso(int skt_client);

void enviar_instruccion_cpu(int client_socket);

#endif /*#define _OP_MEMORIA_H_*/