#include <utils/conexion.h>

void decir_hola(char* quien) {
    printf("Hola desde %s!!\n", quien);
}

t_log* iniciar_logger(char* path, char* nombre)
{
	t_log* logger;
	//Ej: log_create("kernel.log","KERNEL",1,LOG_LEVEL_INFO)
	if((logger = log_create(path,nombre,1,LOG_LEVEL_INFO)) == NULL){
		printf("no pude crear el logger");
		exit(EXIT_FAILURE);
		//exit(1);
	}
	return logger;
} 