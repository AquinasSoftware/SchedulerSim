#include <iostream>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include "descriptions.h"
#include "schedulers/schedulers.h"
#include "dependencies/mathplot.h"

extern std::list<process*> jobs;
extern wxListView* taskList;
extern wxListView* taskQueue;
extern wxListView* schedulerList;
extern wxTextCtrl* schedulerDesc;
extern wxPanel* simuPage;
extern wxButton* startBtn;
extern mpWindow* graph;
extern wxTextCtrl* simuOutput;

enum scheduler{
    FIFO_SCHEDULER,
    SJF_SCHEDULER,
    RR_SCHEDULER,
    RRR_SCHEDULER,
    SWQ_SCHEDULER
};

void devTest();
void addToJobs();
void removeFromJobs();
void clearQueue();
void selectScheduler();
void startSimulation();