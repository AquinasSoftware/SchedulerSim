#include "schedulers.h"

/*
 SJF - Shortest Job First
 Processes are scheduled based on the shortest remaining time first.
 This approach minimizes the average waiting time but can lead to 
 starvation of longer processes. This is the theorietical best
 scheduler for minimizing average waiting time, but it is not 
 commonly used in practice due to the difficulty of accurately 
 predicting process run times and the potential for starvation 
 of longer processes.
*/

void SJF(std::list<process*> &jobs){
    float numJobs = jobs.size();
    std::cout << "Running " << numJobs << " with Round SJF Scheduling" << std::endl;
    simuPrint("Running " + std::to_string(numJobs) + " with SJF Scheduling\n");
    double respTimes[(short)numJobs];
    double turnTimes[(short)numJobs];
    short respCounter = 0;
    short turnCounter = 0;
    float respSum = 0;
    float turnSum = 0;
    mpFXYVector* respLine = new mpFXYVector("Response Time");
    mpFXYVector* turnLine = new mpFXYVector("Turnaround Time");
    respLine->SetPen(*wxRED);
    turnLine->SetPen(*wxGREEN);
    respLine->SetContinuity(true);
    turnLine->SetContinuity(true);
    respLine->SetSymbol(mpsCircle);
    turnLine->SetSymbol(mpsCircle);
    respLine->AddData(0, 0, true);
    turnLine->AddData(0, 0, true);
    graph->AddLayer(respLine);
    graph->AddLayer(turnLine);
    graph->Fit();
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
                respSum += respTimes[jobs.front()->getID()];
                respCounter++;
                respLine->AddData((float)(respCounter / numJobs) * 100, (respSum / (respCounter)), true);
            }
            switch (jobs.front()->run()){
                case BLOCKED:
                    ioQueue.push_back(jobs.front());
                    jobs.pop_front();
                    break;
                case DONE:
                    turnTimes[jobs.front()->getID()] = jobs.front()->turnaround();
                    turnSum += turnTimes[jobs.front()->getID()];
                    turnCounter++;
                    turnLine->AddData((float)(turnCounter / numJobs) * 100, (turnSum / (turnCounter)), true);
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
            updateGraph();
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
    clearQueue();
    setupPage->Enable();
    startBtn->Enable();
    exportBtn->Enable();
}