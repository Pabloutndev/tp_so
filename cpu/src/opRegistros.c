#include "opRegistros.h"

// checkpoint - 2 (SET, SUM, SUB, JNZ and IO_GEN_SLEEP)

void set_register_value(t_cpu *cpu, const char* register_name, uint32_t value) 
{
    int tam = sizeof(registers) / sizeof(registers[0]);
    for (int i = 0; i < tam; ++i) {
        if (strcmp(registers[i].name, register_name) == 0) {
            switch (registers[i].value) {
                case 0: cpu->AX = (uint8_t) value; break;
                case 1: cpu->BX = (uint8_t) value; break;
                case 2: cpu->CX = (uint8_t) value; break;
                case 3: cpu->DX = (uint8_t) value; break;
                case 4: cpu->EAX = value; break;
                case 5: cpu->EBX = value; break;
                case 6: cpu->ECX = value; break;
                case 7: cpu->EDX = value; break;
                case 8: cpu->SI = value; break;
                case 9: cpu->DI = value; break;
                case 10: cpu->PC = value; break;
                default: printf("Unknown register\n");break;
            }
            i = tam;
        }
    }
}

uint32_t get_register_value(t_cpu *cpu, const char* register_name) 
{
    for (int i = 0; i < sizeof(registers) / sizeof(registers[0]); ++i) 
    {
        if (strcmp(registers[i].name, register_name) == 0) {
            switch (registers[i].value) {
                case 0: return cpu->AX;
                case 1: return cpu->BX;
                case 2: return cpu->CX;
                case 3: return cpu->DX;
                case 4: return cpu->EAX;
                case 5: return cpu->EBX;
                case 6: return cpu->ECX;
                case 7: return cpu->EDX;
                case 8: return cpu->SI;
                case 9: return cpu->DI;
                case 10: return cpu->PC;
                default: printf("Unknown register\n");break;
            }
        }
    }
    
    return 0;
}

void sum(t_cpu *cpu, const char* dest_register, const char* src_register) 
{
    uint32_t dest_value = get_register_value(cpu, dest_register);
    uint32_t src_value = get_register_value(cpu, src_register);

    set_register_value(cpu, dest_register, dest_value + src_value);
}

void sub(t_cpu *cpu, const char* dest_register, const char* src_register) 
{
    uint32_t dest_value = get_register_value(cpu, dest_register);
    uint32_t src_value = get_register_value(cpu, src_register);

    set_register_value(cpu, dest_register, dest_value - src_value);
}

void jnz(t_cpu *cpu, uint32_t instruction) 
{
    if ( cpu->PC != 0 ) 
    {
        cpu->PC = instruction;
    }
}

void io_gen_sleep(uint32_t work_units) 
{
    // Request to kernel to send an I/O to SLEEP for a
    // socket cpu_interrupt -> kernel
}