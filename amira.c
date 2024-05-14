#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PARTITIONS 10
#define MAX_PROCESSES 10

typedef struct {
    int size;
    int allocated;
    int process_id;
} Partition;

typedef struct {
    char name[20];
    int size;
    int id;
} Process;

typedef struct Node {
    Process process;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;

Partition partitions[MAX_PARTITIONS];
int num_partitions = 0;
Queue waiting_queue = {NULL, NULL, 0};

void initializePartitions(int sizes[], int n) {
    num_partitions = n;
    for (int i = 0; i < n; i++) {
        partitions[i].size = sizes[i];
        partitions[i].allocated = 0;
        partitions[i].process_id = -1;
    }
}

void initializeQueue(Queue* q) {
    q->front = q->rear = NULL;
    q->size = 0;
}

void enqueue(Queue* q, Process p) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->process = p;
    temp->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = temp;
    } else {
        q->rear->next = temp;
        q->rear = temp;
    }
    q->size++;
}

Process dequeue(Queue* q) {
    if (q->front == NULL) {
        Process p = {"", -1, -1};
        return p;
    }
    Node* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    Process p = temp->process;
    free(temp);
    q->size--;
    return p;
}

int firstFit(Process p) {
    for (int i = 0; i < num_partitions; i++) {
        if (!partitions[i].allocated && partitions[i].size >= p.size) {
            partitions[i].allocated = 1;
            partitions[i].process_id = p.id;
            return i;
        }
    }
    return -1;
}

int bestFit(Process p) {
    int best_index = -1;
    int best_size = 1e9;
    for (int i = 0; i < num_partitions; i++) {
        if (!partitions[i].allocated && partitions[i].size >= p.size && partitions[i].size < best_size) {
            best_index = i;
            best_size = partitions[i].size;
        }
    }
    if (best_index != -1) {
        partitions[best_index].allocated = 1;
        partitions[best_index].process_id = p.id;
    }
    return best_index;
}

void allocateMemory(Process p, const char* strategy) {
    int index;
    if (strcmp(strategy, "First Fit") == 0) {
        index = firstFit(p);
    } else if (strcmp(strategy, "Best Fit") == 0) {
        index = bestFit(p);
    } else {
        printf("Unknown strategy\n");
        return;
    }

    if (index == -1) {
        printf("No suitable partition found for process %s, adding to waiting queue\n", p.name);
        enqueue(&waiting_queue, p);
    } else {
        printf("Allocated process %s to partition %d using %s\n", p.name, index, strategy);
    }
}

void deallocateMemory(int process_id) {
    for (int i = 0; i < num_partitions; i++) {
        if (partitions[i].process_id == process_id) {
            partitions[i].allocated = 0;
            partitions[i].process_id = -1;
            printf("Deallocated memory for process %d from partition %d\n", process_id, i);

            while (waiting_queue.size > 0) {
                Process p = dequeue(&waiting_queue);
                allocateMemory(p, "First Fit");
                if (p.id == -1) break; // If allocation was successful
            }
            return;
        }
    }
    printf("Process ID %d not found in any partition\n", process_id);
}

void displayMemoryStatus() {
    printf("Memory Status:\n");
    for (int i = 0; i < num_partitions; i++) {
        printf("Partition %d: size %d, allocated %d, process ID %d\n", i, partitions[i].size, partitions[i].allocated, partitions[i].process_id);
    }
}

void computeFragmentation() {
    int internal_fragmentation = 0;
    int external_fragmentation = 0;
    int free_blocks = 0;

    for (int i = 0; i < num_partitions; i++) {
        if (partitions[i].allocated) {
            internal_fragmentation += (partitions[i].size - partitions[i].process_id); // Assuming process size is stored in process_id temporarily
        } else {
            free_blocks += partitions[i].size;
        }
    }
    external_fragmentation = free_blocks;

    printf("Internal Fragmentation: %d\n", internal_fragmentation);
    printf("External Fragmentation: %d\n", external_fragmentation);
}

int main() {
    /*int partition_sizes[] = {100, 500, 200, 300, 600};
    initializePartitions(partition_sizes, 5);

    Process p1 = {"Process1", 212, 1};
    Process p2 = {"Process2", 417, 2};
    Process p3 = {"Process3", 112, 3};
    Process p4 = {"Process4", 426, 4};

    allocateMemory(p1, "First Fit");
    allocateMemory(p2, "Best Fit");
    allocateMemory(p3, "First Fit");
    allocateMemory(p4, "Best Fit");

    displayMemoryStatus();

    deallocateMemory(2);
    deallocateMemory(3);

    displayMemoryStatus();

    computeFragmentation();*/

    int choice;
    do{
        printf("Menu: \n--------------------------------\n");
        printf("1. First Fit\n");
        printf("2. Best Fit\n");
        printf("3. Display Memory Status \n");
        printf("4. Compute Fragmentation\n");
        printf("5. Deallocate Memory\n");
        printf("6. Exit\n");
        printf("Enter your choice : \n");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
                allocateMemory(process,"First Fit");
                break;
            case 2:
                allocateMemory(process, "Best Fit");
                break;
            case 3:
                displayMemoryStatus();
                break;
            case 4:
                computeFragmentation();
                break;
            case 5:
                deallocateMemory();
                break;
            case 6:
                exit(0);
                break;
            default:
                printf("Error !! Wrong choice. \n");
        }
    }while(1);

    return 0;
}

