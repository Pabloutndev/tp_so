#include <utils/shared.h>
#include <utils/configs.h>
#include "planificacion.h"


pthread_t scheduling_thread = -1;
sem_t scheduling_pause;
extern t_log* logger;
extern config_kernel config;
extern t_listsProcesses listProcesses;

void startScheduling()
{ 
    // if (firstTime == 0)
    // {
    //     firstTime = 1;
    pthread_t pscheduling;
    sem_init(&scheduling_pause,0,1);

    if (scheduling_thread == -1)
    {
        pthread_create(&pscheduling, NULL, (void *)scheduling, NULL);
    }
    sem_post(&scheduling_pause);
}

void stopScheduling()
{
    sem_wait(&scheduling_pause);
}

void scheduling()
{
    //int size_new        = list_size(get_listOfProcesses("NEW")->Processes);
    //int size_ready      = list_size(get_listOfProcesses("READY")->Processes); 
    //int size_exec       = list_size(get_listOfProcesses("EXECUTE")->Processes);
    //int size_block_io   = list_size(get_listOfProcesses("BLOCK_IO")->Processes);
    //int size_block      = list_size(get_listOfProcesses("BLOCK")->Processes); 
    
    t_process* process;

    while(true)
    {
        sem_wait(&scheduling_pause); // ? -> 1
        while ( list_size(get_listOfProcesses("NEW")->Processes) > 0 && 
                config.grado_multiprogramacion >= ( list_size(get_listOfProcesses("BLOCK")->Processes) + 
                                                    list_size(get_listOfProcesses("READY")->Processes) + 
                                                    list_size(get_listOfProcesses("EXECUTE")->Processes)))
        {
            process = list_remove(get_listOfProcesses("NEW")->Processes,0);
            list_add(get_listOfProcesses("READY")->Processes, process);
            loggerCambioDeEstado(process->PID,"NEW","READY");
            log_info(logger,"PID: %d - Estado anterior: NEW - Estado Actual: READY",process->PID);
            free(process);process = NULL;
        }
        
        // planificar procesos en la lista ready
        // fifo does nothing
        // rr to ready list

        while(list_size(get_listOfProcesses("READY")->Processes) > 0) 
        {
            // -> 1
            sem_wait(&scheduling_pause);
            // -> 0
            process = list_remove(get_listOfProcesses("READY")->Processes,0);
            list_add(get_listOfProcesses("EXECUTE")->Processes, process);
            
            loggerCambioDeEstado(process->PID,"READY","EXECUTE");

            // mandar a cpu, mientras que el proceso no termine o se bloquee por I/O, o quantum
            // si es necesario, mandar a block_io
            // si es necesario, mandar a block, si lo bloquea por interrupcion, una vez el cpu este listo, para seguir el proceso en block, se debe pasar a execute nuevamente
            // si finalizo el proceso, pasar a finish
            int con_CPU_DIS = crear_socket(logger,CLIENTE,config.ip_cpu,config.puerto_cpu_dispatch);
            printf("PROCESO MANDAR A CPU PID:%d\n",process->PID);
            
            enviar_proceso(con_CPU_DIS,process);
            
            if(strcmp(config.algoritmo_planificacion, "FIFO")==0)
            {
                int codop = recibir_operacion(con_CPU_DIS);
                while(1)
                {
                    if(codop == OK)
                    {
                        printf("PROCESO RECIBIDO OK POR CPU");
                        break;
                    } else if(codop==FAIL) {
                        printf("Error al mandar proceso\n");
                        break;
                    } else {
                        printf("Error en send process \n");
                        break;
                    }
                }   
            } 
            else if(strcmp(config.algoritmo_planificacion,"RR")==0)
            {
                // esperar quantum
                sleep(config.quantum / 1000);
            
                // avisar a cpu que termine el proceso
                int con_CPU_INT = crear_socket(logger,CLIENTE,config.ip_cpu,config.puerto_cpu_interrupt);
            
                interrupcion(con_CPU_INT,process,INTERRUPT); //INTERRUPT_QUANTUM

                // recibir proceso de cpu
                esperar_proceso_cpu(con_CPU_DIS,process);
                
                printf("CONTEXTO ACTUALIZADO DEL PROCESO:PID:%d\n",process->PID);
            }
            // mandar proceso a ready
            process = list_remove(get_listOfProcesses("EXECUTE")->Processes,0);
            list_add(get_listOfProcesses("READY")->Processes, process);
            
            loggerCambioDeEstado(process->PID,"EXECUTE","READY");

            liberar_conexion(con_CPU_DIS);
            //free(process);process = NULL;
            sem_post(&scheduling_pause);
            // -> 1
        }

        // pasar block a ready
        while(list_size(get_listOfProcesses("BLOCK")->Processes) > 0) 
        {
            // -> 1
            sem_wait(&scheduling_pause);
            // -> 0
            process = list_remove(get_listOfProcesses("BLOCK")->Processes,0);
            list_add(get_listOfProcesses("READY")->Processes, process);
            free(process);process = NULL;
            sem_post(&scheduling_pause);
            // -> 1
        }
        
        while(list_size(get_listOfProcesses("FINISH")->Processes) > 0)
        {
            process = list_get(get_listOfProcesses("FINISH")->Processes,0);
            log_info(logger,"Finaliza el Proceso PID:%d - Motivo: SUCCESS",process->PID);
            char buffer[10];
            sprintf(buffer, "%d", process->PID);
            finishProcess(buffer);
            free(process);process=NULL;
            //free(buffer);
        }

        sem_post(&scheduling_pause);
    }
    
}

void loggerCambioDeEstado(uint32_t pid, const char* estadoAnterior,const char* estadoActual)
{
    log_info(logger, "PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s>", pid, estadoAnterior, estadoActual);
}

void interrupcion(int skt_cpu_int,t_process* proceso,T_PACKET packet )
{
    t_paquete* paquete = crear_paquete_con_codigo_op(packet);
    agregar_entero_a_paquete(paquete,proceso->PID);
    enviar_paquete(paquete,skt_cpu_int);
    eliminar_paquete(paquete);
}

void esperar_proceso_cpu(int skt_client, t_process* proceso)
{
    T_PACKET cod_op = recibir_operacion(skt_client);
    while(cod_op!=PCKT_PROCESO)
    {
        cod_op = recibir_operacion(skt_client);
    }
    recibir_proceso(skt_client,proceso);
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