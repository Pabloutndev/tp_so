#ifndef _kernel_scheduler_h_
#define _kernel_scheduler_h_

#include <utils/shared.h>
#include <semaphore.h>
#include <pthread.h>
#include "proceso.h"

extern pthread_t pscheduling;

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

void swap(t_process* process);

#endif