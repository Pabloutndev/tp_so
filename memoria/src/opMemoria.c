#include <unistd.h>
#include <limits.h>
#include "opMemoria.h"

extern t_log* logger;
extern t_list memoria_procesos;
extern config_memoria config;

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
	
	char current_path[PATH_MAX];
	getcwd(current_path, sizeof(current_path));

	char* string_leido = leer_string(buffer,&desp);
	size_t total_length = strlen(current_path) + strlen(string_leido) + 1;
	char* path_completo = malloc(total_length);

	strcpy(path_completo, current_path);
	strcat(path_completo, string_leido);
	
	proceso->path = strdup(path_completo);

	printf("\nPATH COMPLETO DEL PROCESO: %s\n",proceso->path);
	
	free(string_leido);
	free(path_completo);

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


void enviar_instruccion_cpu(int client_socket)
{
	int size, desp = 0;
	char * buffer;
	int pid;
	int32_t pc;

	buffer = recibir_buffer(&size, client_socket);
	pid = leer_entero(buffer, &desp);
	pc = leer_entero(buffer, &desp);
	
   	bool find_Process_PID(void* data) {
		proceso_mem *p = (proceso_mem*) data;
		return (p->pid == pid);
	};

	proceso_mem *proceso = list_find(&memoria_procesos,find_Process_PID);
	
	if(proceso!=NULL)
	{    
		FILE *file = fopen(proceso->path, "r");
		if( file == NULL ) {
			perror("Error al abrir archivo de instrucciones");
			return;
		}
		
		char* line = NULL;
		size_t len = 0;
		ssize_t read;
		int current_line = 0;

		while((read = getline(&line, &len, file)) != -1) {
			if(current_line == pc) {
				break;
			}
			current_line++;
		}

		if(current_line == pc && read != -1)
		{
			t_paquete* paquete = crear_paquete_con_codigo_op(PCKT_INSTRUCTION_MEM);
			printf("\nmandamos: %s\n", line);
			agregar_a_paquete(paquete,line,(strlen(line)+1));
			enviar_paquete(paquete,client_socket);
		}
		else 
		{
			perror("Error al buscar instruccion en archivo de instrucciones");
		}

        fclose(file);
        free(line);
	} else {
		printf("PRoceso no encontrado para PID: %d\n",pid);
	}

	free(buffer);
	liberar_conexion(client_socket);
}