#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

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

void escribirArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "a");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    printf("Ingrese el texto a escribir en el archivo (Ctrl+D para finalizar):\n");
    char linea[100];
    while (fgets(linea, sizeof(linea), stdin) != NULL) {
        fputs(linea, archivo);
    }

    fclose(archivo);
    printf("Archivo actualizado exitosamente.\n");
}

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* cambiarColorHilo(void* arg) {
    int opcion = *((int*)arg);

    pthread_mutex_lock(&mutex);

    if (opcion >= 1 && opcion <= 4) {
        int colorPair = 0;

        switch (opcion) {
            case 1: // Rojo
                colorPair = COLOR_PAIR(1);
                break;

            case 2: // Verde
                colorPair = COLOR_PAIR(2);
                break;

            case 3: // Azul
                colorPair = COLOR_PAIR(3);
                break;

            case 4: // Amarillo
                colorPair = COLOR_PAIR(4);
                break;
        }

        attron(colorPair);
        printw("Color de letra cambiado.\n");
        attroff(colorPair);
    } else {
        printw("Opción inválida. Intente de nuevo.\n");
    }

    refresh();
    getch();

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void cambiarColorLetra() {
    clear();
    printw("Opción: Cambiar color de la letra\n");
    printw("Seleccione el color de la letra:\n");
    printw("1. Rojo\n");
    printw("2. Verde\n");
    printw("3. Azul\n");
    printw("4. Amarillo\n");
    printw("Ingrese su elección: ");

    int opcion;
    scanw("%d", &opcion);

    pthread_t hilo;
    pthread_create(&hilo, NULL, cambiarColorHilo, (void*)&opcion);
    pthread_join(hilo, NULL);
}


void configuracionAutoguardado(int* tiempoAutoguardado) {
    printf("Configuración de Autoguardado\n");
    printf("Ingrese el tiempo de autoguardado (en segundos): ");
    scanf("%d", tiempoAutoguardado);
    printf("La configuración de autoguardado se ha guardado.\n");
}

void configuracionEsquemaColores(int* esquemaColores) {
    printf("Configuración de Esquema de Colores\n");
    printf("Seleccione el esquema de colores:\n");
    printf("1. Claro\n");
    printf("2. Oscuro\n");
    printf("Ingrese su elección: ");
    scanf("%d", esquemaColores);
    printf("La configuración de esquema de colores se ha guardado.\n");
}

void abrirArchivoInterfaz(const char* nombreArchivo) {
    clear();
    printw("Opción: Abrir archivo\n");
    abrirArchivo(nombreArchivo);
    refresh();
    getch();
}

void guardarArchivoInterfaz(const char* nombreArchivo, const char* contenido) {
    clear();
    printw("Opción: Guardar archivo\n");
    guardarArchivo(nombreArchivo, contenido);
    refresh();
    getch();
}

void escribirArchivoInterfaz(const char* nombreArchivo) {
    clear();
    printw("Opción: Escribir archivo\n");
    escribirArchivo(nombreArchivo);
    refresh();
    getch();
}

void cambiarColorLetraInterfaz() {
    clear();
    printw("Opción: Cambiar color de la letra\n");
    cambiarColorLetra();
    refresh();
    getch();
}

void configuracionInterfaz(int* tiempoAutoguardado, int* esquemaColores) {
    clear();
    printw("Opción: Configuración\n");
    printw("1. Configurar Autoguardado\n");
    printw("2. Configurar Esquema de Colores\n");
    printw("Seleccione una opción de configuración: ");
    refresh();

    char opcion;
    scanf(" %c", &opcion);

    switch (opcion) {
        case '1':
            configuracionAutoguardado(tiempoAutoguardado);
            break;
        case '2':
            configuracionEsquemaColores(esquemaColores);
            break;
        default:
            printw("Opción inválida. Intente de nuevo.\n");
            refresh();
            getch();
            break;
    }
}

int main() {
    char opcion;
    const char* nombreArchivo = "archivo.txt";
    const char* contenidoArchivo = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";

    initscr(); // Inicializar la pantalla de ncurses
    keypad(stdscr, TRUE); // Habilitar el uso de teclas especiales

    printw("=== Procesador de Texto ===\n");

    while (1) {
        clear();
        printw("\nOpciones:\n");
        printw("1. Abrir archivo\n");
        printw("2. Guardar archivo\n");
        printw("3. Escribir archivo\n");
        printw("4. Cambiar color de la letra\n");
        printw("5. Configuración\n");
        printw("6. Salir\n");
        printw("Seleccione una opción: ");
        refresh();

        opcion = getch();

        switch (opcion) {
            case '1': // Abrir archivo
                abrirArchivoInterfaz(nombreArchivo);
                break;

            case '2': // Guardar archivo
                guardarArchivoInterfaz(nombreArchivo, contenidoArchivo);
                break;

            case '3': // Escribir archivo
                escribirArchivoInterfaz(nombreArchivo);
                break;

            case '4': // Cambiar color de la letra
                cambiarColorLetraInterfaz();
                break;

            case '5': // Configuración
                configuracionInterfaz(&tiempoAutoguardado, &esquemaColores);
                break;

            case '6': // Salir
                clear();
                printw("Saliendo del programa...\n");
                refresh();
                endwin(); // Finalizar la pantalla de ncurses
                return 0;

            default:
                clear();
                printw("Opción inválida. Intente de nuevo.\n");
                refresh();
                getch();
                break;
        }
    }

    return 0;
}

