#ifndef UTILS_CON_H_
#define UTILS_CON_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/log.h>
#include <pthread.h>
#include <utils/paquete.h>
//#include <inttypes.h>

#define PUERTO_KERNEL "8003"
#define PUERTO_MEMORIA "8002"
#define PUERTO_CPU "8006"
#define PUERTO_IO "8009"

#define IP_GENERICA "127.0.0.1"

typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;

typedef struct
{
    int socket;
} t_conexiones;

// Mensajes (codigo de operacion)
// Se requiere para saber que enviar/recibir
typedef enum {
    ERROR=-1,
    MENSAJE=0,
    HANDSHAKE,
    //DEBUG,
} op_code;

enum T_SOCKET{
  SERVER = 0,
  CLIENTE = 1
};

//  --------- THREADS - HILOS ---------

int server_detach(t_log* logger, char* server_name, int socket_servidor);
void procesar_conexion(void* void_args);
void manejar_clientes(int server_fd);
void liberar_conexion(int socket_cliente);

//  ---------  OPERACIONES ---------

int send_string(int fd_conexion, char* string);
int recv_string(int fd_conexion, char** string);

//  ---------  HANDSHAKE ---------

int handshake_serv(t_log *logger,int fd_conexion);
int handshake_client(t_log *logger,int fd_conexion);

//  ---------  SERIALIZACION ---------

void* serializar_string(size_t* size, char* string);
void deserializar_string(void* stream, char** string);

//  ---------  SOCKETS ---------

int crear_socket(t_log *logger, enum T_SOCKET tipo, char* ip, char* puerto);
int esperar_cliente(t_log* logger, char* nombre, int socket_servidor);

//  ---------  LOGERS ---------

/**
* @fn    decir_hola
* @brief Imprime un saludo al nombre que se pase por parámetro por consola.
*/
void decir_hola(char* quien);
t_log* iniciar_logger(char* path, char* nombre);

#endif /* UTILS_CON_H */
