#include "../Kernel/kernel.h"

void taskMarota1()
{
    while(1){
        nkprint("\ntask 1", 0);
    }
    taskexit();
}

void taskMarota2()
{
    while(1){
        nkprint("\ntask 2", 0);
    }
    taskexit();
}

void taskMarota3()
{
    while(1){
        nkprint("\ntask 3", 0);
    }
    taskexit();
}

void taskMarota4()
{
    while(1){
        nkprint("\ntask 4", 0);
    }
    taskexit();
}

void taskMarota5()
{
    while(1){
        nkprint("\ntask 5", 0);
    }
    taskexit();
}

int main (int argc, char *argv[])
{
    int t1, t2, t3, t4, t5;
    taskcreate(&t1, taskMarota1);
    taskcreate(&t2, taskMarota2);
    taskcreate(&t3, taskMarota3);
    taskcreate(&t4, taskMarota4);
    taskcreate(&t5, taskMarota5);
    start(LOT);
    return 0;
}
