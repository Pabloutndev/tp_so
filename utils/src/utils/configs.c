#include <utils/configs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

config_kernel kernel_cargar_config(char* ruta)
{
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

    // borrar config_archivo

    return kernel;
}

config_cpu cpu_cargar_config(char* ruta)
{
    t_config* config_archivo = config_create(ruta);
    config_cpu cpu;

    if(config_archivo==NULL) {
        perror("Archivo de configuracion del cpu no encontrado");
    }

    cpu.ip_memoria         = config_get_string_value(config_archivo, "IP_MEMORIA");
    cpu.puerto_memoria     = config_get_string_value(config_archivo, "PUERTO_MEMORIA");
    cpu.puerto_dispatch    = config_get_string_value(config_archivo, "PUERTO_ESCUCHA_DISPATCH");
    cpu.puerto_interrupt   = config_get_string_value(config_archivo, "PUERTO_ESCUCHA_INTERRUPT");
    cpu.tlb_cant_ent       = config_get_int_value(config_archivo, "CANTIDAD_ENTRADAS_TLB");
    cpu.tlb_algoritmo      = config_get_string_value(config_archivo, "ALGORITMO_TLB");
    
    // borrar config_archivo

    return cpu;
}

config_memoria memoria_cargar_config(char* ruta)
{
    t_config* config_archivo = config_create(ruta);
    config_memoria memoria;

    if(config_archivo==NULL) {
        perror("Archivo de configuracion del memoria no encontrado");
    }

    memoria.puerto_escucha      = config_get_string_value(config_archivo, "PUERTO_ESCUCHA");
    memoria.tam_memoria         = config_get_int_value(config_archivo, "TAM_MEMORIA");
    memoria.tam_pagina          = config_get_int_value(config_archivo, "TAM_PAGINA");
    memoria.path_instrucciones  = config_get_string_value(config_archivo, "PATH_INSTRUCCIONES");
    memoria.retardo_respuesta   = config_get_int_value(config_archivo, "RETARDO_RESPUESTA");

    // borrar config_archivo

    return memoria;
}

config_io interfaz_cargar_config(char* ruta)
{
    t_config* config_archivo = config_create(ruta);
    config_io interfaz;

    if(config_archivo==NULL) {
        perror("Archivo de configuracion de entrada-salida no encontrado");
    }

    interfaz.tipo_interfaz          = config_get_string_value(config_archivo,"TIPO_INTERFAZ");
    interfaz.tiempo_unidad_trabajo  = config_get_int_value(config_archivo,"TIEMPO_UNIDAD_TRABAJO");
    interfaz.ip_kernel              = config_get_string_value(config_archivo,"IP_KERNEL");
    interfaz.port_kernel            = config_get_string_value(config_archivo,"PUERTO_KERNEL");
    interfaz.ip_memoria             = config_get_string_value(config_archivo,"IP_MEMORIA");
    interfaz.puerto_memoria         = config_get_string_value(config_archivo,"PUERTO_MEMORIA");
    interfaz.path_base_dialfs       = config_get_array_value(config_archivo,"PATH_BASE_DIALFS");
    interfaz.block_size             = config_get_int_value(config_archivo,"BLOCK_SIZE");
    interfaz.block_count            = config_get_int_value(config_archivo,"BLOCK_COUNT");
    
    // borrar config_archivo 

    return interfaz;
}

/* 
    ## COMENTARIOS ##
    path = "../kernel.config" en VSC O ECLIPSE
    path = "kernel.config" uso de make
*/
    