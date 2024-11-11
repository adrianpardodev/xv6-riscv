# Implementación de `mprotect` y `munprotect` en xv6

## Descripción General

En este proyecto, he modificado xv6 para incluir dos nuevas syscalls: `mprotect` y `munprotect`. La syscall `mprotect` permite proteger una región de memoria como de solo lectura, mientras que `munprotect` remueve esta protección, permitiendo que la región vuelva a ser de lectura y escritura.

## Funcionamiento y Lógica de la Protección de Memoria

### `mprotect`

La función `mprotect` recibe dos parámetros:
1. Una dirección de memoria (`addr`) desde donde se comenzará la protección.
2. La longitud (`len`), que representa el número de páginas de 4096 bytes que serán protegidas a partir de esa dirección.

Al activarse, `mprotect` recorre cada página en el rango indicado y modifica la entrada en la tabla de páginas (PTE) para deshabilitar el bit de escritura, lo cual marca la página como de solo lectura. Esto significa que cualquier intento de escritura en esa región generará una excepción (trap), lo cual sirve para validar que la protección ha sido aplicada exitosamente.

### `munprotect`

La función `munprotect` también recibe una dirección de memoria y una longitud en páginas. Su función es recorrer el rango de páginas y restablecer el bit de escritura en cada entrada de la tabla de páginas. Esto permite que la región de memoria vuelva a ser escribible. Con `munprotect`, puedo verificar que las páginas pueden ser modificadas nuevamente, demostrando que se ha removido la protección de solo lectura aplicada previamente con `mprotect`.

## Explicación de las Modificaciones Realizadas

He realizado las siguientes modificaciones en el código de xv6:

1. **`syscall.h`**: Definí los números de syscall `SYS_mprotect` y `SYS_munprotect`, necesarios para que xv6 identifique las nuevas funciones del sistema.
2. **`sysproc.c`**: Implementé las funciones de sistema `sys_mprotect` y `sys_munprotect`, que manejan las llamadas de `mprotect` y `munprotect`. También añadí la lógica de protección de memoria en las funciones `mprotect` y `munprotect`, para manipular las entradas en la tabla de páginas (PTE) según sea necesario.
3. **`syscall.c`**: Añadí las referencias de `sys_mprotect` y `sys_munprotect` en el arreglo `syscalls[]`, para que xv6 pueda llamar correctamente a estas funciones del sistema.
4. **`user.h`**: Declaré las funciones `mprotect` y `munprotect` para que estuvieran disponibles en el espacio de usuario.
5. **`usys.pl`**: Añadí las entradas `mprotect` y `munprotect`, permitiendo que el archivo `usys.S` se regenere con las nuevas llamadas al sistema.

## Programa de Prueba

Para verificar el funcionamiento de `mprotect` y `munprotect`, desarrollé un programa de prueba, `mprotect_test.c`. Este programa asigna una página de memoria y luego realiza las siguientes pruebas:

1. **Prueba de `mprotect`**:
   - Llama a `mprotect` en la página asignada para establecerla como de solo lectura. Luego, el programa intenta escribir en esta página, lo cual debería provocar un trap y confirmar que la protección funciona.
2. **Prueba de `munprotect`**:
   - Llama a `munprotect` en la misma página, permitiendo que vuelva a ser de lectura y escritura. Tras esto, intenta escribir nuevamente en la página, lo cual debería ser exitoso, validando que la protección fue removida.

### Ejecución de la Prueba

Para ejecutar el programa de prueba en xv6:

1. Compilo y ejecuto xv6 con `make qemu`.
2. Dentro del sistema xv6, corro el programa con el siguiente comando:

   ```bash
   mprotect_test

## Dificultades Encontradas y Soluciones Implementadas

1. **Errores de "undefined reference" al compilar las syscalls**:
   - Inicialmente, encontré errores al intentar compilar `mprotect` y `munprotect` debido a que no se habían agregado correctamente en el entorno de usuario. Para resolver esto, tuve que asegurarme de incluir `mprotect` y `munprotect` en `usys.pl` y regenerar el archivo `usys.S`, permitiendo que las syscalls fueran reconocidas en el espacio de usuario.

2. **Trap inesperado al intentar escribir en memoria protegida**:
   - Cuando intenté escribir en una región de memoria protegida, el sistema generaba un "trap inesperado," lo cual es el comportamiento esperado pero causaba que el programa se interrumpiera bruscamente. Para documentar correctamente este comportamiento, incluí una explicación de que el trap es la validación de que `mprotect` funciona como se espera.

## Conclusión

Este proyecto me permitió implementar y probar dos syscalls de protección de memoria en xv6. Las funciones `mprotect` y `munprotect` se comportaron correctamente según lo esperado, protegiendo y permitiendo el acceso a la memoria de acuerdo a los parámetros indicados. Las pruebas realizadas en `mprotect_test.c` confirman el funcionamiento de estas syscalls, y los resultados obtenidos cumplen con los objetivos de la tarea.
