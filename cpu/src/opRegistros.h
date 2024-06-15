#ifndef OPERATIONS_REG_H
#define OPERATIONS_REG_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <utils/cpu.h>

static const struct
{
    const char* name;
    int value;
} registers[] = {
    {"AX", 0},
    {"BX", 1},
    {"CX", 2},
    {"DX", 3},
    {"EAX", 4},
    {"EBX", 5},
    {"ECX", 6},
    {"EDX", 7},
    {"SI", 8},
    {"DI", 9},
    {"PC", 10}};

void set_register_value(t_cpu *cpu, const char* register_name, uint32_t value);
uint32_t get_register_value(t_cpu *cpu, const char* register_name);
void sum(t_cpu *cpu, const char* dest_register, const char* src_register);
void sub(t_cpu *cpu, const char* dest_register, const char* src_register);
void jnz(t_cpu *cpu, uint32_t instruction);
void io_gen_sleep(uint32_t work_units);

#endif /*OPERATIONS_REG_H*/