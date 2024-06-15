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

#endif