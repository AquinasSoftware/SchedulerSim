/********************************************* 
The Process Class is a template that contains:
    -ID
    -Process Type
    -Response Time
    -Turnaround Time
    -IO Request Count
    -Status

There are several types of processes, each with different
intialized parameters. The process type is given to the
constructor, along with an ID number When the completion time
hits 0, it IO request count will be decreased. When both
counters reach 0, the process is finished and the
deconstructor function is called.
********************************************/
#ifndef PROCESS_H
#define PROCESS_H

#include <map>
#include <chrono>

enum status {READY, RUNNING, BLOCKED, DONE};
enum procName{
    AI_DESKTOP_ASSISTANT,
    ANTIVIRUS_SCANNER,
    FILE_COMPRESSOR,
    IMAGE_EDITOR,
    MEDIA_PLAYER,
    OS_FUNCTION,
    TEXT_EDITOR,
    WEB_BROWSER,
    VIDEO_STREAMER
};
struct defaults{
    short dRunTime, dIoCount;
};

// Map of Initialized Parameters
//  Format: Type, {runTime, ioCount}
const std::map<procName, defaults> params = {
    {AI_DESKTOP_ASSISTANT, {15, 3}},
    {ANTIVIRUS_SCANNER, {2, 15}},
    {FILE_COMPRESSOR, {5, 5}},
    {IMAGE_EDITOR, {3, 1}},
    {MEDIA_PLAYER, {5, 3}},
    {OS_FUNCTION, {5, 0}},
    {TEXT_EDITOR, {2, 2}},
    {WEB_BROWSER, {2, 0}},
    {VIDEO_STREAMER, {6, 0}}
};

class process{
    private:
        short ID;
        procName procType;
        short runTime;
        short ioCount;
        status currentStatus;
        std::chrono::steady_clock::time_point startTime;
        double respTime = -1.0;
        double turnTime = -1.0;

    public:
        process(procName type, short procID);

        void start();
        short getID();
        void setID(short newID);
        const char* getType();
        enum procName getProcEnum();
        status run();
        void ioCall();
        short timeLeft();
        short getStatus();

        double respond();
        bool isResponded();
        double turnaround();
};

#endif