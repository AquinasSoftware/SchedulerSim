#include "schedulers.h"

void roundRobin(std::list<process*> jobs){
    float numJobs = jobs.size();
    double respTimes[(short)numJobs];
    double turnTimes[(short)numJobs];
    std::cout << "Running " << numJobs << " with Round Robin Scheduling" << std::endl;
    simuPrint("Running " + std::to_string(numJobs) + " with Round Robin Scheduling\n");
    std::list<process*> ioQueue;
    std::thread running(ioCall, std::ref(ioQueue), std::ref(jobs));
    running.detach();
    while(jobs.size() > 0){
        while(jobs.front()->getStatus() == BLOCKED){
            jobs.push_back(jobs.front());
            jobs.pop_front();
        }
        if (!(jobs.front()->isResponded())){
            respTimes[jobs.front()->getID()] = jobs.front()->respond();
        }
        switch (jobs.front()->run()){
            case RUNNING:
                jobs.push_back(jobs.front());
                std::cout << jobs.front()->getID() << ": Running" << std::endl;
                simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Running\n");
                break;
            case BLOCKED:
                ioQueue.push_back(jobs.front());
                break;
            case DONE:
                turnTimes[jobs.front()->getID()] = jobs.front()->turnaround();
                std::cout << jobs.front()->getID() << ": Done" << std::endl;
                simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Done\n");
                delete(jobs.front());
                break;
        }
        jobs.pop_front();
        std::this_thread::sleep_for(TIME_SLICE);
    }
    double avgResp = std::accumulate(respTimes, respTimes + (short)numJobs, 0.0) / numJobs;
    double avgTurn = std::accumulate(turnTimes, turnTimes + (short)numJobs, 0.0) / numJobs;
    std::cout << "Completed all jobs\n\tAvg Response Time: " << avgResp << " seconds\n\tAvg Turnaround Time: " << avgTurn << " seconds" << std::endl;
    simuPrint("Completed all jobs\n\tAvg Response Time: " + std::to_string(avgResp) + " seconds\n\tAvg Turnaround Time: " + std::to_string(avgTurn) + " seconds\n");
}