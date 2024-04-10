#include <utils/configs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

config_kernel kernel_cargar_config(char* ruta)
{
    //t_config* config_archivo = config_create("../kernel.config"); VSC O ECLIPSE
    //t_config* config_archivo = config_create("kernel.config"); uso de make
    t_config* config_archivo = config_create(ruta);
    config_kernel kernel;

    if(config_archivo==NULL) {
        perror("Archivo de configuracion del kernel no encontrado");
    }

    kernel.puerto_escucha            = config_get_string_value(config_archivo, "PUERTO_ESCUCHA");
    kernel.ip_memoria                = config_get_string_value(config_archivo, "IP_MEMORIA");
    kernel.puerto_memoria            = config_get_string_value(config_archivo, "PUERTO_MEMORIA");
    kernel.ip_cpu                    = config_get_string_value(config_archivo, "IP_CPU");
    kernel.puerto_cpu_dispatch       = config_get_string_value(config_archivo, "PUERTO_CPU_DISPATCH");
    kernel.puerto_cpu_interrupt      = config_get_string_value(config_archivo, "PUERTO_CPU_INTERRUPT");
    kernel.algoritmo_planificacion   = config_get_string_value(config_archivo, "ALGORITMO_PLANIFICACION");
    kernel.quantum                   = config_get_int_value(config_archivo, "QUANTUM");
    kernel.recursos                  = config_get_array_value(config_archivo,"RECURSOS");
    kernel.instancias_recursos       = config_get_array_value(config_archivo,"INSTANCIAS_RECURSOS");
    kernel.grado_multiprogramacion   = config_get_int_value(config_archivo, "GRADO_MULTIPROGRAMACION");

    // limpiamos de memoria el config del archivo
    // config_destroy(config_archivo);
    // es necesario hacer copia dinamica a kernel struct 
    // para poder borrar el config_archivo
    // y no tener MEMORY LEAKS
    // retornamos nuestra estructura
    return kernel;
}