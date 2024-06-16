#ifndef _kernel_scheduler_h_
#define _kernel_scheduler_h_

#include <utils/shared.h>
#include <semaphore.h>
#include <pthread.h>
#include "proceso.h"

/*
    * @NAME: startScheduling
    * @DESC: Inicia la planificacion de procesos en el sistema
    * @PARAMS: void
    * @RETURN: void
*/
void startScheduling();

/*
    * @NAME: stopScheduling
    * @DESC: Pausa la planificacion de procesos en el sistema
    * @PARAMS: void
    * @RETURN: void
*/
void stopScheduling();

void scheduling();

void interrupcion(int skt_cpu_int,t_process* proceso,T_PACKET packet );
void esperar_proceso_cpu(int skt_client, t_process* proceso);

void loggerCambioDeEstado(uint32_t pid, const char* estadoAnterior,const char* estadoActual);
#endif