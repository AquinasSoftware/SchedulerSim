#include "schedulers.h"

void roundRobin(std::list<process*> jobs){
    float numJobs = jobs.size();
    std::cout << "Running " << numJobs << " with Round Robin Scheduling" << std::endl;
    std::list<process*> ioQueue;
    std::future<void> running = std::async(std::launch::async, ioCall, std::ref(ioQueue), std::ref(jobs));
    time_t startTime = time(nullptr);
    while(jobs.size() > 0){
        while(jobs.front()->getStatus() == BLOCKED){
            jobs.push_back(jobs.front());
            jobs.pop_front();
        }
        switch (jobs.front()->run()){
            case RUNNING:
                jobs.push_back(jobs.front());
                std::cout << jobs.front()->getID() << ": Running" << std::endl;
                break;
            case BLOCKED:
                ioQueue.push_back(jobs.front());
                break;
            case DONE:
                std::cout << jobs.front()->getID() << ": Done" << std::endl;
                delete(jobs.front());
                break;
        }
        jobs.pop_front();
        sleep(TIME_SLICE);
    }
    double totalTime = difftime(time(nullptr), startTime);
    std::cout << "Completed all jbs in "  << totalTime << " seconds (" << numJobs/totalTime << "/s)" << std::endl;
}