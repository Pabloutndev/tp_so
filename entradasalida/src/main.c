#include <stdio.h>
#include <stdlib.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_io config;
	t_log* logger = iniciar_logger("entradasalida.log","entradasalida");
	
	config = interfaz_cargar_config("entradasalida.config");

	log_info(logger,"LOG INTERFAZ I/O \n");
	
	int con_kr_fd = crear_socket(logger,CLIENTE,IP_GENERICA,config.puerto_kernel);
	log_info(logger,"Enviamos un HOLA KERNEL! \n");
	send_string(con_kr_fd, "HOLA KERNEL!");
	log_info(logger,"Fin de la charla con KERNEL.\n");

	int con_mem_fd = crear_socket(logger,CLIENTE,IP_GENERICA,config.puerto_memoria);
	log_info(logger,"Enviamos un HOLA MEMORIA! \n");
	send_string(con_mem_fd, "HOLA MEMORIA!");
	log_info(logger,"Fin de la charla con MEMORIA.\n");

	log_info(logger,"Modulo Interfaz finalizado.");

	close(con_kr_fd);
	close(con_mem_fd);
	log_destroy(logger);
	
	return 0;
}
