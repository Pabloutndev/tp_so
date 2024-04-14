#include <stdlib.h>
#include <stdio.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_kernel config;
	t_log *logger = iniciar_logger("kernel.log","KERNEL");
	
	config = kernel_cargar_config("kernel.config");

	log_info(logger,"LOG KERNEL %s\n",config.ip_cpu);

	// No hardcodear IP y puerto, leer de config
    
	int conexion_fd = crear_socket(logger,CLIENTE,IP_GENERICA,PUERTO_CPU);
	/*
	if(handshake_client(logger,conexion_fd) == -1){
		log_error(logger,"Error en el handshake con el servidor");
		return 1;
	}*/

	send_string(conexion_fd, "HOLA CPU, COMO ESTAS?");
	//send_string(conexion_fd, "Segundo saludo");

	log_info(logger,"Modulo Kernel Finalizado.");

	close(conexion_fd);
	//free_config_cpu(&config);
	log_destroy(logger);
	
	return 0;
}
