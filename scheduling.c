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
	process p1;
	process p2;
	p1.waiting = 0;
	p2.waiting = 0;
	p1.arrival = 3;
	p2.arrival = 0;
	p1.burst = 5;
	p2.burst = 9;
	p1.waitTime = 0;
	p2.waitTime = 0;
	p1.turnaround = 0;
	p2.turnaround = 0;
	p1.name = "P1";
	p2.name = "P2";
	process processes[2] = {p1, p2};
	
	FILE* ofp = fopen("processes.out", "w");
	
	rr(ofp, 2, 15, 2, processes);
	
	close(ofp);
	
	return 0;
}

void rr(FILE* ofp, int numProcesses, int runFor, int quantum, process processes[]) {
	int i, j, finishedIdx = 0, qIdx = 0, qEnd = 0, idle = 1, arrivalIdx, quantumCounter = quantum;
	process finishedProcs[numProcesses];
	
	for(i=0; i < numProcesses; i++) {
		for(j=0; j < numProcesses-1; j++) {
			if(processes[i].arrival > processes[i+1].arrival) {
				process tmp = processes[i+1];
				processes[i+1] = processes[i];
				processes[i] = tmp;
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
		if(idle && qEnd > 0) {
			idle = 0;
			processes[qIdx].waiting = 0;
			quantumCounter--;
			fprintf(ofp, "Time %d: %s selected (burst %d)\n", i, processes[qIdx].name, processes[qIdx].burst);
		}
		
		// Increment waitTime on all waiting processes
		for(j=0; j < numProcesses; j++) {
			if(processes[j].waiting) {
				processes[j].waitTime++;
			}
		}
		
		// Increment turnaround for all processes in the queue
		for(j=0; j < qEnd; j++) {
			processes[j].turnaround++;
		}
		
		// Decrement burst for the running process
		if(!idle) {
			processes[qIdx].burst--;
		}
		
		// Nothing happened this cycle
		if(idle) {
			fprintf(ofp, "Time %d: Idle\n", i);
		}
		
		i++;
		
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
}
