#include "schedulers.h"

void ioCall(std::list<process*> &ioQueue, std::list<process*> &jobs, bool *done){
    while(true){
        if (ioQueue.size() > 0){
            ioQueue.front()->ioCall();
            std::cout << ioQueue.front()->getID() << ": I/O Access" << std::endl;
            simuPrint("Process " + std::to_string(ioQueue.front()->getID()) + ": I/O Access\n");
            // Safe Check if process is still in job queue to avoid racing conditions
            if (jobs.front() != ioQueue.front()){   
                jobs.push_back(ioQueue.front());
            }
            ioQueue.pop_front();
            std::this_thread::sleep_for(IO_TIME);
        }
        else{
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        if (*done == true) {
            free (done);
            break;
        }

    }
}