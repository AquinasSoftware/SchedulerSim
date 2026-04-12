# Scheduler Sim
Scheduler Sim is an educational tool to showcase how various algorithms can be used by an operating system to complete a list of processes on a single-core CPU. It accepts a user-specified list of jobs, then graphs the average response and turnaround times as the queue is completed. These results can then be exported for comparison.

## Included Algorithms
The following standard scheduling algorithms are included:
- FIFO (First In, First Out)
- SJF (Shortest Job First)
- RR (Round Robin)

Additionally, a custom-made algorithm, Smart Window Queue (SWQ) combines FIFO and RR to create a hybrid approach. Processes are Queued in the order they were received. Then, the top 3 are equally run until one either completes or makes an I/O call.

Finally, 3 joke algorithms are included, Red Robin Restaraunt (RRR,) Red Robin Batman (RRB,) and Dead Robin (DR.) RRR takes a Round Robin approach but modifies it to give increased time slices to the processes that were started first. RRB is a modification of SJF. It runs the fastest processes in a FIFO manner, then does a RR approach on the longer processes. Finally, DR is the inverse of RRB, doing a Round Robin approach on the smallest processes first, before performing a FIFO algorithm on the longest jobs to maximize response and turnaround time.

## Building
Pre-build binaries are included for Windows and Debian-based systems. However, The project files contain a Makefile for self-building on a linux system.
### Windows
To build a Windows x64 version, the *build-essential* package containing make, gcc and g++ is required. Additionally, the *MinGW-w64* is needed for cross-compilation. Specifically, the Makefile utilizes *x86-64-w64-mingw32-g++*. All other binaries are included in the source files as static libraries. Finally, in the root folder, run ```make win64```.
### Linux
To build a Linux version, the *build-essential* package containing make, gcc and g++ is required. Additionally, the wxWidgets 3.2 development library is required. Please note, the dev environment was on a debian-based system so some minor modifications to the Makefile may be required for other operating systems. The Linux option is default, so simply run ```make``` from the project root.

## Running
For Windows, the Scheduler Sim package includes all the files needed to run it. On Linux, wxGTK 3.2 runtime libraries are required before running, along with The GTK3 and C++ runtimes.