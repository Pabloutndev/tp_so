#include "opCPU.h"
#include <string.h>

extern config_cpu config;
extern t_log* logger;
extern sem_t sem_instruccion; // sincronizar fetch y ejecucion

void ciclo_instruccion_cpu(t_process* process)
{
    int conexion_mem = crear_socket(logger,CLIENTE,config.ip_memoria,config.puerto_memoria); 
    int skt_srv_int = crear_socket(logger, SERVER, config.ip_cpu, config.puerto_cpu_interrupt);
    char* instruccion = NULL;
    
    pthread_t interrupt_thread;
    pthread_create(&interrupt_thread, NULL, check_interrupt,&skt_srv_int);

    while(1)
    {   
        conexion_mem = crear_socket(logger,CLIENTE,config.ip_memoria,config.puerto_memoria);

        fetch(conexion_mem,process,&instruccion);

        if (instruccion == NULL) {
            break;// Fin del programa
        }
        
        printf("\n Instruccion por procesar en cpu: %s",instruccion);

        decode(&(process->cpu), &instruccion);
        
        execute(&(process->cpu), instruccion);

        if(     strcmp(instruccion,"FIN") == 0) || 
                check_interrupt()
            )
        {
            break;
        }

        printf("\nPROCESO:%d - PC: %d\n",process->PID, process->cpu.PC);
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
    //uint32_t address = translate_address(cpu->registers.PC, PAGE_SIZE); // Traducimos la dirección
    //strcpy(instruction, "SET AX 1"); // Ejemplo de instrucción
    // Interpretamos la instrucción 
}

// Execute: Ejecutar la instrucción
void execute(t_cpu* cpu, char* instrucciones) {
    // Interpretamos y ejecutamos la instrucción
    char* op;
    char* reg1;
    char* reg2;
    uint32_t value;


    //sscanf(instruction, "%s %s %s", op, reg1, reg2);
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

// Check Interrupt: Verificar interrupciones
void check_interrupt(int socket_interrupt) {
    int socket_servidor = 
    
    int interrupt = 0;

    PCKT_FIN_QUANTUM

    while()


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
    }*/
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




/*
char* fetch_instruction(int memoria_sockfd, uint32_t pc) {
    // Solicitar la instrucción a Memoria utilizando el Program Counter
    send(memoria_sockfd, &pc, sizeof(uint32_t), 0);
    char *instruction = malloc(100); // Asumimos que la instrucción tiene un máximo de 100 caracteres
    recv(memoria_sockfd, instruction, 100, 0);
    printf("FETCH - Program Counter: %u\n", pc);
    return instruction;
}
// Función para ejecutar la instrucción
void execute_instruction(char *opcode, char **params, t_cpu *cpu, int memoria_sockfd, int io_sockfd, int kernel_sockfd) {
    // Implementar la lógica de cada instrucción
    if (strcmp(opcode, "SET") == 0) {
        // Ejemplo para SET
        if (strcmp(params[0], "AX") == 0) {
            cpu->AX = atoi(params[1]);
        } else if (strcmp(params[0], "BX") == 0) {
            cpu->BX = atoi(params[1]);
        }
        // ... otras asignaciones
    } else if (strcmp(opcode, "MOV_IN") == 0) {
        // Ejemplo para MOV_IN
        uint32_t direccion_logica = cpu->EAX; // Asumimos que la dirección lógica está en EAX
        // Traducción de dirección lógica a física
        // Comunicación con Memoria
        // Actualización de registros
    }
    
    // ... otras instrucciones
    printf("Ejecutando: %s\n", opcode);


}

// Función para decodificar la instrucción
void decode_instruction(char *instruction, char *opcode, char **params) {
    char *token = strtok(instruction, " ");
    strcpy(opcode, token);
    int i = 0;
    while ((token = strtok(NULL, " ")) != NULL) {
        params[i++] = token;
    }
}
// Función para chequear si hay una interrupción
bool check_interrupt(int kernel_sockfd, t_process *process) {
    int interrupt;
    recv(kernel_sockfd, &interrupt, sizeof(int), MSG_DONTWAIT);
    if (interrupt) {
        send_context(kernel_sockfd, process);
        return true;
    }
    return false;
}


void ciclo_cpu() {
    int program_counter = 0;
    //char instruccion[MAX_INSTRUCTION_LENGTH];
    //Instruccion instr_decoded;

    while (1) {
        fetch_instruccion(program_counter, instruccion);
        decode_instruccion(instruccion, &instr_decoded);
        execute_instruccion(&instr_decoded);
        check_interrupt(kernel,process);

        // Actualizar el Program Counter (esto es solo un ejemplo)
        program_counter++;
    }
}


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
}*/

// Simula la MMU para la traducción de direcciones
uint32_t translate_address(uint32_t logical_address, uint32_t page_size) {
    uint32_t page_number = logical_address / page_size;
    uint32_t offset = logical_address % page_size;
    return (page_number * page_size) + offset; // Simulación simplificada
}