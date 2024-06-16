#include "opCPU.h"
#include <string.h>

extern config_cpu config;
extern t_log* logger;
extern sem_t sem_instruccion; // sincronizar fetch y ejecucion
extern int interrupt_pid;
extern pthread_mutex_t interrupt_mutex;

void ciclo_instruccion_cpu(int client_socket, t_process* process)
{
    int conexion_mem = crear_socket(logger,CLIENTE,config.ip_memoria,config.puerto_memoria); 
    char* instruccion = NULL;
    
    while(1)
    {   
        conexion_mem = crear_socket(logger,CLIENTE,config.ip_memoria,config.puerto_memoria);

        fetch(conexion_mem,process,&instruccion);
        
        printf("Instruccion por procesar en cpu: %s\n",instruccion);

        decode(&(process->cpu), &instruccion);
        
        execute(&(process->cpu), instruccion);
        
        if(check_interrupt(process,client_socket))
        {
            break;
        }

        printf("PROCESO:%d - PC: %d\n",process->PID, process->cpu.PC);
    }

    liberar_conexion(conexion_mem);
}

// Fetch: Obtener la instrucción desde la memoria
void fetch(int socket_mem,t_process* process,char** instruccion) 
{
    pedir_instruccion(socket_mem,process->PID,process->cpu.PC);
    recibir_instruccion(socket_mem,instruccion);
}

// Decode: Interpretar la instrucción
void decode(t_cpu* cpu, char** instruction) {
    // uint32_t address = translate_address(cpu->registers.PC, PAGE_SIZE); // Traducimos la dirección
    // strcpy(instruction, "SET AX 1"); // Ejemplo de instrucción
    // Interpretamos la instrucción 
}

// Execute: Ejecutar la instrucción
void execute(t_cpu* cpu, char* instrucciones) {
    // Interpretamos y ejecutamos la instrucción
    char* op;
    char* reg1;
    char* reg2;
    uint32_t value;

    char **instruccion = string_split(instrucciones, " ");

    printf("INSTRUCCION A EJECUTAR: %s\n",instrucciones);

    op = strdup(instruccion[0]);
    reg1 = strdup(instruccion[1]);
    
    // Actualizamos el PC
    cpu->PC += 1;

    if (strcmp(op, "SET") == 0) 
    {
        value = atoi(instruccion[2]);
        set_register_value(cpu, reg1, value);
    } 
    else if (strcmp(op, "SUM") == 0) 
    {
        reg2 = strdup(instruccion[2]);
        sum(cpu, reg1, reg2);
    } 
    else if (strcmp(op, "SUB") == 0) 
    {
        reg2 = strdup(instruccion[2]);
        sub(cpu, reg1, reg2);
    } 
    else if (strcmp(op, "JNZ") == 0) 
    {
        value = atoi(instruccion[2]);
        jnz(cpu, value);
    } 
    else if (strcmp(op, "IO_GEN_SLEEP") == 0) 
    {
        value = atoi(instruccion[2]);
        io_gen_sleep(value);
    } 
    /*else if (strcmp(op, "FIN") == 0)
    {

    }*/

    free(instruccion);
    free(op);
    free(reg2);
    free(reg1);
}

int check_interrupt(t_process* process, int client_socket) 
{
    pthread_mutex_lock(&interrupt_mutex);
    if (interrupt_pid != -1) 
    {
        pthread_mutex_unlock(&interrupt_mutex);
        printf("\nHUBO INTERRUPCION!!!\n");
        enviar_proceso(client_socket, process);
        interrupt_pid = -1;
        return 1;
    } else {
        pthread_mutex_unlock(&interrupt_mutex);
        return 0;
    }
}


void pedir_instruccion(int socket, int pid, int32_t pc)
{
    t_paquete* paquete = crear_paquete_con_codigo_op(PCKT_INSTRUCTION);
    agregar_entero_a_paquete(paquete, pid);
    agregar_entero_a_paquete(paquete, pc);
    enviar_paquete(paquete, socket);
    eliminar_paquete(paquete);
}

void recibir_instruccion(int skt_mem,char** instruccion)
{
    T_PACKET cod_op = recibir_operacion(skt_mem);
    while(cod_op!=PCKT_INSTRUCTION_MEM)
    {
        cod_op = recibir_operacion(skt_mem);
    }
    int size;
    int desplazamiento = 0;
    char* buffer = recibir_buffer(&size, skt_mem);
    
    *instruccion = strdup(leer_string(buffer,&desplazamiento));
    free(buffer);
}




/*    if (interrupt) {
        t_paquete* paquete = crear_paquete_con_codigo_op(PCKT_INTERRUPT);
        agregar_entero_a_paquete(paquete, cpu->PID);
        agregar_entero_a_paquete(paquete, cpu->registers.PC);
        enviar_paquete(paquete, socket_interrupt);
        eliminar_paquete(paquete);
    } else {
        if (cpu->QUANTUM > 0) {
            cpu->QUANTUM--;
            fetch(cpu); // Continuamos con el ciclo de instrucción
        } else {
            // Quantum cumplido, enviar contexto de regreso al kernel
            t_paquete* paquete = crear_paquete_con_codigo_op(PCKT_PROCESO_TERMINADO);
            agregar_entero_a_paquete(paquete, cpu->PID);
            agregar_entero_a_paquete(paquete, cpu->registers.PC);
            enviar_paquete(paquete, socket_interrupt);
            eliminar_paquete(paquete);
        }
    }
}*/

/*
// Inicializar la TLB
t_tlb* init_tlb(int max_entries, char *algorithm) {
    t_tlb *tlb = malloc(sizeof(t_tlb));
    tlb->entries = malloc(sizeof(t_tlb_entry) * max_entries);
    tlb->size = 0;
    tlb->max_entries = max_entries;
    tlb->algorithm = algorithm;
    return tlb;
}

// Buscar en la TLB
int search_tlb(t_tlb *tlb, int pid, int pagina) {
    for (int i = 0; i < tlb->size; i++) {
        if (tlb->entries[i].pid == pid && tlb->entries[i].pagina == pagina) {
            printf("TLB HIT - Pagina: %d\n", pagina);
            return tlb->entries[i].marco;
        }
    }
    printf("TLB MISS - Pagina: %d\n", pagina);
    return -1;
}

// Actualizar la TLB
void update_tlb(t_tlb *tlb, int pid, int pagina, int marco) {
    if (tlb->size < tlb->max_entries) {
        tlb->entries[tlb->size++] = (t_tlb_entry){pid, pagina, marco};
    } else {
        // Reemplazo FIFO
        if (strcmp(tlb->algorithm, "FIFO") == 0) {
            for (int i = 1; i < tlb->max_entries; i++) {
                tlb->entries[i - 1] = tlb->entries[i];
            }
            tlb->entries[tlb->max_entries - 1] = (t_tlb_entry){pid, pagina, marco};
        }
        // Reemplazo LRU
        // ... Implementar LRU
    }
    printf("OBTENER MARCO - Página: %d - Marco: %d\n", pagina, marco);
}

// Simula la MMU para la traducción de direcciones
uint32_t translate_address(uint32_t logical_address, uint32_t page_size) {
    uint32_t page_number = logical_address / page_size;
    uint32_t offset = logical_address % page_size;
    return (page_number * page_size) + offset; // Simulación simplificada
}*/