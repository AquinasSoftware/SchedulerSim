#include <iomanip>
#include <chrono>
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
    std::cout << "Running " << (int)numJobs << " with Round SJF Scheduling" << std::endl;
    simuPrint("Running " + std::to_string((int)numJobs) + " with SJF Scheduling\n");
    double respTimes[(short)numJobs] = {0};
    double turnTimes[(short)numJobs] = {0};
    short respCounter = 0;
    short turnCounter = 0;
    float respSum = 0;
    float turnSum = 0;
    float totalTime = 0;
    bool *doneFlag = new bool(false);
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
    std::thread running(ioCall, std::ref(ioQueue), std::ref(jobs), doneFlag);
    running.detach();
    while(jobs.size() > 0 || ioQueue.size() > 0){
        short sizeMonitor = jobs.size();
        if (sizeMonitor > 0){
            if (!(jobs.front()->isResponded())){
                std::cout << jobs.front()->getID() << ": Started" << std::endl;
                    simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Started\n");
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
                    totalTime = turnTimes[jobs.front()->getID()];
                    turnLine->AddData((float)(turnCounter / numJobs) * 100, (turnSum / (turnCounter)), true);
                    std::cout << jobs.front()->getID() << ": Done" << std::endl;
                    simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Done\n");
                    delete jobs.front();
                    jobs.pop_front();
                    break;
                default:       
                    std::this_thread::sleep_for(TIME_SLICE);
                    break;
            }
            updateGraph();
        }
        if (sizeMonitor < jobs.size()){
            jobs.sort([](process* a, process* b){ return a->timeLeft() < b->timeLeft(); });
        }
    }

    *doneFlag = true;
    double avgResp = std::accumulate(respTimes, respTimes + (short)numJobs, 0.0) / numJobs;
    double avgTurn = std::accumulate(turnTimes, turnTimes + (short)numJobs, 0.0) / numJobs;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Completed all jobs in " << totalTime << " nanoseconds\n\tAvg Response Time: " << avgResp << " nanoseconds\n\tAvg Turnaround Time: " << avgTurn << " nanoseconds" << std::endl;
    simuPrint(wxString::Format("Completed all jobs in %.3f nanoseconds\n\tAvg Response Time: %.3f nanoseconds\n\tAvg Turnaround Time: %.3f nanoseconds\n", totalTime, avgResp, avgTurn));
    clearQueue();
    setupPage->Enable();
    startBtn->Enable();
    exportBtn->Enable();
}
