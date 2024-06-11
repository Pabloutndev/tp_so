#include <stdlib.h>
#include <stdio.h>
#include <utils/configs.h>
#include <utils/conexion.h>
#include "consola.h"

void init_listsProcesses(t_listsProcesses *listsProcesses);

t_log *logger;
config_kernel config;
t_listsProcesses listsProcesses;

int main(int argc, char* argv[]) 
{
	logger = iniciar_logger("kernel.log","KERNEL");
	
	config = kernel_cargar_config("kernel.config");

	log_info(logger,"LOG KERNEL \n");

	//int con_CPU_DIS = crear_socket(logger,CLIENTE,config.ip_cpu,config.puerto_cpu_dispatch);
	//int con_CPU_INT = crear_socket(logger,CLIENTE,config.ip_cpu,config.puerto_cpu_dispatch);
	//int con_MEM = crear_socket(logger,CLIENTE,config.ip_memoria,config.puerto_memoria);
	
	//send_string(con_MEM, "HOLA CPU!");
    
    init_listsProcesses(&listsProcesses);

	int server_fd = crear_socket(logger,SERVER,IP_GENERICA,config.puerto_escucha);
	log_info(logger, "Servidor KERNEL listo para recibir clientes");

	pthread_t manejo_consola;
	pthread_create(&manejo_consola, NULL, (void*) handleConsole, NULL);
	pthread_detach(manejo_consola);	

	while(1)
	{

	}

	log_info(logger,"Modulo Kernel Finalizado.");

	close(server_fd);
	log_destroy(logger);
	
	return 0;
}

void init_listsProcesses(t_listsProcesses *listsProcesses) 
{
    // Asigna memoria para 6 elementos de t_listProcess
    listsProcesses->listProcess = malloc(6 * sizeof(t_listProcess));
    if (listsProcesses->listProcess == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Inicializa cada elemento de listProcess
    listsProcesses->listProcess[0] = (t_listProcess) {
        .name = strdup("NEW"),  // strdup para copiar la cadena estática
        .Processes = list_create()
    };
    listsProcesses->listProcess[1] = (t_listProcess) {
        .name = strdup("READY"),
        .Processes = list_create()
    };
    listsProcesses->listProcess[2] = (t_listProcess) {
        .name = strdup("EXECUTE"),
        .Processes = list_create()
    };
    listsProcesses->listProcess[3] = (t_listProcess) {
        .name = strdup("BLOCK_IO"),
        .Processes = list_create()
    };
    listsProcesses->listProcess[4] = (t_listProcess) {
        .name = strdup("BLOCK"),
        .Processes = list_create()
    };
    listsProcesses->listProcess[5] = (t_listProcess) {
        .name = strdup("FINISH"),
        .Processes = list_create()
    };
}