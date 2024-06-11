#include "consola.h"

extern t_listsProcesses listsProcesses;

int run_console_cmnd(char **cmnd)
{ // run a command from the console
    // string command to integer code conversion
    static const struct
    {
        const char* name;
        int value;
    } commands[] = {
        {"Help", 0},
        {"Sexe", 1},
        {"Pstart", 2},
        {"Pfinish", 3},
        {"Sstart", 4},
        {"Sstop", 5},
        {"Pstate", 6},
        {"Exit", 7}};
    int cmd_code = -1;
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
    {
        if (string_equals_ignore_case(cmnd[0], commands[i].name))
        {
            cmd_code = commands[i].value;
            break;
        }
    }
    if (cmd_code == -1)
    {
        printf("Command not recognized\n");
        return;
    }
    switch (cmd_code)
    {
    case 0: // help
        printf("Commands: \n"
               "  Help  - Show this message\n"
               "  Sexe  - Execute a script\n"
               "  Pstart - Start a process\n"
               "  Pfinish - Finish a process\n"
               "  Sstart - Start scheduling\n"
               "  Sstop - Stop scheduling\n"
               "  Pstate - Show process state\n"
               "  Exit  - Exit the console\n"
               "\n");
        break;
    case 1: // EJECUTAR_SCRIPT_OP
        executeScriptOP("./blabla/scripts.txt");
        break;
    case 2: // INICIAR_PROCESO
        startProcess(cmnd);
        break;
    case 3: // FINALIZAR_PROCESO
        finishProcess(cmnd);
        break;
    case 4: // INICIAR_PLANIFICACION
        startScheduling();
        break;
    case 5: // DETENER_PLANIFICACION
        stopScheduling();
        break;
    case 6: // PROCESO_ESTADO
        listStateProcess();
        break;
    case 7: // SALIR
        return;
        break;
    default:
        printf("Comando no reconocido\n");
        break;
    }
}


void handleConsole(void)
{
    // log_info(logger,"INICIANDO CONSOLA\n");
    printf("CONSOLA INICIADA!\n");
    while (1)
    {
        printf("ingresar un comando: \n");
        char* cmd_all = readline(">");

        char **cmd = string_split(cmd_all, " ");
        
        // int length = string_array_length(cmd);
        // if (string_equals_ignore_case(cmd[0], "help")) printf("Comandos: \n" "  EJECUTAR_SCRIPT_OP \n" "  INICIAR_PROCESO \n" "  FINALIZAR_PROCESO \n" "  INICIAR_PLANIFICACION \n" "  DETENER_PLANIFICACION \n" "  PROCESO_ESTADO \n" "  SALIR \n" "\n"); else if (string_equals_ignore_case(cmd[0], "EJECUTAR_SCRIPT_OP")) executeScriptOP("./blabla/scripts.txt"); else if (string_equals_ignore_case(cmd[0], "INICIAR_PROCESO")) startProcess(cmd); else if (string_equals_ignore_case(cmd[0], "FINALIZAR_PROCESO")) finishProcess(cmd); else if (string_equals_ignore_case(cmd[0], "INICIAR_PLANIFICACION")) startScheduling(firstTime); else if (string_equals_ignore_case(cmd[0], "DETENER_PLANIFICACION")) stopScheduling(); else if (string_equals_ignore_case(cmd[0], "PROCESO_ESTADO")) listStateProcess(); else if (string_equals_ignore_case(cmd[0], "SALIR")) break;
        run_console_cmnd(cmd); // better to the eyes, more modular

        free(cmd_all);
        free(cmd); // while(i<length){free(cmd[i]);i++;}
        // free(length);
    }
}

void executeScriptOP(char *path)
{
    char* tasks = readScriptOP(path);
    // execute tasks[i] (?)
}

void readScriptOP(char *path) // void, returning char*?, whats the intention?
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Error al abrir el archivo");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    rewind(file);

    char* content = (char *)malloc(size + 1);
    if (content == NULL)
    {
        perror("Error de asignación de memoria");
        fclose(file);
        return NULL;
    }

    if (fread(content, 1, size, file) != size)
    {
        perror("Error al leer el archivo");
        free(content);
        fclose(file);
        return NULL;
    }

    content[size] = '\0';

    fclose(file);

    return content;
}

void listStateProcess() 
{
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < 6; i++)
    {
        int cant_procesos = list_size(listsProcesses.listProcess[i].Processes);
        for (int j = 0; j < cant_procesos; j++)
        {
            t_process* process = (t_process*)list_get(listsProcesses.listProcess[i].Processes, j);
            printf(
                "Process ID: %d\tStatus: %s\n",
                process->PID,
                listsProcesses.listProcess[i].name
            );
        }
    }
    printf("------------------------------------------------\n");
}

