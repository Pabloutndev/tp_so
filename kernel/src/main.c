#include <stdlib.h>
#include <stdio.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_kernel config;
	t_log *logger = iniciar_logger("kernel.log","KERNEL");
	
	config = kernel_cargar_config("kernel.config");

	log_info(logger,"LOG KERNEL \n");

	int conexion_fd = crear_socket(logger,CLIENTE,config.ip_cpu,config.puerto_cpu_dispatch);
	log_info(logger,"Enviamos un HOLA a CPU!");
	send_string(conexion_fd, "HOLA CPU!");

	int server_fd = crear_socket(logger,SERVER,IP_GENERICA,config.puerto_escucha);
	log_info(logger, "Servidor KERNEL listo para recibir clientes");

	while(server_detach(logger,"KERNEL",server_fd));

	log_info(logger,"Modulo Kernel Finalizado.");

	close(conexion_fd);
	log_destroy(logger);
	
	return 0;
}
