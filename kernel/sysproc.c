#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
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
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
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
  return kkill(pid);
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
// kernel/sysproc.c

// kernel/sysproc.c


// MODIFICACIÓN PROYECTO: System Call para cambiar prioridad
// Implementación de la llamada al sistema sys_set_priority (syscall #22).
// Permite a un proceso modificar su propia prioridad dinámicamente.
// Argumentos:
//    - priority: Entero entre 0 (baja) y 100 (alta).
// Retorno:
//    - 0: Éxito.
//    - -2: Error (prioridad fuera de rango).
uint64
sys_set_priority(void)
{
  int priority; // Quitamos 'pid' porque no se usa

  // En xv6-riscv, argint no devuelve errores, solo busca el valor.
  argint(0, &priority);
  
  // Validamos que la prioridad sea lógica (0 a 100)
  if(priority < 0 || priority > 100)
    return -2;

  // Aplicamos el cambio al proceso actual
  struct proc *p = myproc();
  
  acquire(&p->lock);
  p->priority = priority;
  release(&p->lock);

  // Cedemos la CPU para que el scheduler re-evalúe
  yield(); 

  return 0;
}