#include <stdio.h>
#include <regex.h>//use to parse input
#include <math.h>//use for atoi

typedef struct
{
    int waiting;
    int arrival;
    int burst;
    int waitTime;
    int turnaround;
    char name[12];//max 12 chars, otherwise it complains
} process;

void readFile()
{
    FILE* in;
    in = fopen("processes.in", "r");
    
    char c;
    while((c = fgetc(in)) != EOF)
    {
        //switch on c for various inputs
        
        printf("%c", c);
    }
    fclose(in);
}

/* Performs Shortest Job First scheduling algorithm */
void doSjf( int processCount, int runFor, process processList[] )
{
    FILE* out = fopen("processes.out","w");

    fprintf(out,"\n%d processes\n", processCount);
    fprintf(out,"Using Shortest Job First\n\n");
    //debug
    printf("\n%d processes\n", processCount);
    printf("Using Shortest Job First\n\n");

    int t=0, i, j, n=processCount;
    process* selected = NULL;
    
    //run in sjf order starting with arrival at 0
    while(t < runFor)
    {
        //main control loop
        for(i=n-1; i>=0; i--)
        {
            //check for arrivals, if arrived set waiting to true
            if(processList[i].arrival == t)
            {
                processList[i].waiting=1;
                fprintf(out,"Time %d: %s arrived\n",t, processList[i].name);
                printf("Time %d: %s arrived\n",t, processList[i].name);
            }

            //wait or select from arrived processes
            if(processList[i].waiting)
            {
                //no selected process, so select the next waiting process
                if(selected== NULL)
                 {   
                     selected = &processList[i];
                     selected->waiting=0;

                     fprintf(out,"Time %d: %s selected (burst %d)\n",t,selected->name,selected->burst);
                     printf("Time %d: %s selected (burst %d)\n",t,selected->name,selected->burst);
                 }
                //select based on burst time, if same continue with current selection
                else if(processList[i].burst < selected->burst)
                {
                    //set waiting to true, inc waitTime, and switch out the selected process
                    selected->waiting=1; selected->waitTime++;
                    selected = &processList[i];
                    selected->waiting=0;

                    fprintf(out,"Time %d: %s selected (burst %d)\n",t,selected->name,selected->burst);
                    printf("Time %d: %s selected (burst %d)\n",t,selected->name,selected->burst);
                }
                //not finished and waiting
                else if(processList[i].burst>0)
                    processList[i].waitTime++;
            }
        }

        //either do process or idle
        if(selected != NULL)
            selected->burst--;
    
        else
        {
            fprintf(out,"Time %d: IDLE\n",t);
            printf("Time %d: IDLE\n",t);
            t++;
            continue;
        }
        
        t++;//time inc here for correct finish-time logic
        
        //check for finish and if so calculate turnaround w/ arrival
        if(selected->burst == 0)
        {
            fprintf(out,"Time %d: %s finished\n",t,selected->name);
            printf("Time %d: %s finished\n",t,selected->name);
            selected->turnaround=t-selected->arrival;
            selected->waiting=0;
            selected = NULL;
        }
    }
    fprintf(out,"Finished at time %d\n\n",t);
    printf("Finished at time %d\n\n",t);
    for(i=0;i<n;i++)
    {
        fprintf(out,"%s wait %d turnaround %d\n", processList[i].name, processList[i].waitTime, processList[i].turnaround);
        printf("%s wait %d turnaround %d\n", processList[i].name, processList[i].waitTime, processList[i].turnaround);
    }

    selected=NULL;
    fclose(out);
    return;
}

int main(void)//DEBUG
{
    //dummy data for now
    int processCount = 2;
    int runFor = 15;
    int quantum = 2;//only for Round Robin
    process p1 = {.waiting=0,.arrival=3,.burst=5,.waitTime=0,.turnaround=-1,.name="P1"};
    process p2 = {.waiting=0,.arrival=0,.burst=9,.waitTime=0,.turnaround=-1,.name="P2"};
    process processList[] = {p1,p2};

    doSjf(processCount, runFor, processList);
    //readFile();//havent built parser, kinda hoping someone else did. I'm a terrible person
    return 0;
}