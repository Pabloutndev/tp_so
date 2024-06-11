#include <utils/conexion.h>



// ##################
//  THREADS - HILOS
// ##################

void procesar_conexion(void* void_args)
{
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

    op_code cop;
    while(cliente_socket != -1) {

        if(recv(cliente_socket, &cop, sizeof(op_code), 0) != sizeof(op_code)) {
            log_info(logger,"Desconectado!");
            return ;
        }

        switch(cop)
        {
            /*case DEBUG: 
                log_info(logger,"debug");
                break;
            case HANDSHAKE: 
                log_info(logger,"HANDSHAKE PERFECTO!");
                break;
                uint8_t dato;
                if(!recv_handshake(cliente_socket,&dato)) {
                    log_error(logger, "Fallo HANDSHAKE de %s",server_name);
                    break;
                }
                break;*/
            //case cop ...
            case MENSAJE: 
                char* msg;

                if( !recv_string(cliente_socket, &msg)){
                    log_error(logger,"Fallo recv de cop MENSAJE");
                    break;
                }

                log_info(logger, "Mensaje Recibido: %s", msg);
                free(msg);
                break;

            case -1: log_error(logger,"Cliente desconectado de %s",server_name);
            default: 
                log_error(logger, "algo anduvo mal en el server %s", server_name);
                log_info(logger, "Cop: %d",cop);
                return;
        }
    }
    
    log_warning(logger,"EL cliente se desconecto de %s server", server_name);
    return ;

}

int server_detach(t_log* logger, char* server_name, int socket_servidor)
{
    int cliente_socket = esperar_cliente(logger,server_name,socket_servidor);
    
    if(cliente_socket != -1)
    {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}


// ### OPERACIONES

int send_string(int fd_conexion, char* string)
{
    size_t size;
    void* stream = serializar_string(&size, string);
    if( send(fd_conexion, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}

int recv_string(int fd_conexion, char** string)
{
    size_t size_payload;
    if (recv(fd_conexion, &size_payload, sizeof(size_t), 0) != sizeof(size_t))
    {
        return false;
    }

    void* stream = malloc(size_payload);
    if (recv(fd_conexion, stream, size_payload, 0) != size_payload) {
        free(stream);
        return false;
    }

    deserializar_string(stream, string);

    free(stream);
    return true;
}

// ### SERIALIZACION 
void* serializar_string(size_t* size, char* string)
{
    size_t size_string = strlen(string) + 1;

    *size = 
          sizeof(op_code)   // codigo de operacion
        + sizeof(size_t)    // total (payload)
        + sizeof(size_t)    // size de char* msg
        + size_string;      // char* msg

    size_t size_payload = *size - sizeof(op_code) - sizeof(size_t);

    void* stream = malloc(*size);

    op_code cop = MENSAJE;
    memcpy(stream, &cop, sizeof(op_code));
    memcpy(stream+sizeof(op_code), &size_payload, sizeof(size_t));
    memcpy(stream+sizeof(op_code)+sizeof(size_t), &size_string, sizeof(size_t));
    memcpy(stream+sizeof(op_code)+sizeof(size_t)*2, string, size_string);

    return stream;
}

// ### DESERIALIZACION 

void deserializar_string(void* stream, char** string)
{
    // Recibo tamanio de la cadena
    size_t size_string; 
    memcpy(&size_string, stream, sizeof(size_t));

    // Guardo cadena
    char* string_tam = malloc(size_string);
    memcpy(string_tam, stream+sizeof(size_t), size_string);
    *string = string_tam;
}

// ### HANDSHAKE

int handshake_client(t_log *logger,int fd_conexion)
{
    size_t bytes;
    int32_t handshake = 1; // BUFFER
    int32_t result=0;

    bytes = send(fd_conexion, &handshake, sizeof(int32_t), 0);
    if(bytes==-1) {
        log_error(logger,"Error al enviar handshake al servidor");
        return -1; 
    }
    bytes = recv(fd_conexion, &result, sizeof(int32_t), MSG_WAITALL);
    if(bytes==-1) {
        log_error(logger,"Error al enviar handshake al servidor");
        return -1; 
    }

    if (result == 0) {
        log_info(logger,"Handshake OK");
    } else {
        log_info(logger,"Handshake ERROR");
    }
    
    if(bytes!=sizeof(int32_t))
    {
        return -1;
    }
    
    return 0;
}

int handshake_serv(t_log *logger,int fd_conexion)
{
    size_t bytes;

    int32_t handshake;  // BUFFER
    int32_t resultOk = 0;
    int32_t resultError = -1;

    bytes = recv(fd_conexion, &handshake, sizeof(int32_t), MSG_WAITALL);
    if (handshake == 1) {
        bytes = send(fd_conexion, &resultOk, sizeof(int32_t), 0);
    } else {
        bytes = send(fd_conexion, &resultError, sizeof(int32_t), 0);
    }

    if(bytes!=sizeof(int32_t)){
        return -1;
    }
    
    return 0;
}

// ### SOCKETS (SERVER & CLIENTE)

int crear_socket(t_log *logger, enum T_SOCKET tipo, char* ip, char* puerto)
{
    int err;
    struct addrinfo hints;
    struct addrinfo *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(tipo == SERVER) { hints.ai_flags = AI_PASSIVE; }

    err = getaddrinfo(ip, puerto, &hints, &server_info);

    if(err == -1 ){
        if(tipo == SERVER) {
            log_info(logger, "Error al crear socket server");
        } else if(tipo == CLIENTE) {
            log_info(logger, "Error al crear socket cliente");
        }
        return 0;
    }

    // CREA SOCKET
    int fd_escucha = socket(server_info->ai_family,
                            server_info->ai_socktype,
                            server_info->ai_protocol);
    if(fd_escucha == -1) {
        log_error(logger,"Error creando el socket para %s:%s",ip,puerto);
        return 0;
    }

    if(tipo == SERVER) {
        int activado=1;
        setsockopt(fd_escucha, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));
        err = bind(fd_escucha, server_info->ai_addr, server_info->ai_addrlen);
        if(err!=0){printf("ERROR BIND() SERVER ");}
        err = listen(fd_escucha, SOMAXCONN);
        if(err!=0){printf("ERROR LISTEN() SERVER ");}
    }else if(tipo == CLIENTE) {
	    err = connect(fd_escucha, server_info->ai_addr, server_info->ai_addrlen);
        if(err!= 0){printf("NO SE PUDO CONECTAR");}
    }

    freeaddrinfo(server_info); 

    return fd_escucha;   
}

int esperar_cliente(t_log* logger, char* nombre, int socket_servidor)
{
    struct sockaddr_in dir_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);
    int socket_cliente = accept(socket_servidor, (void*)&dir_cliente, &tam_direccion);
    if(socket_cliente == -1) {
        log_info(logger, "Error al aceptar cliente");
        return -1;
    }

    log_info(logger, "Cliente conectado (a %s)\n", nombre);
    
    return socket_cliente;
}

//### MENSAJES Y LOGGERS

void decir_hola(char* quien) 
{
    printf("Hola desde %s!!\n", quien);
}

t_log* iniciar_logger(char* path, char* nombre)
{
	t_log* logger;
	//Ej: log_create("kernel.log","KERNEL",1,LOG_LEVEL_INFO)
	if((logger = log_create(path,nombre,1,LOG_LEVEL_INFO)) == NULL){
		printf("no pude crear el logger");
		exit(EXIT_FAILURE);
		//exit(1);
	}
	return logger;
} 
