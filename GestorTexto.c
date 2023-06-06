#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncurses.h>

#define MAX_LINE_LENGTH 80 // Longitud máxima de una línea

void abrirArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char caracter;
    while ((caracter = fgetc(archivo)) != EOF) {
        printf("%c", caracter);
    }

    fclose(archivo);
}

void guardarArchivo(const char* nombreArchivo, const char* contenido) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("No se pudo guardar el archivo.\n");
        return;
    }

    fprintf(archivo, "%s", contenido);

    fclose(archivo);
}

void* hiloAutoguardado(void* arg) {
    const char* nombreArchivo = (const char*)arg;

    while (1) {
        // Realizar el autoguardado cada 5 segundos
        sleep(5);

        // Leer el contenido actual del archivo
        FILE* archivo = fopen(nombreArchivo, "r");
        if (archivo == NULL) {
            printf("No se pudo abrir el archivo.\n");
            continue;
        }

        char contenido[1000];
        size_t bytesLeidos = fread(contenido, sizeof(char), sizeof(contenido), archivo);
        fclose(archivo);

        // Guardar el contenido en un archivo de respaldo
        FILE* respaldo = fopen("respaldo.txt", "w");
        if (respaldo == NULL) {
            printf("No se pudo crear el archivo de respaldo.\n");
            continue;
        }

        fwrite(contenido, sizeof(char), bytesLeidos, respaldo);
        fclose(respaldo);

        printf("Autoguardado realizado con éxito.\n");
    }

    return NULL;
}

void* hiloFormatoDesborde(void* arg) {
    char* texto = (char*)arg;

    while (1) {
        // Obtener el ancho de la pantalla
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        int anchoPantalla = w.ws_col;

        // Copiar el texto original para modificarlo
        char textoFormateado[1000];
        strcpy(textoFormateado, texto);

        // Verificar cada línea y ajustar el formato si se desborda el ancho de la pantalla
        char* linea = strtok(textoFormateado, "\n");
        while (linea != NULL) {
            if (strlen(linea) > anchoPantalla) {
                // Ajustar formato de la línea para evitar el desborde
                int i;
                for (i = anchoPantalla; i < strlen(linea); i += anchoPantalla) {
                    memmove(&linea[i + 1], &linea[i], strlen(linea) - i);
                    linea[i] = '\n';
                }
            }

            linea = strtok(NULL, "\n");
        }

        // Mostrar el texto formateado en pantalla
        printf("%s\n", textoFormateado);

        sleep(1); // Esperar 1 segundo antes de verificar el formato nuevamente
    }

    return NULL;
}

int tiempoAutoguardado = 10; // Tiempo de autoguardado en segundos
int esquemaColores = 1; // 1: Claro, 2: Oscuro

void configurarAutoguardado() {
    printf("Configuración de Autoguardado\n");
    printf("Ingrese el tiempo de autoguardado (en segundos): ");
    scanf("%d", &tiempoAutoguardado);
    printf("La configuración de autoguardado se ha guardado.\n");
}

void configurarEsquemaColores() {
    printf("Configuración de Esquema de Colores\n");
    printf("Seleccione el esquema de colores:\n");
    printf("1. Claro\n");
    printf("2. Oscuro\n");
    printf("Ingrese su elección: ");
    scanf("%d", &esquemaColores);
    printf("La configuración de esquema de colores se ha guardado.\n");
}

void mostrarMenu() {
    printf("=== Procesador de Texto ===\n");
    printf("\nOpciones:\n");
    printf("1. Abrir archivo\n");
    printf("2. Guardar archivo\n");
    printf("3. Guardar como...\n");
    printf("4. Cambiar color de la letra\n");
    printf("5. Configuración\n");
    printf("6. Salir\n");
}

int main() {
    char opcion;
    char nombreArchivo[100];
    char contenido[1000];

    pthread_t hiloAutoguardado;
    pthread_t hiloFormatoDesborde;

    initscr(); // Inicializar la pantalla de ncurses
    keypad(stdscr, TRUE); // Habilitar el uso de teclas especiales

    mostrarMenu();

    while (1) {
        printf("\nSeleccione una opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case '1': // Abrir archivo
                printf("\nIngrese el nombre del archivo: ");
                scanf("%s", nombreArchivo);
                abrirArchivo(nombreArchivo);
                break;

            case '2': // Guardar archivo
                if (strlen(nombreArchivo) == 0) {
                    printf("No se ha abierto ningún archivo.\n");
                    break;
                }

                printf("\nIngrese el contenido del archivo:\n");
                scanf(" %[^\n]s", contenido);
                guardarArchivo(nombreArchivo, contenido);
                break;

            case '3': // Guardar como...
                printf("\nIngrese el nombre del archivo: ");
                scanf("%s", nombreArchivo);
                printf("Ingrese el contenido del archivo:\n");
                scanf(" %[^\n]s", contenido);
                guardarArchivo(nombreArchivo, contenido);
                break;

            case '4': // Cambiar color de la letra
                printf("Opción: Cambiar color de la letra\n");
                // Implementación para cambiar el color de la letra
                break;

            case '5': // Configuración
                printf("\nConfiguración:\n");
                printf("1. Configurar Autoguardado\n");
                printf("2. Configurar Esquema de Colores\n");
                printf("Seleccione una opción de configuración: ");
                scanf(" %c", &opcion);

                switch (opcion) {
                    case '1':
                        configurarAutoguardado();
                        break;

                    case '2':
                        configurarEsquemaColores();
                        break;

                    default:
                        printf("Opción inválida. Intente de nuevo.\n");
                        break;
                }

                break;

            case '6': // Salir
                printf("Saliendo del programa...\n");
                endwin(); // Terminar la pantalla de ncurses
                exit(0);

            default:
                printf("Opción inválida. Intente de nuevo.\n");
                break;
        }

        mostrarMenu();
    }

    return 0;
}

