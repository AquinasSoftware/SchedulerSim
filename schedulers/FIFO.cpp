#include <iomanip>
#include <chrono>
#include "schedulers.h"

/*************************************
 FIFO - First In, First Out
    Takes a list of processes and runs each one through 
    to completion, one at a time
*************************************/
void FIFO(std::list<process*> &jobs){
    //Setup
    float numJobs = jobs.size();
    double respTimes[(short)numJobs] = {0};
    double turnTimes[(short)numJobs] = {0};
    short respCounter = 0;
    short turnCounter = 0;
    float totalTime = 0;
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

    // Simulation
    std::cout << "Running " << (int)numJobs << " with FIFO Scheduling" << std::endl;
    simuPrint("Running " + std::to_string((int)numJobs) + " with FIFO Scheduling\n");
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
                totalTime = turnTimes[jobs.front()->getID()];
                turnCounter++;
                turnLine->AddData((float)(turnCounter / numJobs) * 100, (std::accumulate(turnTimes, turnTimes + turnCounter, 0.0) / (turnCounter)), true);
                std::cout << jobs.front()->getID() << ": Done" << std::endl;
                simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Done\n");
                delete(jobs.front());
                jobs.pop_front();
                break;
            default:
                if (!(jobs.front()->isResponded())){
                    std::cout << jobs.front()->getID() << ": Started" << std::endl;
                    simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Started\n");
                    respTimes[jobs.front()->getID()] = jobs.front()->respond();
                    respCounter++;
                    respLine->AddData((float)(respCounter / numJobs) * 100, (std::accumulate(respTimes, respTimes + respCounter, 0.0) / (respCounter)), true);
                }
                std::this_thread::sleep_for(TIME_SLICE);
        }
        updateGraph();
    }

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