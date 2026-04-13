#include <iostream>
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

#define IO_TIME std::chrono::seconds(1)
#define TIME_SLICE std::chrono::milliseconds(500)

void ioCall(std::list<process*> &ioQueue, std::list<process*> &jobs, bool *doneFlag);
void FIFO(std::list<process*> &jobs);
void SJF(std::list<process*> &jobs);
void roundRobin(std::list<process*> &jobs);
void SWQ(std::list<process*> &jobs);
void RedRobinRestaurant(std::list<process*> &jobs);
void RedRobinBatman(std::list<process*> &jobs);
void DeadRobin(std::list<process*> &jobs);

void clearQueue();
void simuPrint(const wxString& text);
void updateGraph();