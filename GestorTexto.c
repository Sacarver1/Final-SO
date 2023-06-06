#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncurses.h>

#define MAX_LINE_LENGTH 80 // Longitud máxima de una línea

typedef struct {
    char nombre[100];
    char contenido[1000];
} Archivo;

void* hiloFormatoDesborde(void* arg) {
    Archivo* archivo = (Archivo*)arg;

    while (1) {
        // Obtener el ancho de la pantalla
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        int anchoPantalla = w.ws_col;

        // Copiar el texto original para modificarlo
        char textoFormateado[1000];
        strcpy(textoFormateado, archivo->contenido);

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

void abrirArchivo(Archivo* archivo) {
    clear();
    printf("Opción: Abrir archivo\n");
    printf("Ingrese el nombre del archivo: ");
    scanf("%s", archivo->nombre);
    // Implementación para abrir el archivo
    // Aquí puedes leer el contenido del archivo y almacenarlo en archivo->contenido
    printf("Archivo abierto exitosamente.\n");
    refresh();
    getch();
}

void guardarArchivo(Archivo* archivo) {
    clear();
    printf("Opción: Guardar archivo\n");
    // Implementación para guardar el archivo actual
    // Aquí puedes guardar el contenido de archivo->contenido en el archivo actual
    printf("Archivo guardado exitosamente.\n");
    refresh();
    getch();
}

void guardarComo(Archivo* archivo) {
    clear();
    printf("Opción: Guardar como...\n");
    printf("Ingrese el nombre del archivo: ");
    scanf("%s", archivo->nombre);
    // Implementación para guardar el archivo con otro nombre
    // Aquí puedes guardar el contenido de archivo->contenido en un archivo con el nuevo nombre
    printf("Archivo guardado como '%s' exitosamente.\n", archivo->nombre);
    refresh();
    getch();
}

void escribirArchivo(Archivo* archivo) {
    clear();
    printf("Opción: Escribir archivo\n");
    printf("Ingrese el contenido del archivo:\n");
    getchar(); // Limpiar el búfer de entrada
    fgets(archivo->contenido, sizeof(archivo->contenido), stdin);
    // Implementación para escribir en el archivo
    printf("Contenido del archivo actualizado.\n");
    refresh();
    getch();
}

void cambiarColorLetra() {
    clear();
    printf("Opción: Cambiar color de la letra\n");
    // Implementación para cambiar el color de la letra
    printf("Color de letra cambiado exitosamente.\n");
    refresh();
    getch();
}

void configuracion() {
    clear();
    printf("Opción: Configuración\n");
    // Implementación para la configuración
    printf("Configuración realizada exitosamente.\n");
    refresh();
    getch();
}

int main() {
    pthread_t hilo;
    Archivo archivo;
    char opcion;

    initscr(); // Inicializar la pantalla de ncurses
    keypad(stdscr, TRUE); // Habilitar el uso de teclas especiales

    printf("=== Procesador de Texto ===\n");

    while (1) {
        clear();
        printf("\nOpciones:\n");
        printf("1. Abrir archivo\n");
        printf("2. Guardar archivo\n");
        printf("3. Guardar como...\n");
        printf("4. Escribir archivo\n");
        printf("5. Cambiar color de la letra\n");
        printf("6. Configuración\n");
        printf("7. Salir\n");
        printf("Seleccione una opción: ");
        refresh();

        opcion = getch();

        switch (opcion) {
            case '1': // Abrir archivo
                abrirArchivo(&archivo);
                break;

            case '2': // Guardar archivo
                guardarArchivo(&archivo);
                break;

            case '3': // Guardar como...
                guardarComo(&archivo);
                break;

            case '4': // Escribir archivo
                escribirArchivo(&archivo);
                break;

            case '5': // Cambiar color de la letra
                cambiarColorLetra();
                break;

            case '6': // Configuración
                configuracion();
                break;

            case '7': // Salir
                clear();
                printf("Saliendo del programa...\n");
                refresh();
                endwin(); // Finalizar la pantalla de ncurses
                exit(0);

            default:
                clear();
                printf("Opción inválida. Intente de nuevo.\n");
                refresh();
                getch();
                break;
        }
    }

    // Crear el hilo de formato de desborde
    if (pthread_create(&hilo, NULL, hiloFormatoDesborde, (void*)&archivo) != 0) {
        printf("Error al crear el hilo de formato de desborde.\n");
        exit(1);
    }

    pthread_join(hilo, NULL); // Esperar a que el hilo termine (esto no se ejecutará en este ejemplo)

    return 0;
}

