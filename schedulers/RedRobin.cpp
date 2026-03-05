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

  const int maxCycles = 10;
  int firstProcCycles = 1;

  process* firstProc = jobs.front();

  while(jobs.size() > 0){
    switch (jobs.front()->run())
    {
      case RUNNING:
        std::cout << "Running Job " << jobs.front()->getID() << "." << std::endl;
        break;
      case BLOCKED:
        std::cout << "Running Job " << jobs.front()->getID() << "." << std::endl;
        break;
     case DONE:
        std::cout << "Completed Job " << jobs.front()->getID() << "." << std::endl;
        jobs.pop_front();
        break;
      
      default:
        break;
    }



  }

}
