#include <stdio.h>
#include <stdlib.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_memoria config;
	t_log* logger = iniciar_logger("memoria.log","memoria");
	
	config = memoria_cargar_config("memoria.config");
	
	log_info(logger,"### LOG MEMORIA ### \n");

	int server_fd = crear_socket(logger,SERVER,IP_GENERICA,config.puerto_escucha);
	log_info(logger, "Servidor %s listo para recibir al cliente\n","MEMORIA");
	while(server_detach(logger,"MEMORIA",server_fd));
	
	log_info(logger,"Modulo memoria finalizado.\n");

	close(server_fd); 

	log_destroy(logger);
	
	return 0;
}
