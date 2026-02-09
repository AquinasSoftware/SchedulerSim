#include "schedulers.h"
#define WINDOW_SIZE 3


/*************************************
 SWQ - Smart Window Queue
    Creates a window of size X and runs
    each process within in round robin fashion.
*************************************/

void SWQ(std::list<process*> jobs){
    float numJobs = jobs.size();
    std::list<process*> ioQueue;
    std::list<process*> window;
    std::future<void> ioThread = std::async(ioCall, std::ref(ioQueue), std::ref(jobs));
    std::cout << "Running " << numJobs << " with SWQ Scheduling" << std::endl;
    time_t startTime = time(nullptr);
    while(jobs.size() > 0 || ioQueue.size() > 0 || window.size() > 0){
        while(window.size() < WINDOW_SIZE){
            if (jobs.size() > 0){
                window.push_back(jobs.front());
                jobs.pop_front();
            }
            else{
                break;
            }
        }
        if(window.size() > 0){
            switch (window.front()->run()){
                case BLOCKED:
                    ioQueue.push_back(window.front());
                    window.pop_front();
                    break;
                case DONE:
                    std::cout << window.front()->getID() << ": Done" << std::endl;
                    delete window.front();
                    window.pop_front();
                    break;
                default:
                    std::cout << window.front()->getID() << ": Running" << std::endl;
                    window.push_back(window.front());
                    window.pop_front();
                    break;
            }
            sleep(TIME_SLICE);
        }
    }
    double totalTime = difftime(time(nullptr), startTime);
    std::cout << "Completed all jbs in "  << totalTime << " seconds (" << numJobs/totalTime << "/s)" << std::endl;
}