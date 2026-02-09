#include "schedulers.h"

void SJF(std::list<process*> jobs){
    float numJobs = jobs.size();
    std::cout << "Running " << numJobs << " with Round SJF Scheduling" << std::endl;
    std::list<process*> ioQueue;
    jobs.sort([](process* a, process* b){ return a->timeLeft() < b->timeLeft(); });
    std::future<void> ioThread = std::async(ioCall, std::ref(ioQueue), std::ref(jobs));
    time_t startTime = time(nullptr);
    while(jobs.size() > 0 || ioQueue.size() > 0){
        short sizeMonitor = jobs.size();
        if (sizeMonitor > 0){
            switch (jobs.front()->run()){
                case BLOCKED:
                    ioQueue.push_back(jobs.front());
                    jobs.pop_front();
                    break;
                case DONE:
                    std::cout << jobs.front()->getID() << ": Done" << std::endl;
                    delete jobs.front();
                    jobs.pop_front();
                    break;
                default:
                    std::cout << jobs.front()->getID() << ": Running" << std::endl;
                    break;
            }
        }
        sleep(TIME_SLICE);
        if (sizeMonitor < jobs.size()){
            jobs.sort([](process* a, process* b){ return a->timeLeft() < b->timeLeft(); });
        }
    }
    double totalTime = difftime(time(nullptr), startTime);
    std::cout << "Completed all jbs in "  << totalTime << " seconds (" << numJobs/totalTime << "/s)" << std::endl;
}
