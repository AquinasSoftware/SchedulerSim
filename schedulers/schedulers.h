#include <iostream> // Temporary until GUI is made
#include <cstdlib>
#include <list>
#include <future>
#include <ctime>
#include "../processes.h"

#ifdef _WIN32
    #include <windows.h>
    #define sleep(x) Sleep((x)*1000)
#else
    #include <unistd.h>
#endif

#define IO_TIME 2
#define TIME_SLICE 1

void ioCall(std::list<process*> &ioQueue, std::list<process*> &jobs);
void FIFO(std::list<process*> jobs);
void SJF(std::list<process*> jobs);
void roundRobin(std::list<process*> jobs);
void SWQ(std::list<process*> jobs);