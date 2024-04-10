#ifndef CONFIGS_H_
#define CONFIGS_H_

#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>

typedef struct
{
    char* puerto_escucha;
    char* ip_memoria;
    char* puerto_memoria;
    char* ip_cpu;
    char* puerto_cpu_dispatch;
    char* puerto_cpu_interrupt;
    char* algoritmo_planificacion;
    int quantum;
    char** recursos; //puntero a punteros de strings
    char** instancias_recursos;
    int grado_multiprogramacion;
} config_kernel;

// CONFIGURACION DEL KERNEL
config_kernel kernel_cargar_config(char* ruta);

#endif /*CONFIGS_H_*/