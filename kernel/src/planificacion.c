#include "planificacion.h"
#include <utils/shared.h>
#include <utils/configs.h>


extern pthread_t scheduling_thread = -1;
sem_t scheduling_pause;

extern config_kernel config;
extern t_listsProcesses listProcesses;

void startScheduling()
{ 
    // if (firstTime == 0)
    // {
    //     firstTime = 1;
    //     pthread_t scheduling_short;
    //     if (strcmp(/*config.ALGORITHM*/ "FIFO", "FIFO") == 0)
    //     {
    //         pthread_create(&scheduling_short, NULL, (void *)scheduling_FIFO, NULL);
    //     }
    //     else if (strcmp(/*config.ALGORITHM*/ "RR", "RR") == 0)
    //     {
    //         pthread_create(&scheduling_short, NULL, (void *)scheduling_RR, NULL);
    //     }
    //     else if (strcmp(/*config.ALGORITHM*/ "VRR", "VRR") == 0)
    //     {
    //         pthread_create(&scheduling_short, NULL, (void *)scheduling_VRR, NULL);
    //     }
    //     pthread_detach(scheduling_short);
    // }
    if (scheduling_thread == -1)
        //pthread_create(&pscheduling, NULL, (void *)scheduling, NULL);
    sem_post(&scheduling_pause);
}

void stopScheduling()
{
    sem_wait(&scheduling_pause);
}

void scheduling()
{
    int size_new        = list_size(get_listOfProcesses("NEW"));
    int size_ready      = list_size(get_listOfProcesses("READY")); 
    int size_exec       = list_size(get_listOfProcesses("EXECUTE"));
    int size_block_io   = list_size(get_listOfProcesses("BLOCK_IO"));
    int size_block      = list_size(get_listOfProcesses("BLOCK")); 
    
    
    /*
        crear la lista de procesos que esten ready, si hay que cargar new's a ready hacerlo
        si no hay procesos ready, esperar a que haya
        si hay procesos ready, ordenarlos segun el algoritmo de scheduling en una lista para mandar al cpu
        ejecutar(1) el proceso que este en la cabeza de la lista
        
        *(1) antes de ejecutar, revisar sem para ver si se pauso el scheduling
        *(2) pensar en como hacer I/O executions
    */ 
   t_process* process;
    while (true){
        sem_wait(&scheduling_pause); // ? -> 1
        while (list_size(get_listOfProcesses("NEW")) > 0 && config.grado_multiprogramacion >= (size_new + size_ready + size_exec) )
        {
//            process = list_remove(get_listOfProcesses("NEW")->Processes,0);
//            list_add(get_listOfProcesses("READY")->Processes, process);
            free(process);process = NULL;
        }
        // planificar procesos en la lista ready
        // fifo does nothing
        // rr to ready list

        while(list_size(get_listOfProcesses("READY")) > 0) 
        {
            // -> 1
            wait(&scheduling_pause);
            // -> 0
            process = list_remove(get_listOfProcesses("READY")->Processes,0);
            list_add(get_listOfProcesses("EXECUTE")->Processes, process);
            // mandar a cpu, mientras que el proceso no termine o se bloquee por I/O, o quantum
                // si es necesario, mandar a block_io
                // si es necesario, mandar a block, si lo bloquea por interrupcion, una vez el cpu este listo, para seguir el proceso en block, se debe pasar a execute nuevamente
                // si finaliso el proceso, pasar a finish
            list_remove(get_listOfProcesses("EXECUTE")->Processes,0);
            free(process);process = NULL;
            sem_post(&scheduling_pause);
            // -> 1
        } // termina ejecutar todos los procesos en ready

        // pasar block a ready
        while(list_size(get_listOfProcesses("BLOCK")) > 0) 
        {
            // -> 1
            wait(&scheduling_pause);
            // -> 0
            process = list_remove(get_listOfProcesses("BLOCK")->Processes,0);
            list_add(get_listOfProcesses("READY")->Processes, process);
            free(process);process = NULL;
            sem_post(&scheduling_pause);
            // -> 1
        } 

        sem_post(&scheduling_pause);

    }
    
    


}

/*
    ready -> execute -> mandar al cpu -> block_io -> finish
                                         finish 
*/

//void swap(t_process* process,char* list_origen)
//{
    /*if( process->END == TRUE )
    {
        list_add(get_listOfProcesses("FINISH")->Processes, process);
        // conect socket MEMORY - ELIMINAR ESTRUCTURA {PID,PATH}
        // send(FINISH_PROCESS,process->PID)
    } 
    else
    {
        list_add(get_listOfProcesses("READY")->Processes, process);
    }

    list_remove_element(get_listOfProcesses(list_origen)->Processes, process);*/
//}