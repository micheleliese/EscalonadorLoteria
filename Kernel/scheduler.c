#include "scheduler.h"
#include <sys/time.h>

int haveId(int id);

void InsertReadyList(int id)
{
  switch(SchedulerAlgorithm)
  {
    case RR:
      if(ready_queue.queue[ready_queue.head]!=0) //coloca na fila apenas se o começo é diferente do fim
      {
    	if(ready_queue.tail<MaxNumberTask-1)
    		ready_queue.tail++;
    	else
    		ready_queue.tail=0;
      }
      ready_queue.queue[ready_queue.tail]=id;
      break;
    case LOT:
      if(ready_queue.queue[ready_queue.head]!=0) //coloca na fila apenas se o começo é diferente do fim
      {
        if(ready_queue.tail<MaxNumberTask-1)
          ready_queue.tail++;
        else
          ready_queue.tail=0;
      }
      //ready_queue.queue[ready_queue.tail]= id;
      int i= 2, valida = 0, nvezes = 0, temId = 0;
      do{
        int prioridade = rand()%10;
        nvezes++;
        if (ready_queue.tail <= NumberTaskAdd-2)
        {
          printk("\n[%d] PRIORIDADE SORTEADA PARA EXECUTAR: %d", nvezes, prioridade);
        }
        for(i = 2; i <= NumberTaskAdd; i++){
          if(Descriptors[i].Prio == prioridade && prioridade != 0){            
            temId = haveId(Descriptors[i].Tid);
            if(temId == 0){ // temId == 0: não tem id igual dentro da readylist
              //printk("\nADICIONEI A TARTEFA DE PRIORIDADE [%d] NA FILA\n", prioridade);
              ready_queue.queue[ready_queue.tail] = Descriptors[i].Tid;
              valida = 1;
              i = NumberTaskAdd;
              i++;
            }
          }
        }  
      }while(valida == 0);
      break;  
    default:
      break;
  }
}

int haveId(int id){
  int i, tem = 0;
  for(i = ready_queue.head; i <= ready_queue.tail; i++){
    if(ready_queue.queue[i] == id){
      //printk("\nACHEI UM ID IGUAL\n", 0);
      tem = 1;
      break;
    }
  }
  return tem;
}

void WakeUP()
{
  int i=2;
  for(i=2;i<=NumberTaskAdd; i++)
  {
    //sleep
    if(Descriptors[i].Time>0)
    {
      Descriptors[i].Time--;
      if(Descriptors[i].Time <= 0 && Descriptors[i].State == BLOCKED)
      {
        Descriptors[i].State = READY;
        InsertReadyList(i);
      }
    }
  }
}

void Dispatcher(void) //responsavel por realizar o disparo/restauração da tarefa
{
    
  switch(Descriptors[TaskRunning].State)
  {
    case TERMINATED:
    case BLOCKED:
    case READY:
      TaskRunning=ready_queue.queue[ready_queue.head];
      ready_queue.queue[ready_queue.head]=0;
      if(ready_queue.head!=ready_queue.tail)
      {
	    if(ready_queue.head<MaxNumberTask-1)
	    	ready_queue.head++;
	    else
	    	ready_queue.head=0;
      }
      if(Descriptors[TaskRunning].State!=READY)
	     Dispatcher();
      else
      {
#ifdef DEBUG_SYSTEM
	    TTYout();
#endif
#ifdef DEBUG_PLOT
	    TTYplot();
#endif
#ifdef SHARED_NUMBER
	    TTYshared(0);
#endif
	    T0IR = 1;
	    VICVectAddr = 0;
	    Descriptors[TaskRunning].State=RUNNING;
	    RestoreContext(Descriptors[TaskRunning].SP);
      }
      break;
    case INITIAL:
#ifdef DEBUG_SYSTEM
      TTYout();
#endif
#ifdef DEBUG_PLOT
	    TTYplot();
#endif
#ifdef SHARED_NUMBER
	    TTYshared(0);
#endif
MoveToSP(&KernelStack[299]);
      T0IR = 1;
      VICVectAddr = 0;
      Descriptors[TaskRunning].State=RUNNING;
      GoTask(Descriptors[TaskRunning].SP,Descriptors[TaskRunning].EP);
      break;
    default:
#ifdef DEBUG_SYSTEM
      TTYout();
#endif
#ifdef DEBUG_PLOT
	    TTYplot();
#endif
#ifdef SHARED_NUMBER
	    TTYshared(0);
#endif
//MoveToSP(&KernelStack[299]);
      T0IR = 1;
      VICVectAddr = 0;
      Descriptors[TaskRunning].State=RUNNING;
      RestoreContext(Descriptors[TaskRunning].SP);
      break;
  }

}

void Select(unsigned int *pilha)
{
  Descriptors[TaskRunning].SP=pilha; //salva a pilha do contexto da task
  MoveToSP(&KernelStack[289]);
  WakeUP();
  U0getchar();
  switch (SchedulerAlgorithm)
  {
    case RR:
      if(TaskRunning!=1)
	      Descriptors[TaskRunning].State=READY;
      if(TaskRunning>1)
    	  InsertReadyList(TaskRunning);
      break;
    case LOT:
      if(TaskRunning!=1)
	      Descriptors[TaskRunning].State = READY;
      if(TaskRunning > 1)
    	  InsertReadyList(TaskRunning);
      break;
    default:
      break;
  }
  Dispatcher();  
}
