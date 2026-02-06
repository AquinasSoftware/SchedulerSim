#include "schedulers/schedulers.h"

// Testing Function until GUI is added
int main(){
    std::list<process*> jobs;
    process *job1 = new process(OS_FUNCTION, 0);
    process *job2 = new process(TEXT_EDITOR, 1);
    process *job3 = new process(MEDIA_PLAYER, 2);
    process *job4 = new process(TEXT_EDITOR, 3);
    process *job5 = new process(OS_FUNCTION, 4);

    jobs.push_back(job1);
    jobs.push_back(job2);
    jobs.push_back(job3);
    jobs.push_back(job4);
    jobs.push_back(job5);

    SWQ(jobs);

    return 0;
}