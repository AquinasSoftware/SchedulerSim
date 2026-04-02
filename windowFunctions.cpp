#include "main.h"
short selectedScheduler;

void addToJobs(){
    short id = jobs.size();
    enum procName type = static_cast<enum procName>(taskList->GetFirstSelected());
    process *newProcess = new process(type, id);
    jobs.push_back(newProcess);
    taskQueue->InsertItem(taskQueue->GetItemCount(), std::to_string(id));
    taskQueue->SetItem(taskQueue->GetItemCount() - 1, 1, newProcess->getType());
}

void removeFromJobs(){
    if (taskQueue->GetItemCount() == 0) return;
    long selected = taskQueue->GetFirstSelected();
    if (selected == -1) return;
    taskQueue->DeleteItem(selected);
    std::list<process*>::iterator iter = jobs.begin();
    std::advance(iter, selected);
    delete *iter;
    std::list<process*>::iterator iter2 = iter;;
    std::advance(iter, 1);
    while (iter != jobs.end()){
        (*iter2) = (*iter);
        (*iter2)->setID((*iter2)->getID() - 1);
        std::advance(iter2, 1);
        std::advance(iter, 1);
    }
    jobs.pop_back();

    // Refresh the list
    iter = jobs.begin();
    std::advance(iter, selected);
    for (long i = selected; i < taskQueue->GetItemCount(); i++){
        taskQueue->SetItem(i, 0, std::to_string((*iter)->getID()));
        taskQueue->SetItem(i, 1, (*iter)->getType());
    }
}

void clearQueue(){
    while (jobs.size() > 0){
        delete jobs.front();
        jobs.pop_front();
    }
    taskQueue->DeleteAllItems();
}

void selectScheduler(){
    int selected = schedulerList->GetFirstSelected();
        switch(selected){
            case 0:
                selectedScheduler = FIFO_SCHEDULER;
                schedulerDesc->SetValue(FIFO_DESC);
                simuPage->SetBackgroundColour(wxColour(0x91, 0x91, 0x91));
                graph->SetColourTheme(wxColour(0xEE,0xEE,0xEE), *wxBLACK, *wxBLACK);
                break;
            case 1:
                selectedScheduler = SJF_SCHEDULER;
                schedulerDesc->SetValue(SJF_DESC);
                simuPage->SetBackgroundColour(wxColour(0xFA, 0xEC, 0x57));
                graph->SetColourTheme(wxColour(0xFE,0xFC,0xE5), *wxBLACK, *wxBLACK);
                break;
            case 2:
                selectedScheduler = RR_SCHEDULER;
                schedulerDesc->SetValue(RR_DESC);
                simuPage->SetBackgroundColour(wxColour(0x5F, 0xFA, 0x62));
                graph->SetColourTheme(wxColour(0xE7,0xFE,0xE7), *wxBLACK, *wxBLACK);
                break;
            case 3:
                selectedScheduler = RRR_SCHEDULER;
                schedulerDesc->SetValue(RRR_DESC);
                simuPage->SetBackgroundColour(wxColour(0xED, 0x40, 0x40));
                graph->SetColourTheme(wxColour(0xFC,0xE2,0xE2), *wxBLACK, *wxBLACK);
                break;
            case 4:
                selectedScheduler = SWQ_SCHEDULER;
                schedulerDesc->SetValue(SWQ_DESC);
                simuPage->SetBackgroundColour(wxColour(0x69,0xAE,0xC7));
                graph->SetColourTheme(wxColour(0xE8,0xF3,0xF6), *wxBLACK, *wxBLACK);
                break;
        }
}

void startSimulation(){
    std::thread simuThread;
    switch(selectedScheduler){
        case FIFO_SCHEDULER:
            simuThread = std::thread(FIFO, std::ref(jobs));
            break;
        case SJF_SCHEDULER:
            simuThread = std::thread(SJF, std::ref(jobs));
            break;
        case RR_SCHEDULER:
            simuThread = std::thread(roundRobin, std::ref(jobs));
            break;
        case RRR_SCHEDULER:
            simuThread = std::thread(RedRobinRestaurant, std::ref(jobs));
            break;
        default:
            simuThread = std::thread(SWQ, std::ref(jobs));
            break;
    }
    startBtn->Disable();
    simuThread.detach();
}

void simuPrint(const wxString& text){
    if (wxTheApp){
        wxTheApp->CallAfter([text](){
            simuOutput->AppendText(text);
            simuOutput->ScrollLines(-1);
        });
    }
}