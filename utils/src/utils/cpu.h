#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct {
    int8_t AX, BX, CX, DX;
    int32_t EAX, EBX, ECX, EDX;
    int32_t SI, DI;
    int32_t PC; // PROGRAM_COUNTER
} t_cpu;

// Estructura para representar un proceso
typedef struct {
    int PID;
    int QUANTUM; // Quantum para VRR
    t_cpu cpu;
} t_process;

// Estructura para representar una entrada de la TLB
typedef struct {
    int pid;
    int pagina;
    int marco;
} t_tlb_entry;

// Estructura para representar la TLB
typedef struct {
    t_tlb_entry *entries;
    int size;
    int max_entries;
    char *algorithm; // FIFO o LRU
} t_tlb;

#endif /* CPU_H */
