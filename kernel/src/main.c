#include <stdlib.h>
#include <stdio.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_kernel config;
	config = kernel_cargar_config("kernel.config");

	t_log *logger = iniciar_logger("kernel.log","KERNEL");
	log_info(logger,"LOG KERNEL %s\n",config.ip_cpu);

	// No hardcodear IP y puerto, leer de config
    
	int conexion = crear_socket(logger,CLIENTE,IP_GENERICA,"4444");

	//free_config_cpu(&config);
	log_info(logger,"CPU Finalizada.");
	log_destroy(logger);
	
	return 0;
}
