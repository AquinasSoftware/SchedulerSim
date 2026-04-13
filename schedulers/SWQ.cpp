#include <iomanip>
#include <chrono>
#include "schedulers.h"

/*************************************
 SWQ - Smart Window Queue
    Processes are put in a linear queue in the order they were received. 
    The first 3 are then executed in a rotation until one is complete or 
    issues an I/O instruction. 
*************************************/
void SWQ(std::list<process*> &jobs){
    //Setup
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

    long long totalCycles = 0;
    auto start = std::chrono::high_resolution_clock::now();

    // Simulation
    std::cout << "Running " << numJobs << " with Smart Window Queue Scheduling" << std::endl;
    simuPrint("Running " + std::to_string(numJobs) + " with SWQ Scheduling\n");
    std::list<process*> readyQueue;
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
            switch (readyQueue.front()->run()){
                case RUNNING:
                    readyQueue.push_back(readyQueue.front());
                    std::cout << readyQueue.front()->getID() << ": Running" << std::endl;
                    simuPrint("Process " + std::to_string(readyQueue.front()->getID()) + ": Running\n");
                    break;
                case BLOCKED:
                    ioQueue.push_back(readyQueue.front());
                    break;
                case DONE:
                    turnTimes[readyQueue.front()->getID()] = readyQueue.front()->turnaround();
                    turnSum += turnTimes[readyQueue.front()->getID()];
                    turnCounter++;
                    turnLine->AddData((float)(turnCounter / numJobs) * 100, (turnSum / (turnCounter)), true);
                    std::cout << window.front()->getID() << ": Done" << std::endl;
                    simuPrint("Process " + std::to_string(window.front()->getID()) + ": Done\n");
                    delete window.front();
                    window.pop_front();
                    break;
                default:
                    std::this_thread::sleep_for(TIME_SLICE);
                    window.push_back(window.front());
                    window.pop_front();
                    break;
            }
        }
        updateGraph();
    }

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
