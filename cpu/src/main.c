#include <stdlib.h>
#include <stdio.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_cpu config;
	config = cpu_cargar_config("cpu.config");

	t_log* logger = iniciar_logger("cpu.log","CPU");
	log_info(logger,"LOG CPU \n");
    return 0;
}


