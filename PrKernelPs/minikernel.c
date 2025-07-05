// minikernel.c
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>

#define STACK_SIZE 1024 * 64 // 64 KB stack size

typedef struct TaskControlBlock {
    ucontext_t context; // Task context
    struct TaskControlBlock *next; // Next task in the queue
} TCB;

TCB *ready_queue = NULL; // Head of the ready queue
TCB *current_task = NULL; // Currently running task

void scheduler(int signum) {
    // Switch context to the next task in the ready queue
    if (current_task && current_task->next) {
        current_task = current_task->next;
    } else {
        current_task = ready_queue; // Loop back to the first task
    }
    setcontext(&current_task->context);
}

void setup_timer() {
    struct sigaction sa;
    struct itimerval timer;

    sa.sa_handler = &scheduler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 10000; // 10ms
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 10000; // 10ms
    setitimer(ITIMER_REAL, &timer, NULL);
}

void *initlibrary() {
    ready_queue = NULL;
    setup_timer();
}

int create_task(void *(*start_routine)()) {
    TCB *new_task = malloc(sizeof(TCB));
    if (new_task == NULL) return 1; // Allocation error

    getcontext(&new_task->context);
    new_task->context.uc_stack.ss_sp = malloc(STACK_SIZE);
    new_task->context.uc_stack.ss_size = STACK_SIZE;
    new_task->context.uc_link = 0;
    makecontext(&new_task->context, start_routine, 0);

    // Add the new task to the ready queue
    new_task->next = ready_queue;
    ready_queue = new_task;

    return 0; // Success
}
   