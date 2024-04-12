#include <stdio.h>
#include <stdlib.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_memoria config;
	config = memoria_cargar_config("memoria.config");

	t_log* logger = iniciar_logger("memoria.log","memoria");
	log_info(logger,"LOG MEMORIA \n");
    // Creamos una conexión SOCKET SERVIDOR IP:PUERTO_MEMORIA
	int server_fd = crear_socket(logger,SERVER,IP_GENERICA,PUERTO_MEMORIA);
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(logger,server_fd);
	
	if (handshake_serv(logger, cliente_fd) == -1) {
        log_error(logger,"Error en el handshake con el cliente\n");
        exit(1);
    }

	close(server_fd);

	//libera_config_memoria(&config);
	log_info(logger,"Modulo memoria finalizado.");
	log_destroy(logger);
	
	return 0;
}
