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
    std::thread running(ioCall, std::ref(ioQueue), std::ref(jobs));
    running.detach();
    while(jobs.size() > 0 || readyQueue.size() > 0){
        totalCycles++;
        while(readyQueue.size() < 3 && jobs.size() > 0){
            readyQueue.push_back(jobs.front());
            jobs.pop_front();
        }

        if (readyQueue.size() > 0){
            if (!(readyQueue.front()->isResponded())){
                respTimes[readyQueue.front()->getID()] = readyQueue.front()->respond();
                respSum += respTimes[readyQueue.front()->getID()];
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
                    std::cout << readyQueue.front()->getID() << ": Done" << std::endl;
                    simuPrint("Process " + std::to_string(readyQueue.front()->getID()) + ": Done\n");
                    delete(readyQueue.front());
                    break;
            }
            readyQueue.pop_front();
        }
        std::this_thread::sleep_for(TIME_SLICE);
        updateGraph();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

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