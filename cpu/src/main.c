#include <stdlib.h>
#include <stdio.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_cpu config;
	t_log* logger = iniciar_logger("cpu.log","CPU");

	config = cpu_cargar_config("cpu.config");
	
	log_info(logger,"CPU Iniciado! \n");

	// Conexión SOCKET cliente IP:PUERTO_MEMORIA
	int conexion_fd = crear_socket(logger,CLIENTE,IP_GENERICA,PUERTO_MEMORIA);
	log_info(logger,"Enviamos un HOLA MEMORIA! \n");
	send_string(conexion_fd, "HOLA MEMORIA!");
	log_info(logger,"Fin de la charla con memoria.");

	// Creamos una conexión SOCKET SERVIDOR IP:PUERTO_CPU
	int server_fd = crear_socket(logger,SERVER,IP_GENERICA,PUERTO_CPU);
	log_info(logger, "Servidor listo para recibir al cliente");
	//int cliente_fd = esperar_cliente(logger,server_fd); // ESTO FUNCIONA
	while(server_detach(logger,"CPU",server_fd));
	
	/*if (handshake_serv(logger, cliente_fd) == -1) {
        log_error(logger,"Error en el handshake con el cliente\n");
        exit(1);
    }*/
	
	log_info(logger,"CPU Finalizada.");

	close(server_fd);

	//liberar_config_cpu(&config);
	log_destroy(logger);

    return 0;
}


