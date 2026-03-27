/* Red Robin

*** Red Robin Restaurant ***
Red Robin (restaurant) is a scheduling algorithm based on a restaurant. 
RRR aims to please ALL the customers (processes) but also serve them in
order of arrival. RRR is a mix of FIFO and Round Robin.

** Red Robin Batman ***



*/
#include "schedulers.h"

void RedRobinRestaurant(std::list<process*> jobs){

  int numjobs = jobs.size();
  std::cout << "Running " << numjobs << " with Red Robin (Restaurant) Scheduling" << std::endl;
  time_t starttime = time(nullptr);

  const int maxCycles = 2;
  int firstProcCycles = 0;

  process* firstProc = jobs.front();

  while(jobs.size() > 0){

    switch (jobs.front()->run())
    {
      case RUNNING:
        std::cout << "Running Job " << jobs.front()->getID() << "." << std::endl;
        std::this_thread::sleep_for(TIME_SLICE);
        break;
      case BLOCKED:
        jobs.front()->ioCall();
        std::cout << "Job " << jobs.front()->getID() << " IO Call" << std::endl;
        std::this_thread::sleep_for(IO_TIME);
        break;
     case DONE:
        std::cout << "Completed Job " << jobs.front()->getID() << "." << std::endl;
        jobs.pop_front();
        if (jobs.size() > 0){
          firstProc = jobs.front();
        }
        break;
    }
  
    if (jobs.front() == firstProc && firstProcCycles < maxCycles){
      firstProcCycles++;
    } else {
      jobs.push_back(jobs.front());
      jobs.pop_front();
      firstProcCycles = 0;
    }
    
  }

  std::cout << "Finished Processing" << std::endl;

}
