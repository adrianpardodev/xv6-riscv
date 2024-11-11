#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

int mprotect(uint64 addr, int len);
int munprotect(uint64 addr, int len);


uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
uint64
sys_mprotect(void) {
    uint64 addr;
    int len;

    // Obtener los argumentos de la syscall
    argaddr(0, &addr);
    argint(1, &len);

    // Llamar a la implementación de mprotect
    return mprotect(addr, len);
}

uint64
sys_munprotect(void) {
    uint64 addr;
    int len;

    // Obtener los argumentos de la syscall
    argaddr(0, &addr);
    argint(1, &len);

    // Llamar a la implementación de munprotect
    return munprotect(addr, len);
}

int
mprotect(uint64 addr, int len) {
    if (addr == 0 || len <= 0 || addr % PGSIZE != 0) {
        return -1;
    }

    int num_pages = (len + PGSIZE - 1) / PGSIZE;
    struct proc *p = myproc();
    for (int i = 0; i < num_pages; i++) {
        pte_t *pte = walk(p->pagetable, addr + i * PGSIZE, 0);
        if (pte == 0 || (*pte & PTE_V) == 0) {
            return -1;
        }
        *pte &= ~PTE_W;  // Deshabilitar el bit de escritura
    }

    return 0;
}
int
munprotect(uint64 addr, int len) {
    if (addr == 0 || len <= 0 || addr % PGSIZE != 0) {
        return -1;
    }

    int num_pages = (len + PGSIZE - 1) / PGSIZE;
    struct proc *p = myproc();
    for (int i = 0; i < num_pages; i++) {
        pte_t *pte = walk(p->pagetable, addr + i * PGSIZE, 0);
        if (pte == 0 || (*pte & PTE_V) == 0) {
            return -1;
        }
        *pte |= PTE_W;  // Habilitar el bit de escritura
    }

    return 0;
}

