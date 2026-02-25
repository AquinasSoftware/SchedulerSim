#include "timer.h"

void timer::start(int procNum){
    procNumbers.push_back(procNum);
    startTime.push_back(time(nullptr));
    respTime.push_back(NULL);
    turnTime.push_back(NULL);
}

double timer::respond(int procNum){
    procIter = std::find(procNumbers.begin(), procNumbers.end(), procNum);
    timeIter = respTime.begin();
    std::advance(timeIter, std::distance(procNumbers.begin(), procIter));
}

double timer::turnaround(int procNum){

}