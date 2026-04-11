#include <iostream> // Temporary until GUI is made
#include <cstdlib>
#include <list>
#include <numeric>
#include <thread>
#include "../dependencies/mathplot.h"
#include "../processes.h"
#include <wx/wx.h>

extern wxPanel* setupPage;
extern wxButton* startBtn;
extern wxButton* exportBtn;
extern mpWindow* graph;

#define IO_TIME std::chrono::nanoseconds(100)
#define TIME_SLICE std::chrono::nanoseconds(1)

void ioCall(std::list<process*> &ioQueue, std::list<process*> &jobs);
void FIFO(std::list<process*> &jobs);
void SJF(std::list<process*> &jobs);
void roundRobin(std::list<process*> &jobs);
void SWQ(std::list<process*> &jobs);
void RedRobinRestaurant(std::list<process*> &jobs);
void RedRobinBatman(std::list<process*> &jobs);

void clearQueue();
void simuPrint(const wxString& text);
void updateGraph();
