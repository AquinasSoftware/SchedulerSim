/********************************************* 
The Process Class is a template that contains:
    -ID
    -Process Type
    -Completion Time
    -IO Request Count
    -Status

There are several types of processes, each with different
intialized parameters. The process type is given to the
constructor, along with an ID number When the completion time
hits 0, it IO request count will be decreased. When both
counters reach 0, the process is finished and the
deconstructor function is called.
********************************************/
#include <map>

enum status {READY, RUNNING, BLOCKED, DONE};
enum procName{
    OS_FUNCTION,
    TEXT_EDITOR,
    MEDIA_PLAYER
};
struct defaults{
    short dRunTime, dIoCount;
};

// Map of Initialized Parameters
//  Format: Type, {runTime, ioCount}
const std::map<procName, defaults> params = {
    {OS_FUNCTION, {5, 0}},
    {TEXT_EDITOR, {2, 2}},
    {MEDIA_PLAYER, {5, 1}}
};

class process{
    private:
        short ID;
        procName procType;
        short runTime;
        short ioCount;
        status currentStatus;

    public:
        process(procName type, short procID);

        short getID();
        status run();
        void ioCall();
        short timeLeft();
        short getStatus();
};