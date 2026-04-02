#include "schedulers.h"

void SJF(std::list<process*> jobs){
    float numJobs = jobs.size();
    std::cout << "Running " << numJobs << " with Round SJF Scheduling" << std::endl;
    simuPrint("Running " + std::to_string(numJobs) + " with SJF Scheduling\n");
    double respTimes[(short)numJobs];
    double turnTimes[(short)numJobs];
    std::list<process*> ioQueue;
    jobs.sort([](process* a, process* b){ return a->timeLeft() < b->timeLeft(); });
    std::thread running(ioCall, std::ref(ioQueue), std::ref(jobs));
    running.detach();
    time_t startTime = time(nullptr);
    while(jobs.size() > 0 || ioQueue.size() > 0){
        short sizeMonitor = jobs.size();
        if (sizeMonitor > 0){
            if (!(jobs.front()->isResponded())){
                respTimes[jobs.front()->getID()] = jobs.front()->respond();
            }
            switch (jobs.front()->run()){
                case BLOCKED:
                    ioQueue.push_back(jobs.front());
                    jobs.pop_front();
                    break;
                case DONE:
                    turnTimes[jobs.front()->getID()] = jobs.front()->turnaround();
                    std::cout << jobs.front()->getID() << ": Done" << std::endl;
                    simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Done\n");
                    delete jobs.front();
                    jobs.pop_front();
                    break;
                default:
                    std::cout << jobs.front()->getID() << ": Running" << std::endl;
                    simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Running\n");
                    break;
            }
        }
        std::this_thread::sleep_for(TIME_SLICE);
        if (sizeMonitor < jobs.size()){
            jobs.sort([](process* a, process* b){ return a->timeLeft() < b->timeLeft(); });
        }
    }
    double avgResp = std::accumulate(respTimes, respTimes + (short)numJobs, 0.0) / numJobs;
    double avgTurn = std::accumulate(turnTimes, turnTimes + (short)numJobs, 0.0) / numJobs;
    std::cout << "Completed all jobs\n\tAvg Response Time: " << avgResp << " seconds\n\tAvg Turnaround Time: " << avgTurn << " seconds" << std::endl;
    simuPrint("Completed all jobs\n\tAvg Response Time: " + std::to_string(avgResp) + " seconds\n\tAvg Turnaround Time: " + std::to_string(avgTurn) + " seconds\n");
}
