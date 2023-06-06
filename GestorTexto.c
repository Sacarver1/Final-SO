#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncurses.h>

#define MAX_LINE_LENGTH 80 // Longitud m�xima de una l�nea

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

        // Verificar cada l�nea y ajustar el formato si se desborda el ancho de la pantalla
        char* linea = strtok(textoFormateado, "\n");
        while (linea != NULL) {
            if (strlen(linea) > anchoPantalla) {
                // Ajustar formato de la l�nea para evitar el desborde
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
    printf("Opci�n: Abrir archivo\n");
    printf("Ingrese el nombre del archivo: ");
    scanf("%s", archivo->nombre);
    // Implementaci�n para abrir el archivo
    // Aqu� puedes leer el contenido del archivo y almacenarlo en archivo->contenido
    printf("Archivo abierto exitosamente.\n");
    refresh();
    getch();
}

void guardarArchivo(Archivo* archivo) {
    clear();
    printf("Opci�n: Guardar archivo\n");
    // Implementaci�n para guardar el archivo actual
    // Aqu� puedes guardar el contenido de archivo->contenido en el archivo actual
    printf("Archivo guardado exitosamente.\n");
    refresh();
    getch();
}

void guardarComo(Archivo* archivo) {
    clear();
    printf("Opci�n: Guardar como...\n");
    printf("Ingrese el nombre del archivo: ");
    scanf("%s", archivo->nombre);
    // Implementaci�n para guardar el archivo con otro nombre
    // Aqu� puedes guardar el contenido de archivo->contenido en un archivo con el nuevo nombre
    printf("Archivo guardado como '%s' exitosamente.\n", archivo->nombre);
    refresh();
    getch();
}

void escribirArchivo(Archivo* archivo) {
    clear();
    printf("Opci�n: Escribir archivo\n");
    printf("Ingrese el contenido del archivo:\n");
    getchar(); // Limpiar el b�fer de entrada
    fgets(archivo->contenido, sizeof(archivo->contenido), stdin);
    // Implementaci�n para escribir en el archivo
    printf("Contenido del archivo actualizado.\n");
    refresh();
    getch();
}

void cambiarColorLetra() {
    clear();
    printf("Opci�n: Cambiar color de la letra\n");
    // Implementaci�n para cambiar el color de la letra
    printf("Color de letra cambiado exitosamente.\n");
    refresh();
    getch();
}

void configuracion() {
    clear();
    printf("Opci�n: Configuraci�n\n");
    // Implementaci�n para la configuraci�n
    printf("Configuraci�n realizada exitosamente.\n");
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
        printf("6. Configuraci�n\n");
        printf("7. Salir\n");
        printf("Seleccione una opci�n: ");
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

            case '6': // Configuraci�n
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
                printf("Opci�n inv�lida. Intente de nuevo.\n");
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

    pthread_join(hilo, NULL); // Esperar a que el hilo termine (esto no se ejecutar� en este ejemplo)

    return 0;
}

