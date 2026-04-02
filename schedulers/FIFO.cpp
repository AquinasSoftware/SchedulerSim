#include "schedulers.h"

/*************************************
 FIFO - First In, First Out
    Takes a list of processes and runs each one through 
    to completion, one at a time
*************************************/
void FIFO(std::list<process*> jobs){
    float numJobs = jobs.size();
    double respTimes[(short)numJobs];
    double turnTimes[(short)numJobs];
    std::cout << "Running " << numJobs << " with FIFO Scheduling" << std::endl;
    simuPrint("Running " + std::to_string(numJobs) + " with FIFO Scheduling\n");
    while(jobs.size() > 0){
        switch(jobs.front()->run()){
            case BLOCKED:
                jobs.front()->ioCall();
                std::cout << jobs.front()->getID() << ": I/O Access" << std::endl;
                simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": I/O Access\n");
                std::this_thread::sleep_for(IO_TIME);
                break;
            case DONE:
                turnTimes[jobs.front()->getID()] = jobs.front()->turnaround();
                std::cout << jobs.front()->getID() << ": Done" << std::endl;
                simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Done\n");
                delete(jobs.front());
                jobs.pop_front();
                break;
            default:
                if (!(jobs.front()->isResponded())){
                    respTimes[jobs.front()->getID()] = jobs.front()->respond();
                }
                std::cout << jobs.front()->getID() << ": Running" << std::endl;
                simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Running\n");
                std::this_thread::sleep_for(TIME_SLICE);
        }
    }

    double avgResp = std::accumulate(respTimes, respTimes + (short)numJobs, 0.0) / numJobs;
    double avgTurn = std::accumulate(turnTimes, turnTimes + (short)numJobs, 0.0) / numJobs;
    std::cout << "Completed all jobs\n\tAvg Response Time: " << avgResp << " seconds\n\tAvg Turnaround Time: " << avgTurn << " seconds" << std::endl;
    simuPrint("Completed all jobs\n\tAvg Response Time: " + std::to_string(avgResp) + " seconds\n\tAvg Turnaround Time: " + std::to_string(avgTurn) + " seconds\n");
}