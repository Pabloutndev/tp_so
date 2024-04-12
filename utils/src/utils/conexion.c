#include <utils/conexion.h>

// ### SOCKETS

int crear_socket(t_log *logger, enum T_SOCKET tipo, char* ip, char* puerto)
{
    int err;
    struct addrinfo hints, *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if(tipo == SERVER) { hints.ai_flags = AI_PASSIVE; }

    if(tipo == SERVER) { // servidor
        err = getaddrinfo(ip, puerto, &hints, &server_info);
        if(err == -1) { log_info(logger, "Error al crear socket server"); }
    } else if(tipo == CLIENTE){
        err = getaddrinfo(ip,puerto, &hints, &server_info);
        if(err == -1) { log_info(logger, "Error al crear socket cliente"); }
    }

    // CREA SOCKET
    int fd_escucha = socket(server_info->ai_family,
                            server_info->ai_socktype,
                            server_info->ai_protocol);
    if(tipo == SERVER) {
        err = bind(fd_escucha, server_info->ai_addr, server_info->ai_addrlen);
        if(err!=0){printf("ERROR BIND() SERVER ");}
        err = listen(fd_escucha, SOMAXCONN);
        if(err!=0){printf("ERROR LISTEN() SERVER ");}
    }else if(tipo == CLIENTE) {
	    err = connect(fd_escucha, server_info->ai_addr, server_info->ai_addrlen);
        if(err!= 0){printf("ERROR CONNECT() CLIENTE");}
    }

    freeaddrinfo(server_info); 

    return fd_escucha;   
}

int esperar_cliente(t_log* logger, int socket_servidor)
{
	/*int socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;*/
    struct sockaddr_storage cliente_addr;
    socklen_t addr_size = sizeof(cliente_addr);
    int socket_cliente = accept(socket_servidor, NULL, NULL);
    if(socket_cliente == -1) {
        log_error(logger, "Error al aceptar cliente");
        return -1;
    }
    log_info(logger, "Se conectó un cliente!");
    return socket_cliente;
}

//### MENSAJES Y LOGGERS

void decir_hola(char* quien) {
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