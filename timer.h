/********************************************* 
The Timer Class is a set of tools to track

- Individual Process Response Time
- Individual Process Turnaround Time
- Average Process Response Tme
- Average Process Turnaround Time

It has 4 lists that will expand as new
processes are given to it. When a process is
started or completed, a function will be called
to mark the time from start to completion
********************************************/

#include <chrono>>
#include <algorithm>
#include "schedulers/schedulers.h"

class timer{
    private:
    std::list<time_t> startTime, respTime, turnTime;
    std::list<time_t>::iterator timeIter;

    std::list<short> procNumbers;
    std::list<short>::iterator procIter;

    public:
    void start(int procNum);
    double respond(int procNum);
    bool isResponded(int procNum);
    double turnaround(int procNum);
};