/* Red Robin

*** Red Robin Restaurant ***
Red Robin (restaurant) is a scheduling algorithm based on a restaurant. 
RRR aims to please ALL the customers (processes) but also serve them in
order of arrival. RRR is a mix of FIFO and Round Robin.

** Red Robin Batman ***

*/
#include "schedulers.h"

void RedRobinRestaurant(std::list<process*> &jobs){

  int numjobs = jobs.size();
  double respTimes[(short)numjobs];
  double turnTimes[(short)numjobs];
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

  std::cout << "Running " << numjobs << " with Red Robin (Restaurant) Scheduling" << std::endl;
  simuPrint("Running " + std::to_string(numjobs) + " with Red Robin (Restaurant) Scheduling\n");
  time_t starttime = time(nullptr);

  const int maxCycles = 2;
  int firstProcCycles = 0;

  process* firstProc = jobs.front();

  while(jobs.size() > 0){

    if (!(jobs.front()->isResponded())){
        respTimes[jobs.front()->getID()] = jobs.front()->respond();
        respSum += respTimes[jobs.front()->getID()];
        respCounter++;
        respLine->AddData((float)(respCounter / numjobs) * 100, (respSum / (respCounter)), true);
    }

    switch (jobs.front()->run())
    {
      case RUNNING:
        std::cout << "Running Job " << jobs.front()->getID() << "." << std::endl;
        simuPrint("Running Job " + std::to_string(jobs.front()->getID()) + ".\n");
        std::this_thread::sleep_for(TIME_SLICE);
        break;
      case BLOCKED:
        jobs.front()->ioCall();
        std::cout << "Job " << jobs.front()->getID() << " IO Call" << std::endl;
        simuPrint("Job " + std::to_string(jobs.front()->getID()) + " IO Call\n");
        std::this_thread::sleep_for(IO_TIME);
        break;
     case DONE:
        turnTimes[jobs.front()->getID()] = jobs.front()->turnaround();
        turnSum += turnTimes[jobs.front()->getID()];
        turnCounter++;
        turnLine->AddData((float)(turnCounter / numjobs) * 100, (turnSum / (turnCounter)), true);
        std::cout << "Completed Job " << jobs.front()->getID() << "." << std::endl;
        simuPrint("Completed Job " + std::to_string(jobs.front()->getID()) + ".\n");
        jobs.pop_front();
        if (jobs.size() > 0){
          firstProc = jobs.front();
        }
        firstProcCycles = 0;
        break;
    }
  
    if (jobs.front() == firstProc && firstProcCycles < maxCycles){
      firstProcCycles++;
    } else {
      jobs.push_back(jobs.front());
      jobs.pop_front();
      firstProcCycles = 0;
    }

    updateGraph();
    
  }

    double avgResp = std::accumulate(respTimes, respTimes + (short)numjobs, 0.0) / numjobs;
    double avgTurn = std::accumulate(turnTimes, turnTimes + (short)numjobs, 0.0) / numjobs;
    std::cout << "Completed all jobs\n\tAvg Response Time: " << avgResp << " seconds\n\tAvg Turnaround Time: " << avgTurn << " seconds" << std::endl;
    simuPrint("Completed all jobs\n\tAvg Response Time: " + std::to_string(avgResp) + " seconds\n\tAvg Turnaround Time: " + std::to_string(avgTurn) + " seconds\n");
    clearQueue();
    setupPage->Enable();
    startBtn->Enable();
    exportBtn->Enable();
}

