#include <iomanip>
#include "schedulers.h"

void roundRobin(std::list<process*> &jobs){
    float numJobs = jobs.size();
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
            respSum += respTimes[jobs.front()->getID()];
            respCounter++;
            respLine->AddData((float)(respCounter / numJobs) * 100, (respSum / (respCounter)), true);
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
                turnSum += turnTimes[jobs.front()->getID()];
                turnCounter++;
                turnLine->AddData((float)(turnCounter / numJobs) * 100, (turnSum / (turnCounter)), true);
                std::cout << jobs.front()->getID() << ": Done" << std::endl;
                simuPrint("Process " + std::to_string(jobs.front()->getID()) + ": Done\n");
                delete(jobs.front());
                break;
        }
        jobs.pop_front();
        std::this_thread::sleep_for(TIME_SLICE);
        updateGraph();
    }
    double avgResp = std::accumulate(respTimes, respTimes + (short)numJobs, 0.0) / numJobs;
    double avgTurn = std::accumulate(turnTimes, turnTimes + (short)numJobs, 0.0) / numJobs;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Completed all jobs\n\tAvg Response Time: " << avgResp << " nanoseconds\n\tAvg Turnaround Time: " << avgTurn << " nanoseconds" << std::endl;
    simuPrint(wxString::Format("Completed all jobs\n\tAvg Response Time: %.3f nanoseconds\n\tAvg Turnaround Time: %.3f nanoseconds\n", avgResp, avgTurn));
    clearQueue();
    setupPage->Enable();
    startBtn->Enable();
    exportBtn->Enable();
}