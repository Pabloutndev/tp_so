#include "paquete.h"

sem_t mutexOk;

T_PACKET recibir_operacion(int socket_cliente)
{
	T_PACKET cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(T_PACKET), MSG_WAITALL) != 0)
	{
		return cod_op;
	}
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;
	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	
	if(buffer != NULL){
		recv(socket_cliente, buffer, *size, MSG_WAITALL);
	}else {
		printf("Error de size en malloc size:%d. recibir_buffer\n\n", *size);
	}

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	//log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_paquete* crear_paquete_con_codigo_op(T_PACKET codigo_op)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo_op;
	crear_buffer(paquete);
	return paquete;
}

void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

void agregar_entero_a_paquete(t_paquete* paquete, int x) // Agrega un entero a un paquete (ya creado)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + sizeof(int));
	
	memcpy(paquete->buffer->stream + paquete->buffer->size, &x, sizeof(int));

	paquete->buffer->size += sizeof(int);
	
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + sizeof(int) + sizeof(T_PACKET);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(T_PACKET);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

int leer_entero(char*buffer, int* desplazamiento)
{
	int ret;
	memcpy(&ret, buffer + (*desplazamiento), sizeof(int));
	(*desplazamiento)+=sizeof(int);
	return ret;
}

char* leer_string(char* buffer, int* desplazamiento) 
{
    int tamanio = leer_entero(buffer, desplazamiento);

    char* string = malloc(tamanio);
    
    memcpy(string, buffer + (*desplazamiento), tamanio);
    (*desplazamiento) += tamanio;

    return string;
}

void enviarOK(int socket)
{
	pthread_mutex_lock(&mutexOk);
	
	t_paquete* paquete;
						
	paquete = crear_paquete_con_codigo_op(OK);
	
	enviar_paquete(paquete,socket);

	eliminar_paquete(paquete);
	
	pthread_mutex_unlock(&mutexOk);
}

void enviarOK_PID(int socket, int pid)
{
	pthread_mutex_lock(&mutexOk);
	
	t_paquete* paquete;
						
	paquete = crear_paquete_con_codigo_op(OK);

	agregar_entero_a_paquete(paquete,pid);
	
	enviar_paquete(paquete,socket);
	
	eliminar_paquete(paquete);
	
	pthread_mutex_unlock(&mutexOk);
}


void enviarFail(int socket)
{
	t_paquete* paquete;
		
	paquete = crear_paquete_con_codigo_op(FAIL);
	
	enviar_paquete(paquete,socket);
	
	eliminar_paquete(paquete);
	
	liberar_conexion(socket);
}


int get_pid(int socket)
{
	int size, desp = 0, pid;
	char * buffer;
	buffer = recibir_buffer(&size, socket);// get PID from memory module
	pid = leer_entero(buffer,&desp);
	return pid;
}

void enviar_proceso(int socket_client, t_process* proceso)
{
    t_paquete* paquete = crear_paquete_con_codigo_op(PCKT_PROCESO);

    agregar_entero_a_paquete(paquete,proceso->PID);
    agregar_entero_a_paquete(paquete,proceso->QUANTUM);
    agregar_entero_a_paquete(paquete,proceso->cpu.AX);
    agregar_entero_a_paquete(paquete,proceso->cpu.BX);
    agregar_entero_a_paquete(paquete,proceso->cpu.CX);
    agregar_entero_a_paquete(paquete,proceso->cpu.DI);
    agregar_entero_a_paquete(paquete,proceso->cpu.DX);
    agregar_entero_a_paquete(paquete,proceso->cpu.EAX);
    agregar_entero_a_paquete(paquete,proceso->cpu.EBX);
    agregar_entero_a_paquete(paquete,proceso->cpu.ECX);
    agregar_entero_a_paquete(paquete,proceso->cpu.EDX);
    agregar_entero_a_paquete(paquete,proceso->cpu.PC);
    agregar_entero_a_paquete(paquete,proceso->cpu.SI);

    enviar_paquete(paquete,socket_client);
    eliminar_paquete(paquete);
}

void recibir_proceso(int socket_client, t_process* proceso)
{
    int size;
    int desplazamiento = 0;
    char* buffer = recibir_buffer(&size, socket_client);
    
    proceso->PID        = leer_entero(buffer,&desplazamiento);
    proceso->QUANTUM    = leer_entero(buffer,&desplazamiento);
    proceso->cpu.AX     = leer_entero(buffer,&desplazamiento);
    proceso->cpu.BX     = leer_entero(buffer,&desplazamiento);
    proceso->cpu.CX     = leer_entero(buffer,&desplazamiento);
    proceso->cpu.DI     = leer_entero(buffer,&desplazamiento);
    proceso->cpu.DX     = leer_entero(buffer,&desplazamiento);
    proceso->cpu.EAX    = leer_entero(buffer,&desplazamiento);
    proceso->cpu.EBX    = leer_entero(buffer,&desplazamiento);
    proceso->cpu.ECX    = leer_entero(buffer,&desplazamiento);
    proceso->cpu.EDX    = leer_entero(buffer,&desplazamiento);
    proceso->cpu.PC     = leer_entero(buffer,&desplazamiento);
    proceso->cpu.SI     = leer_entero(buffer,&desplazamiento);

    free(buffer);
}
