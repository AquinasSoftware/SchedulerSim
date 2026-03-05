#include <iostream> // Temporary until GUI is made
#include <cstdlib>
#include <list>
#include <numeric>
#include <thread>
#include "../processes.h"

#define IO_TIME std::chrono::seconds(2)
#define TIME_SLICE std::chrono::seconds(1)

void ioCall(std::list<process*> &ioQueue, std::list<process*> &jobs);
void FIFO(std::list<process*> jobs);
void SJF(std::list<process*> jobs);
void roundRobin(std::list<process*> jobs);
void SWQ(std::list<process*> jobs);
void RedRobinRestaurant(std::list<process*> jobs);