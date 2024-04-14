#include <stdio.h>
#include <stdlib.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_io config;
	t_log* logger = iniciar_logger("memoria.log","MEMORIA");
	
	config = interfaz_cargar_config("entradasalida.config");

	log_info(logger,"LOG INTERFAZ \n");
    
	int conexion_fd = crear_socket(logger,CLIENTE,IP_GENERICA,PUERTO_MEMORIA);
	if(handshake_client(logger,conexion_fd) == -1){
		log_error(logger,"Error en el handshake con el servidor");
		return 1;
	}

	close(conexion_fd);

	//libera_config_io(&config);
	log_info(logger,"Modulo Interfaz finalizado.");
	log_destroy(logger);
	
	return 0;
}
