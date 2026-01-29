#include <iostream> // Temporary until GUI is made
#include <cstdlib>
#include <list>
#include "../processes.h"

#ifdef _WIN32
    #include <windows.h>
    #define usleep(x) Sleep((x)/1000)
#else
    #include <unistd.h>
#endif

#define IO_TIME 2
#define TIME_SLICE 1

void FIFO(std::list<process*> jobs);