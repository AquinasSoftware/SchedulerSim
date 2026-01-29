#include "schedulers.h"

/*************************************
 FIFO - First In, First Out
    Takes a list of processes and runs each one through 
    to completion, one at a time
*************************************/
void FIFO(std::list<process*> jobs){
    std::cout << "Running " << jobs.size() << " with FIFO Scheduling" << std::endl;
    while(jobs.size() > 0){
        switch(jobs.front()->run()){
            case BLOCKED:
                jobs.front()->ioCall();
                std::cout << jobs.front()->getID() << ": I/O Access" << std::endl;
                sleep(IO_TIME);
                break;
            case DONE:
                std::cout << jobs.front()->getID() << ": Done" << std::endl;
                delete(jobs.front());
                jobs.pop_front();
                break;
            default:
                std::cout << jobs.front()->getID() << ": Running" << std::endl;
                sleep(TIME_SLICE);
        }
    }
}