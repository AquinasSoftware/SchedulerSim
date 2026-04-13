#include <iomanip>
#include <chrono>
#include "schedulers.h"

/* 
RR - Round Robin
Processes are put in a rotating queue and given a fixed time slice 
to run. If a process doesn't finish within its time slice, it's 
moved to the back of the queue. This approach is simple and fair, 
but can lead to longer turnaround times for short processes if the
time slice is too large, or increased overhead if the time slice is 
too small.
*/

void roundRobin(std::list<process*> &jobs){
    float numJobs = jobs.size();
    double respTimes[(short)numJobs] = {0};
    double turnTimes[(short)numJobs] = {0};
    short respCounter = 0;
    short turnCounter = 0;
    float respSum = 0;
    float turnSum = 0;
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

    std::cout << "Running " << (int)numJobs << " with Round Robin Scheduling" << std::endl;
    simuPrint("Running " + std::to_string((int)numJobs) + " with Round Robin Scheduling\n");
    std::list<process*> ioQueue;
    std::thread running(ioCall, std::ref(ioQueue), std::ref(jobs), doneFlag);
    running.detach();
    while(jobs.size() > 0 || ioQueue.size() > 0){
        if (jobs.size() > 0){
            while(jobs.front()->getStatus() == BLOCKED){
                jobs.push_back(jobs.front());
                jobs.pop_front();
            }
        
            if (!(jobs.front()->isResponded())){
                std::cout << jobs.front()->getID() << ": Started" << std::endl;
                simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Started\n");
                respTimes[jobs.front()->getID()] = jobs.front()->respond();
                respSum += respTimes[jobs.front()->getID()];
                respCounter++;
                respLine->AddData((float)(respCounter / numJobs) * 100, (respSum / (respCounter)), true);
            }
            switch (jobs.front()->run()){
                case RUNNING:
                    std::this_thread::sleep_for(TIME_SLICE);
                    jobs.push_back(jobs.front());
                    break;
                case BLOCKED:
                    ioQueue.push_back(jobs.front());
                    break;
                case DONE:
                    turnTimes[jobs.front()->getID()] = jobs.front()->turnaround();
                    turnSum += turnTimes[jobs.front()->getID()];
                    turnCounter++;
                    turnLine->AddData((float)(turnCounter / numJobs) * 100, (turnSum / (turnCounter)), true);
                    std::cout << jobs.front()->getID() << ": Done" << std::endl;
                    simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Done\n");
                    delete(jobs.front());
                    break;
            }
            jobs.pop_front();
            updateGraph();
        }
        else{
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    updateGraph();
    *doneFlag = true;
    double avgResp = std::accumulate(respTimes, respTimes + (short)numJobs, 0.0) / numJobs;
    double avgTurn = std::accumulate(turnTimes, turnTimes + (short)numJobs, 0.0) / numJobs;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Completed all jobs\n\tAvg Response Time: " << avgResp << " nanoseconds\n\tAvg Turnaround Time: " << avgTurn << " nanoseconds" << std::endl;
    std::cout << "Total Cycles: " << totalCycles << "\nTotal Time: " << totalTime << " nanoseconds" << std::endl;
    simuPrint(wxString::Format("Completed all jobs\n\tAvg Response Time: %.3f nanoseconds\n\tAvg Turnaround Time: %.3f nanoseconds\n", avgResp, avgTurn));
    simuPrint("Total Cycles: " + std::to_string(totalCycles) + "\nTotal Time: " + std::to_string(totalTime) + " nanoseconds\n");
    clearQueue();
    setupPage->Enable();
    startBtn->Enable();
    exportBtn->Enable();
}
