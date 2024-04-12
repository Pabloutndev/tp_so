#include <main.h>
#include <utils/configs.h>
#include <utils/conexion.h>

int main(int argc, char* argv[]) {
	config_kernel kernel;
	kernel = kernel_cargar_config("kernel.config");

	t_log *logger = iniciar_logger("kernel.log","KERNEL");
	log_info(logger,"LOG KERNEL %s\n",kernel.ip_cpu);
    return 0;
}
