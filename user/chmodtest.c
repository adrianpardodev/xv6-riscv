#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main() {
    int fd;

    // Crear un archivo
    fd = open("testfile", O_CREATE | O_RDWR);
    if (fd < 0) {
        printf("Error al crear el archivo\n");
        exit(1);
    }

    // Escribir en el archivo
    if (write(fd, "hello", 5) < 0) {
        printf("Error al escribir en el archivo\n");
        exit(1);
    }
    close(fd);

    // Cambiar permisos a solo lectura
    if (chmod("testfile", 1) < 0) {
        printf("Error al cambiar permisos\n");
        exit(1);
    }

    // Intentar escribir con permisos de solo lectura (debe fallar)
    fd = open("testfile", O_WRONLY);
    if (fd >= 0) {
        printf("Error: se pudo abrir en modo escritura con solo lectura\n");
        close(fd);
        exit(1);
    }

    // Cambiar permisos a lectura y escritura nuevamente
    if (chmod("testfile", 3) < 0) {
        printf("Error al cambiar permisos nuevamente\n");
        exit(1);
    }

    // Escribir nuevamente (debe funcionar)
    fd = open("testfile", O_WRONLY);
    if (fd < 0) {
        printf("Error al abrir el archivo en modo escritura\n");
        exit(1);
    }
    if (write(fd, "world", 5) < 0) {
        printf("Error al escribir en el archivo\n");
        close(fd);
        exit(1);
    }
    close(fd);

    printf("Todas las pruebas pasaron correctamente\n");
    exit(0);
}
