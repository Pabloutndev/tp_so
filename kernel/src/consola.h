#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <readline/readline.h>
#include <pthread.h>
#include <semaphore.h>
#include "planificacion.h"
#include <utils/configs.h>

/*
    * @NAME: handleConsole
    * @DESC: Consola de kernel
    * @PARAMS: void
*/
void handleConsole(void);

/*
    * @NAME: executeScriptOP
    * @DESC: Funcion de para ejecutar comandos de consola
    * @PARAMS:
    *  - path: char*
*/
void executeScriptOP(char* path);

/*
    * @NAME: listStateProcess
    * @DESC: lista procesos con sus estados
    * @PARAMS: void
*/
void listStateProcess();

/*
    * @NAME: string_array_length
    * @DESC: Devuelve el largo de un vector de char*
    * @PARAMS:
    * - arr: char**
    * @RETURN: int
*/
int string_array_length(char **arr);

/*
    * @NAME: getProcessState
    * @DESC: Devuelve el estado del proceso en formato string
    * @PARAMS:
    *  - state: char
    * @RETURN: char*
*/
char* getProcessState(char state);

void cambiar_multiprogramacion(char* valor);

#endif /*_CONSOLE_H_*/