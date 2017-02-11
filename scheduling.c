#include <stdio.h>

typedef struct
{
    int waiting;
    int arrival;
    int burst;
    int waitTime;
    int turnaround;
    char* name;
} process;

void rr(FILE* ofp, int numProcesses, int runFor, int quantum, process processes[]);

int main() {
	process p1 = {.waiting=0,.arrival=3,.burst=5,.waitTime=0,.turnaround=0,.name="P1"};
	process p2 = {.waiting=0,.arrival=0,.burst=9,.waitTime=0,.turnaround=0,.name="P2"};
	process processes[] = {p1, p2};
	
	FILE* ofp = fopen("processes.out", "w");
	
	rr(ofp, 2, 15, 2, processes);
	
	fclose(ofp);
	
	return 0;
}

void rr(FILE* ofp, int numProcesses, int runFor, int quantum, process processes[]) {
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
