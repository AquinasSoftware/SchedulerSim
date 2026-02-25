#include "schedulers.h"

/*************************************
 FIFO - First In, First Out
    Takes a list of processes and runs each one through 
    to completion, one at a time
*************************************/
void FIFO(std::list<process*> jobs){
    float numJobs = jobs.size();
    std::cout << "Running " << numJobs << " with FIFO Scheduling" << std::endl;
    time_t startTime = time(nullptr);
    while(jobs.size() > 0){
        switch(jobs.front()->run()){
            case BLOCKED:
                jobs.front()->ioCall();
                std::cout << jobs.front()->getID() << ": I/O Access" << std::endl;
                std::this_thread::sleep_for(IO_TIME);
                break;
            case DONE:
                std::cout << jobs.front()->getID() << ": Done" << std::endl;
                delete(jobs.front());
                jobs.pop_front();
                break;
            default:
                std::cout << jobs.front()->getID() << ": Running" << std::endl;
                std::this_thread::sleep_for(TIME_SLICE);
        }
    }
    double totalTime = difftime(time(nullptr), startTime);
    std::cout << "Completed all jbs in "  << totalTime << " seconds (" << numJobs/totalTime << "/s)" << std::endl;
}