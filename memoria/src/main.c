#include <stdio.h>
#include <stdlib.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_memoria config;
	t_log* logger = iniciar_logger("memoria.log","memoria");

	config = memoria_cargar_config("memoria.config");
	
	log_info(logger,"### LOG MEMORIA ### \n");

    // Creamos una conexión SOCKET SERVIDOR IP:PUERTO_MEMORIA
	int server_fd = crear_socket(logger,SERVER,IP_GENERICA,PUERTO_MEMORIA);
	log_info(logger, "Servidor %s listo para recibir al cliente","MEMORIA");
	//int cliente_fd = esperar_cliente_threads(logger,server_fd);
	//int cliente_fd = esperar_cliente(logger,server_fd);
	while(server_detach(logger,"MEMORIA",server_fd));
	
	/*if (handshake_serv(logger, cliente_fd) == -1) {
        log_error(logger,"Error en el handshake con el cliente\n");
        exit(1);
    }*/

	log_info(logger,"Modulo memoria finalizado.");
	
	close(server_fd);

	//libera_config_memoria(&config);
	log_destroy(logger);
	
	return 0;
}
