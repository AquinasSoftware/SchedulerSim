#include "schedulers.h"

void ioCall(std::list<process*> &ioQueue, std::list<process*> &jobs){
    while(jobs.size() > 0 || ioQueue.size() > 0){
        if (ioQueue.size() > 0){
            ioQueue.front()->ioCall();
            std::cout << ioQueue.front()->getID() << ": I/O Access" << std::endl;
            jobs.push_back(ioQueue.front());
            ioQueue.pop_front();
            sleep(IO_TIME);
        }
    }
}