const char FIFO_DESC[219] = 
"First In First Out (FIFO):\n\n"
"Proccesses are completed in the order they were received. "
"Fairness is maximized, at the expense of efficiency.";

const char SJF_DESC[143] =
"Shortest Job First (SJF):\n\n"
"The process with the shortest execution time is selected next. "
"This approach minimizes the average turnaround time.";

const char RR_DESC[145] =
"Round Robin (RR):\n\n"
"Each process is assigned a fixed time slice and is executed in a rotation. "
"This approach minimized the average response time.";

const char RRR_DESC[280] =
"Red Robin Restaurant (RRR):\n\n"
"Processes are put in a rotating queue with the time slice determined by seniority. "
"This approach balances fairness and efficiency.";

const char DR_DESC[219] =
"Dead Robin (DR):\n\n"
"The Least Efficient Scheduler. The longest jobs are executed firs in a "
"linear queue. The shorter processes are then run in a rotation. "
"This approach maximizes the average response and turnaround time.";

const char SWQ_DESC[277] =
"Smart Window Queue (SWQ):\n\n"
"Processes are put in a linear queue in the order they were received. "
"The first 3 are then executed in a rotation until one is complete or "
"issues an I/O instruction. This approach emphasises fairness and "
"turnaround time, at the cost of response time.";