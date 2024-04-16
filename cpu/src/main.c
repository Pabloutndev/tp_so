#include <stdlib.h>
#include <stdio.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_cpu config;
	t_log* logger = iniciar_logger("cpu.log","CPU");

	config = cpu_cargar_config("cpu.config");
	
	log_info(logger,"CPU Iniciado! \n");

	int conexion_fd = crear_socket(logger,CLIENTE,config.ip_memoria,config.puerto_memoria);
	log_info(logger,"Enviamos un HOLA MEMORIA! \n");
	send_string(conexion_fd, "HOLA MEMORIA!");
	log_info(logger,"Fin de la charla con memoria.");
	
	int server_fd = crear_socket(logger,SERVER,IP_GENERICA,config.puerto_dispatch);
	log_info(logger, "Servidor CPU listo para recibir clientes");

	while(server_detach(logger,"CPU",server_fd));
	
	log_info(logger,"El modulo CPU Finalizo.");

	close(server_fd);
	log_destroy(logger);

    return 0;
}


