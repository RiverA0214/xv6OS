#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// PROGRAMA DE PRUEBA: Planificador por Prioridades
// Objetivo: Demostrar que el scheduler favorece a los procesos con mayor prioridad.
// Estrategia: Crear dos procesos idénticos que compitan por la CPU (CPU-bound)
// y asignarles prioridades extremas (100 vs 0).

int
main(int argc, char *argv[])
{
  int pid1, pid2;

  printf("Iniciando prueba de prioridades...\n");

  // --- CREACIÓN DEL PRIMER COMPETIDOR (EL FAVORITO) ---
  pid1 = fork();
  if(pid1 == 0){
    // Estamos en el proceso Hijo 1
    
    // Usamos la nueva System Call para darnos la máxima prioridad posible
    set_priority(100); 
    printf("Proceso Hijo 1 (Alta Prio) iniciado\n");
    
    // Generamos carga de trabajo artificial (CPU Bound).
    // Este bucle no hace nada útil, solo gasta ciclos de reloj para
    // obligar al scheduler a mantenernos en ejecución.
    for(int i = 0; i < 10000000; i++){
       if(i % 1000000 == 0) printf("Hijo 1 trabajando...\n");
    }
    
    printf(">>> HIJO 1 TERMINO (Ganador esperado)\n");
    exit(0); // Termina el Hijo 1
  }

  // --- CREACIÓN DEL SEGUNDO COMPETIDOR (EL DESFAVORECIDO) ---
  pid2 = fork();
  if(pid2 == 0){
    // Estamos en el proceso Hijo 2
    
    // Le asignamos la prioridad mínima.
    // Según nuestro algoritmo, este proceso solo debería correr cuando
    // el Hijo 1 no esté usando la CPU (o haya terminado).
    set_priority(0); 
    printf("Proceso Hijo 2 (Baja Prio) iniciado\n");
    
    // Misma carga de trabajo exacta que el Hijo 1
    for(int i = 0; i < 10000000; i++){
       if(i % 1000000 == 0) printf("Hijo 2 trabajando...\n");
    }
    
    printf(">>> HIJO 2 TERMINO (Perdedor esperado)\n");
    exit(0); // Termina el Hijo 2
  }

  // --- EL JUEZ (PROCESO PADRE) ---
  // El padre espera a que ambos hijos terminen para no volver al shell antes de tiempo.
  wait(0);
  wait(0);
  
  printf("Prueba terminada.\n");
  exit(0);
}