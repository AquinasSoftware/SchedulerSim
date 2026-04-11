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
    // Delete process
    long selected = taskQueue->GetFirstSelected();
    if (selected == -1) return;
    taskQueue->DeleteItem(selected);
    std::list<process*>::iterator iter = jobs.begin();
    std::advance(iter, selected);
    delete *iter;
    jobs.erase(iter);

    // Reassign IDs
    int newID = 0;
    for (process* &proc : jobs){
        proc->setID(newID++);
    }

    // Refresh the list
    taskQueue->DeleteAllItems();
    for (process* &proc : jobs){
        taskQueue->InsertItem(taskQueue->GetItemCount(), std::to_string(proc->getID()));
        taskQueue->SetItem(taskQueue->GetItemCount() - 1, 1, proc->getType());
    }
}

void clearQueue(){
    while (jobs.size() > 0){
        if (jobs.front() != nullptr){
            delete jobs.front();
        }
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
                selectedScheduler = RRB_SCHEDULER;
                schedulerDesc->SetValue(RRB_DESC);
                simuPage->SetBackgroundColour(*wxBLACK);
                graph->SetColourTheme(*wxBLACK, *wxWHITE, *wxWHITE);
                break;
            case 5:
                selectedScheduler = DR_SCHEDULER;
                schedulerDesc->SetValue(DR_DESC);
                simuPage->SetBackgroundColour(wxColour(0x55, 0x55, 0x50));
                graph->SetColourTheme(wxColour(0x44, 0x44, 0x40), *wxLIGHT_GREY, *wxLIGHT_GREY);
                break;
            case 6:
                selectedScheduler = SWQ_SCHEDULER;
                schedulerDesc->SetValue(SWQ_DESC);
                simuPage->SetBackgroundColour(wxColour(0x69,0xAE,0xC7));
                graph->SetColourTheme(wxColour(0xE8,0xF3,0xF6), *wxBLACK, *wxBLACK);
                break;
        }
}

void startSimulation(){
    if(jobs.size() == 0){
        return;
    }
    graph->DelAllPlot(mpYesDelete);
    mpInfoLegend* legend = new mpInfoLegend(wxRect(10, 10, 600, 75));
    legend->SetItemDirection(mpHorizontal);
    graph->AddLayer(legend);
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
        case RRB_SCHEDULER:
            simuThread = std::thread(RedRobinBatman, std::ref(jobs));
            break;
        case DR_SCHEDULER:
            simuThread = std::thread(DeadRobin, std::ref(jobs));
            break;
        default:
            simuThread = std::thread(SWQ, std::ref(jobs));
            break;
    }
    startBtn->Disable();
    exportBtn->Disable();
    setupPage->Disable();
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

void updateGraph(){
    if (wxTheApp){
        wxTheApp->CallAfter([](){
            graph->UpdateAll();
            graph->Fit();
        });
    }
}

void exportGraph(){
    wxFileDialog saveFileDialog(nullptr, "Save Graph", "", "",
        "PNG files (*.png)|*.png|All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    graph->SaveScreenshot(saveFileDialog.GetPath(), wxBITMAP_TYPE_PNG, wxDefaultSize, true);
    if (wxTheApp){
        wxTheApp->CallAfter([](){
            wxMessageBox("Graph exported successfully!", "Graph Exported", wxICON_INFORMATION | wxOK);
        });
    }
}