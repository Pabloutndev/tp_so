#ifndef OPERATIONS_CPU_H
#define OPERATIONS_CPU_H

#include <stdio.h>
#include <stdint.h>
#include <commons/string.h>
#include <utils/cpu.h>
#include <utils/configs.h>
#include <utils/conexion.h>
#include "opRegistros.h"

// FUnciones del ciclo de instrucción
void ciclo_instruccion_cpu(int client_socket, t_process* process);

void fetch(int socket_mem,t_process* process,char** instruccion);
void decode(t_cpu* cpu, char** instruction);
void execute(t_cpu* cpu, char* instruction);
int check_interrupt(t_process* process, int client_socket);

void pedir_instruccion(int socket, int pid, int32_t pc);
void recibir_instruccion(int skt_mem,char** instruccion);

#endif /*OPERATIONS_CPU_H*/