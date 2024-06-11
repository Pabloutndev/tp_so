#include <commons/collections/list.h>
#include <string.h>
#include "opMemoria.h"

extern t_log* logger;
extern t_list memoria_procesos;

int pid = 0;

void iniciar_proceso(int socket_cliente)
{
    int size, desp = 0;
	char * buffer;
	//int pid = list_size(&memoria_procesos) + 1;
	pid++;

	log_info(logger,"Iniciando proceso %d",pid);
	
	proceso_mem *proceso = malloc(sizeof(proceso_mem));
	
	buffer = recibir_buffer(&size, socket_cliente);
    
	proceso->pid = pid;
	proceso->path = strdup(leer_string(buffer, &desp));

	list_add(&memoria_procesos,proceso);

	log_info(logger,"\nProceso PID: %d, Path: %s\n\n", proceso->pid, proceso->path);
  	
	int guardar = 1;
	
	//if(elegirEsquema() == SEGMENTACION){            
        //guardar = s_iniciarProceso(pid);
    //}else if(elegirEsquema() == PAGINACION){
        //guardar = p_iniciarProceso(pid);
    //}
	
	if(guardar == 1){
		log_info(logger,"Proceso guardado OK");
		enviarOK_PID(socket_cliente,pid);
	}
	else
	{	
		log_error(logger, "No hay lugar para alojar la patota en memoria, lo sentimos");
		enviarFail(socket_cliente);
	}

	free(buffer);
}

void finalizar_proceso(int socket_cliente)
{
    int size, desp = 0;
	char * buffer;
	int pid;
	
	buffer = recibir_buffer(&size, socket_cliente);
    
	pid = leer_entero(buffer, &desp);

   	bool find_Process_PID(void* data) {
		proceso_mem *p = (proceso_mem*) data;
		return (p->pid == pid);
	};

	proceso_mem *proceso = list_find(&memoria_procesos,find_Process_PID);
	
	if(proceso!=NULL)
	{
		proceso = list_remove_by_condition(&memoria_procesos,find_Process_PID);
	}

	log_info(logger,"\nPROCESO BORRADO DE MEMORIA\n");
  	
	free(proceso->path);
	free(proceso);
	
	//int guardar = 1;
	
}