void RedRobinBatman(std::list<process*> &jobs){
  float numJobs = jobs.size();
  if (numJobs == 0) return;

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

  std::cout << "Running " << numJobs << " with Red Robin (Batman) Scheduling" << std::endl;
  simuPrint("Running " + std::to_string(numJobs) + " with Red Robin (Batman) Scheduling\n");

  std::list<process*> goons;
  std::list<process*> bosses;

  // Initial pass to identify goons and bosses
  for (process* p : jobs) {
    if (p->timeLeft() <= 5) {
      goons.push_back(p);
      std::cout << "Process " << p->getID() << " identified as a Goon." << std::endl;
      simuPrint("Process " + std::to_string(p->getID()) + " identified as a Goon.\n");
    } else {
      bosses.push_back(p);
      std::cout << "Process " << p->getID() << " identified as a Boss." << std::endl;
      simuPrint("Process " + std::to_string(p->getID()) + " identified as a Boss.\n");
    }
  }
  jobs.clear();

  // FIFO for Goons
  std::cout << "Processing Goons (FIFO)..." << std::endl;
  simuPrint("Processing Goons (FIFO)...\n");
  while(!goons.empty()){
    process* current = goons.front();
    if (!current->isResponded()){
      respTimes[current->getID()] = current->respond();
      respSum += respTimes[current->getID()];
      respCounter++;
      respLine->AddData((float)(respCounter / numJobs) * 100, (respSum / (respCounter)), true);
    }

    switch(current->run()){
      case BLOCKED:
        current->ioCall();
        std::cout << current->getID() << ": I/O Access (Goon)" << std::endl;
        simuPrint("Process " + std::to_string(current->getID()) + ": I/O Access (Goon)\n");
        std::this_thread::sleep_for(IO_TIME);
        break;
      case DONE:
        turnTimes[current->getID()] = current->turnaround();
        turnSum += turnTimes[current->getID()];
        turnCounter++;
        turnLine->AddData((float)(turnCounter / numJobs) * 100, (turnSum / (turnCounter)), true);
        std::cout << current->getID() << ": Done (Goon)" << std::endl;
        simuPrint("Process " + std::to_string(current->getID()) + ": Done (Goon)\n");
        delete(current);
        goons.pop_front();
        break;
      default:
        std::cout << current->getID() << ": Running (Goon)" << std::endl;
        simuPrint("Process " + std::to_string(current->getID()) + ": Running (Goon)\n");
        std::this_thread::sleep_for(TIME_SLICE);
        break;
    }
    updateGraph();
  }

  // Round Robin for Bosses
  if (!bosses.empty()){
    std::cout << "Processing Bosses (Round Robin)..." << std::endl;
    simuPrint("Processing Bosses (Round Robin)...\n");
    
    std::list<process*> ioQueue;
    std::thread running(ioCall, std::ref(ioQueue), std::ref(bosses));
    running.detach();

    while(!bosses.empty()){
      // Busy wait if front is blocked - but careful if all are blocked
      // We'll move blocked ones to the back to find one that's READY/RUNNING
      size_t searchCount = 0;
      while(!bosses.empty() && bosses.front()->getStatus() == BLOCKED && searchCount < bosses.size()){
        bosses.push_back(bosses.front());
        bosses.pop_front();
        searchCount++;
      }

      if (bosses.empty()) break;
      
      // If still blocked after cycling, it means all are in IO or blocked.
      if (bosses.front()->getStatus() == BLOCKED) {
          std::this_thread::sleep_for(TIME_SLICE);
          updateGraph();
          continue;
      }

      process* current = bosses.front();

      if (!(current->isResponded())){
        respTimes[current->getID()] = current->respond();
        respSum += respTimes[current->getID()];
        respCounter++;
        respLine->AddData((float)(respCounter / numJobs) * 100, (respSum / (respCounter)), true);
      }

      switch (current->run()){
        case RUNNING:
          bosses.push_back(current);
          std::cout << current->getID() << ": Running (Boss)" << std::endl;
          simuPrint("Process " + std::to_string(current->getID()) + ": Running (Boss)\n");
          break;
        case BLOCKED:
          ioQueue.push_back(current);
          std::cout << current->getID() << ": I/O Call (Boss)" << std::endl;
          simuPrint("Process " + std::to_string(current->getID()) + ": I/O Call (Boss)\n");
          break;
        case DONE:
          turnTimes[current->getID()] = current->turnaround();
          turnSum += turnTimes[current->getID()];
          turnCounter++;
          turnLine->AddData((float)(turnCounter / numJobs) * 100, (turnSum / (turnCounter)), true);
          std::cout << current->getID() << ": Done (Boss)" << std::endl;
          simuPrint("Process " + std::to_string(current->getID()) + ": Done (Boss)\n");
          delete(current);
          break;
      }
      bosses.pop_front();
      std::this_thread::sleep_for(TIME_SLICE);
      updateGraph();
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

void DeadRobin(std::list<process*> &jobs){
  float numJobs = jobs.size();
  if (numJobs == 0) return;

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

  std::cout << "Running " << numJobs << " with Dead Robin Scheduling" << std::endl;
  simuPrint("Running " + std::to_string(numJobs) + " with Dead Robin Scheduling\n");

  std::list<process*> goons;
  std::list<process*> bosses;

  // Initial pass to identify goons and bosses
  for (process* p : jobs) {
    if (p->timeLeft() <= 5) {
      goons.push_back(p);
      std::cout << "Process " << p->getID() << " identified as a Goon." << std::endl;
      simuPrint("Process " + std::to_string(p->getID()) + " identified as a Goon.\n");
    } else {
      bosses.push_back(p);
      std::cout << "Process " << p->getID() << " identified as a Boss." << std::endl;
      simuPrint("Process " + std::to_string(p->getID()) + " identified as a Boss.\n");
    }
  }
  jobs.clear();

  // FIFO for Bosses (Terrible efficiency: doing long jobs first)
  std::cout << "Processing Bosses (FIFO)..." << std::endl;
  simuPrint("Processing Bosses (FIFO)...\n");
  while(!bosses.empty()){
    process* current = bosses.front();
    if (!current->isResponded()){
      respTimes[current->getID()] = current->respond();
      respSum += respTimes[current->getID()];
      respCounter++;
      respLine->AddData((float)(respCounter / numJobs) * 100, (respSum / (respCounter)), true);
    }

    switch(current->run()){
      case BLOCKED:
        current->ioCall();
        std::cout << current->getID() << ": I/O Access (Boss)" << std::endl;
        simuPrint("Process " + std::to_string(current->getID()) + ": I/O Access (Boss)\n");
        std::this_thread::sleep_for(IO_TIME);
        break;
      case DONE:
        turnTimes[current->getID()] = current->turnaround();
        turnSum += turnTimes[current->getID()];
        turnCounter++;
        turnLine->AddData((float)(turnCounter / numJobs) * 100, (turnSum / (turnCounter)), true);
        std::cout << current->getID() << ": Done (Boss)" << std::endl;
        simuPrint("Process " + std::to_string(current->getID()) + ": Done (Boss)\n");
        delete(current);
        bosses.pop_front();
        break;
      default:
        std::cout << current->getID() << ": Running (Boss)" << std::endl;
        simuPrint("Process " + std::to_string(current->getID()) + ": Running (Boss)\n");
        std::this_thread::sleep_for(TIME_SLICE);
        break;
    }
    updateGraph();
  }

  // Round Robin for Goons
  if (!goons.empty()){
    std::cout << "Processing Goons (Round Robin)..." << std::endl;
    simuPrint("Processing Goons (Round Robin)...\n");
    
    std::list<process*> ioQueue;
    std::thread running(ioCall, std::ref(ioQueue), std::ref(goons));
    running.detach();

    while(!goons.empty()){
      size_t searchCount = 0;
      while(!goons.empty() && goons.front()->getStatus() == BLOCKED && searchCount < goons.size()){
        goons.push_back(goons.front());
        goons.pop_front();
        searchCount++;
      }

      if (goons.empty()) break;
      
      if (goons.front()->getStatus() == BLOCKED) {
          std::this_thread::sleep_for(TIME_SLICE);
          updateGraph();
          continue;
      }

      process* current = goons.front();

      if (!(current->isResponded())){
        respTimes[current->getID()] = current->respond();
        respSum += respTimes[current->getID()];
        respCounter++;
        respLine->AddData((float)(respCounter / numJobs) * 100, (respSum / (respCounter)), true);
      }

      switch (current->run()){
        case RUNNING:
          goons.push_back(current);
          std::cout << current->getID() << ": Running (Goon)" << std::endl;
          simuPrint("Process " + std::to_string(current->getID()) + ": Running (Goon)\n");
          break;
        case BLOCKED:
          ioQueue.push_back(current);
          std::cout << current->getID() << ": I/O Call (Goon)" << std::endl;
          simuPrint("Process " + std::to_string(current->getID()) + ": I/O Call (Goon)\n");
          break;
        case DONE:
          turnTimes[current->getID()] = current->turnaround();
          turnSum += turnTimes[current->getID()];
          turnCounter++;
          turnLine->AddData((float)(turnCounter / numJobs) * 100, (turnSum / (turnCounter)), true);
          std::cout << current->getID() << ": Done (Goon)" << std::endl;
          simuPrint("Process " + std::to_string(current->getID()) + ": Done (Goon)\n");
          delete(current);
          break;
      }
      goons.pop_front();
      std::this_thread::sleep_for(TIME_SLICE);
      updateGraph();
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