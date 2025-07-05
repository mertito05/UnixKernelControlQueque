// minikernel.h
#ifndef MINIKERNEL_H
#define MINIKERNEL_H

void *initlibrary();
int create_task(void *(*start_routine)());

#endif // MINIKERNEL_H
   