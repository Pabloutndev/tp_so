#include <main.h>
#include <utils/configs.h>

t_log* logger;
t_log* iniciar_logger(void);

int main(int argc, char* argv[]) {
    //decir_hola("Kernel");
	config_kernel kernel;
	kernel = kernel_cargar_config("kernel.config");

	logger = iniciar_logger();
	log_info(logger,"SOY ALTO LOG  %s\n",kernel.ip_cpu);
    return 0;
}

t_log* iniciar_logger(void)
{
	if((logger = log_create("kernel.log","KERNEL",1,LOG_LEVEL_INFO)) == NULL){
		printf("no pude crear el logger");
		exit(EXIT_FAILURE);
		//exit(1);
	}
	return logger;
} 