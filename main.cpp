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

    std::cout << "Enter Scheduler:" << std::endl;
    std::cout << "1. FIFO\n2. SJF\n3. RR\n4. SWQ\n\n";

    int choice;
    std::cin >> choice;

    switch(choice){
        case 1:
            FIFO(jobs);
            break;
        case 2:
            SJF(jobs);
            break;
        case 3:
            roundRobin(jobs);
            break;
        default:
            SWQ(jobs);
            break;
    }

    return 0;
}