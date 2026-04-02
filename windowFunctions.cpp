#include "main.h"

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