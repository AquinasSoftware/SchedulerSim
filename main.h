#include <iostream>
#include <wx/wx.h>
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

void devTest();
void addToJobs();
void removeFromJobs();
void clearQueue();