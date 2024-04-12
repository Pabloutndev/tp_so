#include <stdio.h>
#include <stdlib.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_io config;
	config = interfaz_cargar_config("entradasalida.config");

	t_log* logger = iniciar_logger("memoria.log","MEMORIA");
	log_info(logger,"LOG INTERFAZ \n");

    // config_destroy(config);

    return 0;
}
