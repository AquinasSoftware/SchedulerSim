#include "schedulers.h"
#define WINDOW_SIZE 3


/*************************************
 SWQ - Smart Window Queue
    Creates a window of size X and runs
    each process within in round robin fashion.
*************************************/

void SWQ(std::list<process*> &jobs){
    float numJobs = jobs.size();
    double respTimes[(short)numJobs];
    double turnTimes[(short)numJobs];
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
    std::list<process*> ioQueue;
    std::list<process*> window;
    std::thread running(ioCall, std::ref(ioQueue), std::ref(jobs), doneFlag);
    running.detach();
    std::cout << "Running " << (int)numJobs << " jobs with SWQ Scheduling" << std::endl;
    simuPrint("Running " + std::to_string((int)numJobs) + " jobs with SWQ Scheduling\n");
    time_t startTime = time(nullptr);
    while(jobs.size() > 0 || ioQueue.size() > 0 || window.size() > 0){
        while(window.size() < WINDOW_SIZE){
            if (jobs.size() > 0){
                window.push_back(jobs.front());
                jobs.pop_front();
            }
            else{
                break;
            }
        }
        if(window.size() > 0){
            if (!(window.front()->isResponded())){
                std::cout << window.front()->getID() << ": Started" << std::endl;
                simuPrint("Process " + std::to_string(window.front()->getID()) + ": Started\n");
                respTimes[window.front()->getID()] = window.front()->respond();
                respSum += respTimes[window.front()->getID()];
                respCounter++;
                respLine->AddData((float)(respCounter / numJobs) * 100, (respSum / (respCounter)), true);
            }
            switch (window.front()->run()){
                case BLOCKED:
                    ioQueue.push_back(window.front());
                    window.pop_front();
                    break;
                case DONE:
                    turnTimes[window.front()->getID()] = window.front()->turnaround();
                    turnSum += turnTimes[window.front()->getID()];
                    turnCounter++;
                    turnLine->AddData((float)(turnCounter / numJobs) * 100, (turnSum / (turnCounter)), true);
                    std::cout << window.front()->getID() << ": Done" << std::endl;
                    simuPrint("Process " + std::to_string(window.front()->getID()) + ": Done\n");
                    delete window.front();
                    window.pop_front();
                    break;
                default:
                    window.push_back(window.front());
                    window.pop_front();
                    break;
            }
        }
        std::this_thread::sleep_for(TIME_SLICE);
        updateGraph();
    }

    *doneFlag = true;
    double avgResp = std::accumulate(respTimes, respTimes + (short)numJobs, 0.0) / numJobs;
    double avgTurn = std::accumulate(turnTimes, turnTimes + (short)numJobs, 0.0) / numJobs;
    std::cout << "Completed all jobs\n\tAvg Response Time: " << avgResp << " seconds\n\tAvg Turnaround Time: " << avgTurn << " seconds" << std::endl;
    simuPrint("Completed all jobs\n\tAvg Response Time: " + std::to_string(avgResp) + " seconds\n\tAvg Turnaround Time: " + std::to_string(avgTurn) + " seconds\n");
    clearQueue();
    setupPage->Enable();
    startBtn->Enable();
    exportBtn->Enable();
}