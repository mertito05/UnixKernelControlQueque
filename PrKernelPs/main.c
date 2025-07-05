// main.c
#include <stdio.h>
#include "minikernel.h" // Include the mini kernel header

void task1() {
    while (1) {
        printf("+");
        fflush(stdout);
    }
}

void task2() {
    while (1) {
        printf("-");
        fflush(stdout);
    }
}

int main() {
    initlibrary(); // Initialize the mini kernel
    create_task(task1); // Create the first task
    create_task(task2); // Create the second task

    // Main loop (can be empty, as tasks run indefinitely)
    while (1) {
        // Main thread can perform other operations if needed
    }
    return 0;
}
   