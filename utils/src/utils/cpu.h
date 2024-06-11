#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct {
    int8_t AX, BX, CX, DX;
    int32_t EAX, EBX, ECX, EDX;
    int32_t SI, DI;
    int32_t PC; // PROGRAM_COUNTER
} t_cpu;

#endif /* CPU_H */
