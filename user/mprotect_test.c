#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    char *addr;
    int len = 1; // Protege una sola página (4096 bytes)

    // Asigna memoria
    addr = sbrk(4096); // Solicita una página de memoria

    printf("Probando mprotect y munprotect:\n");

    // Probar mprotect
    if (mprotect(addr, len) < 0) {
        printf("Error: mprotect falló\n");
        exit(1);
    } else {
        printf("mprotect exitoso: la dirección está protegida como solo lectura\n");
    }

    // Intentar escribir en la memoria protegida (debería fallar)
    printf("Intentando escribir en la dirección protegida...\n");
    *addr = 'A';  // Esto debería causar un error de segmentación si mprotect funciona

    printf("Error: se pudo escribir en la dirección protegida\n");

    // Probar munprotect
    if (munprotect(addr, len) < 0) {
        printf("Error: munprotect falló\n");
        exit(1);
    } else {
        printf("munprotect exitoso: la dirección ahora tiene permiso de escritura\n");
    }

    // Intentar escribir en la memoria nuevamente (debería funcionar)
    printf("Intentando escribir en la dirección después de munprotect...\n");
    *addr = 'B';  // Esto debería funcionar si munprotect funciona
    printf("Escritura exitosa después de munprotect: %c\n", *addr);

    exit(0);
}
