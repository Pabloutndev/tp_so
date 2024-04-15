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
typedef struct
{
    char* ip_memoria;
    char* puerto_memoria;
    char* puerto_dispatch;
    char* puerto_interrupt;
    int tlb_cant_ent;
    char* tlb_algoritmo;
} config_cpu;

typedef struct
{
    char* puerto_escucha;
    int tam_memoria;
    int tam_pagina;
    char* path_instrucciones;
    int retardo_respuesta;
} config_memoria;

typedef struct
{
    char* tipo_interfaz;
    int tiempo_unidad_trabajo;
    char* ip_kernel;
    char* puerto_kernel;
    char* ip_memoria;
    char* puerto_memoria;
    char** path_base_dialfs;
    int block_size;
    int block_count;
} config_io;

// CONFIGURACION DEL KERNEL
config_kernel kernel_cargar_config(char* ruta);

// CONFIGURACION DEL CPU
config_cpu cpu_cargar_config(char* ruta);

// CONFIGURACION DEL MEMORIA
config_memoria memoria_cargar_config(char* ruta);

// CONFIGURACION DEL INTERFAZ_IO
config_io interfaz_cargar_config(char* ruta);

#endif /*CONFIGS_H_*/