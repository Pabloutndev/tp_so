#ifndef _kernel_Proceso_h_
#define _kernel_Proceso_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <utils/cpu.h>
#include <utils/conexion.h>

//#include <utils/shared.h> // for utils shared throughout the project
//#include "cpu.h"

/*typedef struct  { 
    int PID;
    int QUANTUM; // VRR
    t_cpu cpu;
} t_process;*/

typedef struct {
    char* name;
    t_list* Processes;
} t_listProcess;

typedef struct {// change to adapt to cpu module  
    t_listProcess* listProcess;
} t_listsProcesses;

extern t_listsProcesses listsProcesses;

t_listProcess* get_listOfProcesses(char*);

/*
    * @NAME: getProcessState
    * @DESC: Devuelve el estado del proceso en formato string
    * @PARAMS:
    *  - state: char
    * @RETURN: char*
*/
char* processStateToString(char);

/*
    * @NAME: startProcess
    * @DESC: Inicia un proceso
    * @PARAMS:
    *  - path: char*
*/
void startProcess(char*);

/*
    * @NAME: finishProcess
    * @DESC: Finaliza un proceso
    * @PARAMS:
    *  - pid_str: char*
*/
void finishProcess(char*);

/*
    * @NAME: finishProcess
    * @DESC: Inicializa un proceso con todos sus registros en 0
    * @PARAMS:
    *  - process: t_process*
    *  - pid: int
    *  - quantum: int
*/
void initialize_process(t_process* process, int pid, int quantum);


bool compare_pid(void* data, void* arg);
int remove_process_by_pid(int pid);

#endif
