#include <stdio.h>
#define MAX 5

typedef struct process {
    char name[50];
    int arrival_time;
    int burst_time;
    int tat;
    int completion_time;
    int wait_time;
    int priority;
    int done;
} process;

// Order processes by arrival time
void order_by_arrival(process *p) {
    for (int i = 0; i < MAX - 1; i++) {
        for (int j = 0; j < MAX - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

void calculate(process *p) {
    int time = 0; // Start time at 0
    int completed = 0;
    int sum_tat = 0, sum_wait = 0;

    while (completed < MAX) {
        int idx = -1;
        int highest_priority = 99999;

        for (int i = 0; i < MAX; i++) {
            if (p[i].arrival_time <= time && !p[i].done) {
                if (p[i].priority < highest_priority) {
                    highest_priority = p[i].priority;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            // No process is ready, increment time (CPU idle)
            time++;
            continue;
        }

        // Execute the selected process
        time += p[idx].burst_time;
        p[idx].completion_time = time;
        p[idx].tat = p[idx].completion_time - p[idx].arrival_time;
        p[idx].wait_time = p[idx].tat - p[idx].burst_time;
        p[idx].done = 1;
        completed++;

        sum_tat += p[idx].tat;
        sum_wait += p[idx].wait_time;
    }

    printf("Average Turn Around Time: %.2f\n", (double)sum_tat / MAX);
    printf("Average Wait Time: %.2f\n", (double)sum_wait / MAX);
}

int main() {
    process p[MAX];

    for (int i = 0; i < MAX; i++) {
        scanf("%s", p[i].name);
        scanf("%d", &p[i].arrival_time);
        scanf("%d", &p[i].burst_time);
        scanf("%d", &p[i].priority);
        getchar(); // consume newline
        p[i].completion_time = -1;
        p[i].tat = -1;
        p[i].wait_time = -1;
        p[i].done = 0;
    }

    order_by_arrival(p);
    calculate(p);

    printf("\nFinal Process Table:\n");
    printf("%-8s %-8s %-8s %-8s %-12s %-8s %-8s\n", 
           "Name", "Arrive", "Burst", "Priority", "Complete", "TAT", "Wait");

    for (int i = 0; i < MAX; i++) {
        printf("%-8s %-8d %-8d %-8d %-12d %-8d %-8d\n",
               p[i].name, p[i].arrival_time, p[i].burst_time, p[i].priority,
               p[i].completion_time, p[i].tat, p[i].wait_time);
    }

    return 0;
}
