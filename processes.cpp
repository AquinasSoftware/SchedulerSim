#include "processes.h"

process::process(procName type, short procID){
    ID = procID;
    procType = type;
    runTime = params.at(type).dRunTime;
    ioCount = params.at(type).dIoCount;
    currentStatus = READY;
    startTime = std::chrono::steady_clock::now();
}

void process::start(){
    startTime = std::chrono::steady_clock::now();
    respTime = -1.0;
    turnTime = -1.0;
}

// Return Process ID
short process::getID(){
    return ID;
}

void process::setID(short newID){
    ID = newID;
}

const char* process::getType(){
    switch (procType){
        case OS_FUNCTION:
            return "OS Function";
        case TEXT_EDITOR:
            return "Text Editor";
        case MEDIA_PLAYER:
            return "Media Player";
        case IMAGE_EDITOR:
            return "Image Editor";
        case WEB_BROWSER:
            return "Web Browser";
        case VIDEO_STREAMER:
            return "Video Streamer";
        case AI_DESKTOP_ASSISTANT:
            return "AI Desktop Assistant";
        case ANTIVIRUS_SCANNER:
            return "Antivirus Scanner";
        case FILE_COMPRESSOR:
            return "File Compressor";
        default:
            return "Unknown";
    }
}

enum procName process::getProcEnum(){
    return procType;
}

short process::getStatus(){
    return currentStatus;
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

// Calculates time left for SJF
short process::timeLeft(){
    return runTime + (ioCount * params.at(procType).dRunTime);
}

// Synchronous function that simulates an IO Call
void process::ioCall(){
    ioCount--;
    runTime = params.at(procType).dRunTime;
    currentStatus = READY;
}

double process::respond(){
    if(respTime < 0){
        auto now = std::chrono::steady_clock::now();
        respTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - startTime).count();
    }
    return respTime;
}

bool process::isResponded(){
    return respTime >= 0;
}

double process::turnaround(){
    if(turnTime < 0){
        auto now = std::chrono::steady_clock::now();
        turnTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - startTime).count();
    }
    return turnTime;
}