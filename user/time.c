#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int start, end;
  int pid;

  if(argc < 2){
    fprintf(2, "Usage: time command [args...]\n");
    exit(1);
  }

  start = uptime();
  
  pid = fork();
  if(pid == 0){
    // Proceso hijo: ejecutar el comando
    exec(argv[1], argv + 1);
    fprintf(2, "time: exec %s failed\n", argv[1]);
    exit(1);
  }
  
  // Proceso padre: esperar y medir
  wait(0);
  end = uptime();
  
  printf("Time: %d ticks\n", end - start);
  
  exit(0);
}