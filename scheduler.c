/**
    COP4600 - Group 31
    Programming Assignment 1
    Christopher Williams
    Kevin Shoults
    Mark McCulloh 
**/
#include <stdio.h>
#include <stdlib.h>//malloc
#include <string.h>//use to parse input
#include <math.h>//use for atoi

typedef struct
{
    int waiting;
    int arrival;
    int burst;
    int waitTime;
    int turnaround;
    char name[32];
} process;

/**************
 Performs First Come First Serve scheduling algorithm 
 **************/
void fcfs(process progs[], int runtime, int numprogs) {
    process arrivals[numprogs];
    int running_i = 0;
    int cycle = 0;
    int selectnext = 1;
    int arrival_i = 0;

    FILE * fp = fopen("processes.out", "w+");

    fprintf(fp, "%d processes\nUsing First Come First Served\n\n", numprogs);

    for (cycle = 0; cycle <= runtime; cycle++) {
        //Check for arrivals
        int i;
        for (i = 0; i < numprogs; i++) {
            if (progs[i].arrival == cycle) {
                arrivals[arrival_i] = progs[i];
                arrivals[arrival_i].waitTime = 0;
                fprintf(fp, "Time %d: %s arrived\n", cycle, arrivals[arrival_i].name);
                arrival_i++;
            }
        }

        //check if current process is done
        if (arrivals[running_i].burst == 0) {
            fprintf(fp, "Time %d: %s finished\n", cycle, arrivals[running_i].name);
            //complete!
            arrivals[running_i].turnaround = cycle;
            running_i++;
            selectnext = 1;
        }

        //select next process if needed
        if (selectnext == 1) {
            //next program available?
            if (running_i < arrival_i) {
                selectnext = 0;
                fprintf(fp, "Time %d: %s selected (burst %d)\n", cycle, arrivals[running_i].name, arrivals[running_i].burst);
            } else {
                fprintf(fp, "Time %d: Idle\n", cycle);
            }
        }

        //run or wait all arrivals
        int j;
        for (j = 0; j < arrival_i; j++) {
            if (arrivals[j].burst > 0) { //only care if it's incomplete
                if (j == running_i) {
                    arrivals[j].burst--;
                } else {
                    arrivals[j].waitTime++;
                }
            }
        }
    }

    fprintf(fp, "Finished at time %d\n\n", runtime);

    int k;
    for (k = 0; k < numprogs; k++) {
        fprintf(fp, "%s wait %d turnaround %d\n", arrivals[k].name, arrivals[k].waitTime, arrivals[k].turnaround - arrivals[k].arrival);
    }

    fclose(fp);
}

/**************
 Performs Shortest Job First scheduling algorithm 
 **************/
void sjf( int processCount, int runFor, process processList[] )
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
                if(selected == NULL)
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
    }//TODO: handle unfinished processes

    selected=NULL;
    fclose(out);
    return;
}

/**************
 Performs Round Robin scheduling algorithm 
 **************/
