/************************************************
debug usando monitor
*************************************************/
#include "debug.h"

#ifdef DEBUG_SYSTEM
void TTYout(void)
{
  unsigned short head_aux=ready_queue.head;
  printk("\n--------------------------------------------------\n");
  printk("%d:%d:%d",RTC_HOUR,RTC_MIN,RTC_SEC);
  switch(SchedulerAlgorithm)
  {
    case RR:
      printk("\nEscalonador: Round-Robin");
      break;
    case RM:
      printk("\nEscalonador: Rate-Monotonic");
      break;
    case EDF:
      printk("\nEscalonador: Earlist Deadline First");
      break;
    case LOT:
      printk("\nEscalonador: Loteria");
      break;
    default:
      printk("\nEscalonador: Desconhecido");
      break;
  }
  printk("\nNumero de Tarefa Maximo: %d | Numero de tarefas: %d ", MaxNumberTask, NumberTaskAdd);
  printk("\n|ID Task Running: %d | Nome: %s |\n",Descriptors[TaskRunning].Tid, Descriptors[TaskRunning].name);
  printk("\nReady List: \n");
  while(head_aux!=ready_queue.tail)
  {
    printk("ID TASK: %d|\n",Descriptors[ready_queue.queue[head_aux]].Tid);
    if(head_aux==MaxNumberTask-1)
      head_aux=0;
    else
      head_aux++;
  }
  printk("ID TASK DO FIM: %d |\n", Descriptors[ready_queue.queue[ready_queue.tail]].Tid);

//  printk("\nnum= %d",num);
//  printk("\nRelogio= %d",T0MR0);
}
#endif

#ifdef DEBUG_START
void TTYstart(void)
{
  int i;
  unsigned short head_aux=ready_queue.head;
  printk("\n++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printk("\nStart Executado\n");
  printk("\nInterrupcao do sistema: ");
  if(ClkT>=1)
  {
    i=ClkT;
    printk("%d Segundos",i);
  }
  else if(ClkT>=0.001)
  {
    i=ClkT*1000;
    printk("%d Milissegundos",i);
  }
  else
  {
    i=ClkT*1000000;
    printk("%d Microssegundos",i);
  }
  printk("\nAlgoritimo Selecionado: ");
  switch(SchedulerAlgorithm)
  {
    case RR:
      printk("Round-Robin");
      break;
    case LOT:
      printk("Loteria");
      break;
    default:
      printk("Desconhecido");
      break;
  }
  printk("\n\nPrioridades (Tarefa | Prioridade): ", 0);
  for(i=2;i<=NumberTaskAdd;i++)
    printk("\n[ %d ] | %d |",Descriptors[i].Tid,Descriptors[i].Prio);
  printk("\n\nReady List: \n");
  while(head_aux!=ready_queue.tail)
  {
    printk("%d | ",Descriptors[ready_queue.queue[head_aux]].Tid);
    if(head_aux==MaxNumberTask-1)
      head_aux=0;
    else
      head_aux++;
  }
  printk("%d",Descriptors[ready_queue.queue[ready_queue.tail]].Tid);

  printk("\n++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}
#endif

#ifdef SHARED_NUMBER
void TTYshared(int id)
{

	int i;
	if(id==0)
		for(i=0;i<SHARED_NUMBER;i++)
			printk("\nshared[%d] = %d --> Dispatcher",i,shared[i]);
	else
		for(i=0;i<SHARED_NUMBER;i++)
			printk("\nshared[%d] = %d --> Syscall",i,shared[i]);

}
#endif

#ifdef DEBUG_PLOT
void TTYplot(void)
{
	static unsigned int tempo=0;
	static unsigned int tempo_ini=0;
	static unsigned int ultima_TaskRunning=0;
	static int inicio=0;
	static unsigned int marc=1;
	if(inicio==0)
	{
		printk("tarefa start end\n");
		inicio=1;
	}
	if(TaskRunning!=1)
	{
		if(TaskRunning!=ultima_TaskRunning && tempo!=0)
		{
			printk("%d %s %d %d\n",marc,Descriptors[ultima_TaskRunning].name,tempo_ini,tempo);
			tempo_ini=tempo;
			ultima_TaskRunning=TaskRunning;
			marc++;
		}
		if(TaskRunning!=ultima_TaskRunning && tempo==0)
		{
			tempo_ini=tempo;
			ultima_TaskRunning=TaskRunning;
			marc++;
		}
		tempo++;
	}
}
#endif
