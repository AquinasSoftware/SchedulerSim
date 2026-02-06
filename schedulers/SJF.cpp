#include "schedulers.h"

void SJF(std::list<process*> jobs){
    std::list<process*> ioQueue;
    jobs.sort([](process* a, process* b){ return a->timeLeft() < b->timeLeft(); });
    std::future<void> ioThread = std::async(ioCall, std::ref(ioQueue), std::ref(jobs));
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
}