void rr(int numProcesses, int runFor, int quantum, process processes[]) {
	FILE* ofp = fopen("processes.out","w");
    int i, j, finishedIdx = 0, qIdx = 0, qEnd = 0, idle = 1, arrivalIdx, quantumCounter = quantum;
	process finishedProcs[numProcesses];
	
	for(i=0; i < numProcesses-1; i++) {
		for(j=0; j < numProcesses-i-1; j++) {
			if(processes[j].arrival > processes[j+1].arrival) {
				process tmp = processes[j];
				processes[j] = processes[j+1];
				processes[j+1] = tmp;
			}
		}
	}
	
	i = 0;
	
	fprintf(ofp, "%d processes\n", numProcesses);
	fprintf(ofp, "Using Round-Robin\n");
	fprintf(ofp, "Quantum %d\n\n", quantum);
	
	while(i < runFor) {
		// Load the arriving process into the queue
		if(qEnd < numProcesses && i == processes[qEnd].arrival) {
			processes[qEnd].waiting = 1;
			fprintf(ofp, "Time %d: %s arrived\n", i, processes[qEnd].name);
			qEnd++;
		}
		
		// Run the next process if the CPU is idle and the queue is not empty
		if(idle == 1 && qEnd > 0) {
			idle = 0;
			quantumCounter = quantum;
			processes[qIdx].waiting = 0;
			fprintf(ofp, "Time %d: %s selected (burst %d)\n", i, processes[qIdx].name, processes[qIdx].burst);
		}
		
		quantumCounter --;
		
		// Increment turnaround for all processes in the queue and waitTime for waiting processes
		for(j=0; j < qEnd; j++) {
			if(processes[j].waiting == 1) {
				processes[j].waitTime++;
			}
			
			processes[j].turnaround++;
		}
		
		// Decrement burst for the running process
		if(idle == 0) {
			processes[qIdx].burst--;
		}
		
		// Nothing happened this cycle
		if(idle == 1) {
			fprintf(ofp, "Time %d: IDLE\n", i);
		}
		
		i++;
		
		if(qEnd > 0 && quantumCounter == 0) {
			idle = 1;
			processes[qIdx].waiting = 1;
			
			qIdx++;
			
			if(qIdx >= qEnd) {
				qIdx = 0;
			}
		}
		
		// The current process has finished
		if(qEnd > 0 && processes[qIdx].burst == 0) {
			fprintf(ofp, "Time %d: %s finished\n", i, processes[qIdx].name);
			
			idle = 1;
			
			finishedProcs[finishedIdx] = processes[qIdx];
			finishedIdx++;
			
			// Remove the finished process and move everything behind it
			// up one position in the queue
			for(j = qIdx; j < qEnd-1; j++) {
				processes[j] = processes[j+1];
			}
			
			qIdx++;
			qEnd--;
			
			if(qIdx >= qEnd) {
				qIdx = 0;
			}
		}
	}
	
	fprintf(ofp, "Finished at time %d\n\n", runFor);
	
	for(i=0; i < finishedIdx; i++) {
		fprintf(ofp, "%s wait %d turnaround %d\n", finishedProcs[i].name, finishedProcs[i].waitTime, finishedProcs[i].turnaround);
	}
	
	return;
}

int main(void)
{
    int processCount, runFor, quantum, index=0;
    char use[32]="";
    process* processList = (process*)malloc(1*sizeof(process));

    //File Parsing
    char str[100]; 
    char p[32], temp[32], temp2[32], temp3[32];
    FILE *in;
    in = fopen("processes.in", "r");
    while(!feof(in))
    {
        fgets(str, 100, in);
        sscanf(str,"%s",p);
        if(strcmp(p,"processcount")==0)
        {
           sscanf(str,"%*s%s",temp);
           processCount = atoi(temp);
           processList = (process*) realloc(processList, processCount*sizeof(process));
        }
        else if(strcmp(p,"runfor")==0)
        {
           sscanf(str,"%*s%s",temp);
           runFor = atoi(temp);
        }
        else if(strcmp(p,"use")==0)
        {
           sscanf(str,"%*s%s",temp);
           strcpy(use,temp);
        }
        else if(strcmp(p,"quantum")==0)
        {
            if(strcmp(use,"rr")!=0)
                quantum=0;
           else
           {
                sscanf(str,"%*s%s",temp);
                quantum = atoi(temp);
           }
        }
        else if(strcmp(p,"process")==0)
        {
           sscanf(str,"%*s%*s%s%*s%s%*s%s",temp,temp2,temp3);
           strcpy(processList[index].name, temp);
           processList[index].arrival = atoi(temp2);
           processList[index].burst = atoi(temp3);
           index++;
        }
     }
    fclose(in);
    if(strcmp(use,"fcfs")==0)
        fcfs(processList, runFor, processCount);
    else if(strcmp(use,"sjf")==0)
        sjf(processCount, runFor, processList);
    else if(strcmp(use,"rr")==0)
        rr(processCount, runFor, quantum, processList);
    free(processList);
    return 0;
}