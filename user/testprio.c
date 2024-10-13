// En user/testprio.c
#include "kernel/types.h"
#include "user/user.h"

int main() {
  int pid;

  for (int i = 0; i < 20; i++) {
    pid = fork();
    if (pid == 0) {
      // Este es el proceso hijo
      printf("Ejecutando proceso %d\n", getpid());
      sleep(20);  // Haz que el proceso se detenga un poco
      exit(0);    // Termina el proceso
    }
  }

  // Espera que todos los hijos terminen
  for (int i = 0; i < 20; i++) {
    wait(0);
  }

  exit(0);
}
