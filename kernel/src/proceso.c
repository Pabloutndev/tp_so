#include "proceso.h"
#include <utils/conexion.h>
#include <utils/configs.h>

extern t_listsProcesses listsProcesses;
//extern t_conexiones con_Memory;
//extern T_CONNECTION con_CPU_clientDisp;
extern config_kernel config;
extern t_log *logger;

void startProcess(char* path)
{
    t_process* process = malloc(sizeof(t_process));
    
    int con_MEM = crear_socket(logger,CLIENTE,config.ip_memoria,config.puerto_memoria);

    t_paquete* package = crear_paquete_con_codigo_op(PCKT_START_PROCESS);
    path = "/instrucciones/process1.txt";
    agregar_a_paquete(package,path,(strlen(path) + 1));

    enviar_paquete(package, con_MEM);
    log_info(logger,"Se envio el paquete a MEMORIA");   
    
    int codop = recibir_operacion(con_MEM);
    if(codop == OK)
    {
        int pid = get_pid(con_MEM);
        printf("\npid: %d\n",pid);
        initialize_process(process,pid,config.quantum);
    }
    else if(codop==FAIL)
    {
        printf("no habia suficiente espacio en memoria \n");
        log_info(logger,"no habia suficiente espacio en memoria \n");
    }
    else
    {
        printf("Error en startProcess \n");
    }
    eliminar_paquete(package);
    liberar_conexion(con_MEM);
    
    int tam_multprog =  list_size( get_listOfProcesses("NEW")->Processes)   + 
                        list_size( get_listOfProcesses("READY")->Processes) + 
                        list_size( get_listOfProcesses("EXECUTE")->Processes);

    if(config.grado_multiprogramacion > tam_multprog)
    {
        list_add((get_listOfProcesses("READY")->Processes),process);
        log_info(logger,"\nProceso %d: se creo y envio a READY\n",process->PID);
    } 
    else 
    {
        list_add((get_listOfProcesses("NEW")->Processes),process);
        log_info(logger,"LIMITE DEL GRADO DE MULTIPROGRAMACION - VALOR:%d\n",config.grado_multiprogramacion);
        log_info(logger,"\nProceso %d: SE CREO EN NEW\n",process->PID);
    }

}

void finishProcess(char* pid_str)
{
    int pid = atoi(pid_str);

	int remove = remove_process_by_pid(pid);

    if (remove == 1)
    {
        int con_MEM = crear_socket(logger,CLIENTE,config.ip_memoria,config.puerto_memoria);

        t_paquete* paquete = crear_paquete_con_codigo_op(PCKT_FINISH_PROCESS);

        agregar_entero_a_paquete(paquete,pid);

        enviar_paquete(paquete, con_MEM);

        eliminar_paquete(paquete);
        
        //log_info(logger,"Proceso PID:%d finished\n",pid);
        
        liberar_conexion(con_MEM);
    } 
    else 
    {
        log_info(logger,"Proceso PID:%d No se encontro.\n",pid);
    }
}

void initialize_process(t_process* process, int pid, int quantum) 
{
    process->PID = pid;
    process->QUANTUM = quantum;
    process->cpu.AX = 0;
    process->cpu.BX = 0;
    process->cpu.CX = 0;
    process->cpu.DX = 0;
    process->cpu.EAX = 0;
    process->cpu.EBX = 0;
    process->cpu.ECX = 0;
    process->cpu.EDX = 0;
    process->cpu.SI = 0;
    process->cpu.DI = 0;
    process->cpu.PC = 0;
}

t_listProcess* get_listOfProcesses(char* name)
{
    int i = 0;
    // Modificar 6 por un valor calculado
    for (i=0; i < 6; i++)
    {
        if (strcmp(listsProcesses.listProcess[i].name, name) == 0)
        {
            return &(listsProcesses.listProcess[i]);
        }
    }
    return NULL;
}

int remove_process_by_pid(int pid)
{
    int i=0;
    for(i=0;i<6;i++)
    {
        t_list* processList = listsProcesses.listProcess[i].Processes;
        
        bool _findProcessPID(void* data) {
            t_process* proceso = (t_process*) data;
            return proceso->PID == pid;
        };
        
        t_process* foundProcess = (t_process*) list_find(processList, _findProcessPID);
        
        if (foundProcess != NULL) 
        {
            t_process* removedProcess = (t_process*) list_remove_by_condition(processList, _findProcessPID);
            free(removedProcess);
            return 1; // Process was found and removed
        }
    }

    return 0; // Process not found
}
