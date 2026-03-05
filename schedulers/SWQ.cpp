#include "schedulers.h"
#define WINDOW_SIZE 3


/*************************************
 SWQ - Smart Window Queue
    Creates a window of size X and runs
    each process within in round robin fashion.
*************************************/

void SWQ(std::list<process*> jobs){
    float numJobs = jobs.size();
    double respTimes[(short)numJobs];
    double turnTimes[(short)numJobs];
    std::list<process*> ioQueue;
    std::list<process*> window;
    std::thread running(ioCall, std::ref(ioQueue), std::ref(jobs));
    running.detach();
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
            if (!(window.front()->isResponded())){
                respTimes[window.front()->getID()] = window.front()->respond();
            }
            switch (window.front()->run()){
                case BLOCKED:
                    ioQueue.push_back(window.front());
                    window.pop_front();
                    break;
                case DONE:
                    turnTimes[window.front()->getID()] = window.front()->turnaround();
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
            std::this_thread::sleep_for(TIME_SLICE);
        }
    }
    double avgResp = std::accumulate(respTimes, respTimes + (short)numJobs, 0.0) / numJobs;
    double avgTurn = std::accumulate(turnTimes, turnTimes + (short)numJobs, 0.0) / numJobs;
    std::cout << "Completed all jobs\n\tAvg Response Time: " << avgResp << " seconds\n\tAvg Turnaround Time: " << avgTurn << " seconds" << std::endl;
}