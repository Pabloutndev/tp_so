#ifndef _OP_MEMORIA_H_
#define _OP_MEMORIA_H_

#include <stdlib.h>
#include <commons/log.h>
#include <utils/paquete.h>

typedef struct {
    int pid;
    char* path;
} proceso_mem;

void iniciar_proceso(int skt_client);


#endif /*#define _OP_MEMORIA_H_*/