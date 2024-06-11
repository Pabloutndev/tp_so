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
    path = "/instructions/process1.txt";
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

    int tam_multprog =  list_size( get_listOfProcesses("NEW")->Processes)   + 
                        list_size( get_listOfProcesses("READY")->Processes) + 
                        list_size( get_listOfProcesses("EXECUTE")->Processes);

    if(config.grado_multiprogramacion >= tam_multprog)
    {
        list_add((get_listOfProcesses("NEW")->Processes),process);
        printf("Proceso %d: ENCOLADO EN NEW\n",process->PID);
    } 
    else 
    {
        printf("Proceso %d: NO PUEDE SER ENCOLADO EN NEW POR GRADO EXCESO DE MULTIPROGRAMACION\n",process->PID);
    }

    eliminar_paquete(package);
    liberar_conexion(con_MEM);
}

void finishProcess(char* pid_str)
{
    //int pid = atoi(pid_str);

    // kernel
    // Search for process in all lists of process and delete

    // Notify to Memory
    //T_PACKAGE* package = create_package(FINISH_PROCESS);
    //add_int_to_package(package,pid);

    //int resp_op = send_package(skt_client,package);
    
    printf("\nProcess finished\n");
    
    //destroy_package(package);
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