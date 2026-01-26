#include "processes.h"

process::process(procType typeOfProc, short procID){
    ID = procID;
    type = typeOfProc;
    runTime = params.at(type).dRunTime;
    ioCount = params.at(type).dIoCount;
    currentStatus = READY;
}

// Return Process ID
short process::getID(){
    return ID;
}

// Run process and return the process status
status process::run(){
    if (currentStatus == READY){
        currentStatus = RUNNING;
    }

    if(currentStatus == RUNNING){
        runTime--;
        if (runTime <= 0 && ioCount <= 0){
            currentStatus = DONE;
        }
        else if (runTime <= 0){
            currentStatus = BLOCKED;
        }
    }

    return currentStatus;
}

// Synchronous function that simulates an IO Call
void process::ioCall(){
    ioCount--;
    runTime = params.at(type).dRunTime;
    currentStatus = READY;
}