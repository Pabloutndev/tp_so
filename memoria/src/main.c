#include <stdio.h>
#include <stdlib.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_memoria config;
	config = memoria_cargar_config("memoria.config");

	t_log* logger = iniciar_logger("memoria.log","memoria");
	log_info(logger,"LOG MEMORIA \n");
    return 0;
}
