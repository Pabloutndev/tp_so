#ifndef UTILS_HELLO_H_
#define UTILS_HELLO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/log.h>
#include <pthread.h>
//#include <inttypes.h>

#define PUERTO_KERNEL "8003"
#define PUERTO_MEMORIA "8002"
#define PUERTO_CPU "8006"
#define PUERTO_IO "8009"

#define IP_GENERICA "127.0.0.1"

typedef enum {
    MENSAJE1,
    MENSAJE2,
    ERROR=-1
} op_code;

enum T_SOCKET{
  SERVER = 0,
  CLIENTE = 1
};

//  ---------  HANDSHAKE ---------

int handshake_serv(t_log *logger,int fd_conexion);
int handshake_client(t_log *logger,int fd_conexion);

//  ---------  SOCKETS ---------

int crear_socket(t_log *logger, enum T_SOCKET tipo, char* ip, char* puerto);
int esperar_cliente(t_log* logger, int socket_servidor);

//  ---------  LOGERS ---------

/**
* @fn    decir_hola
* @brief Imprime un saludo al nombre que se pase por parámetro por consola.
*/
void decir_hola(char* quien);
t_log* iniciar_logger(char* path, char* nombre);

#endif
