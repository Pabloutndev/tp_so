#include <stdlib.h>
#include <stdio.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_cpu config;
	t_log* logger = iniciar_logger("cpu.log","CPU");

	config = cpu_cargar_config("cpu.config");
	
	log_info(logger,"CPU Iniciado! \n");

	// Creamos una conexión SOCKET SERVIDOR IP:PUERTO_CPU
	int server_fd = crear_socket(logger,SERVER,IP_GENERICA,PUERTO_CPU);
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(logger,server_fd);

	//liberar_config_cpu(&config);
	log_info(logger,"CPU Finalizada.");
	log_destroy(logger);

    return 0;
}


