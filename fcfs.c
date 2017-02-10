#include <stdio.h>

typedef struct {
    char * name;
    int burst, arrival, wait, end;
}Program;

void fcfs(Program progs[], int runtime, int numprogs) {
    Program arrivals[numprogs];
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
                arrivals[arrival_i].wait = 0;
                fprintf(fp, "Time %d: %s arrived\n", cycle, arrivals[arrival_i].name);
                arrival_i++;
            }
        }

        //check if current process is done
        if (arrivals[running_i].burst == 0) {
            fprintf(fp, "Time %d: %s finished\n", cycle, arrivals[running_i].name);
            //complete!
            arrivals[running_i].end = cycle;
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
                    arrivals[j].wait++;
                }
            }
        }
    }

    fprintf(fp, "Finished at time %d\n\n", runtime);

    int k;
    for (k = 0; k < numprogs; k++) {
        fprintf(fp, "%s wait %d turnaround %d\n", arrivals[k].name, arrivals[k].wait, arrivals[k].end - arrivals[k].arrival);
    }

    fclose(fp);
}

//NOT WRITTEN
Program * readProgs() {
    char line[1024];
    int num;
    FILE * fp = fopen("processes.in", "r");

    fgets(line, sizeof(line), fp);
    sscanf(line, "%d", & num);

    printf("%d\n", num);
    printf("%s\n", line);
}

int main(int argc, char * argv[]) {
    //readProgs();

    Program programs[3];
    programs[0].name = "P1";
    programs[0].burst = 2;
    programs[0].arrival = 0;

    programs[1].name = "P2";
    programs[1].burst = 3;
    programs[1].arrival = 3;

    programs[2].name = "P3";
    programs[2].burst = 3;
    programs[2].arrival = 4;

    fcfs(programs, 30, 3);

    return 0;
}