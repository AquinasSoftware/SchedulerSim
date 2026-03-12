#include <wx/wx.h>
#include "schedulers/schedulers.h"

class GUI : public wxApp {
    public:
        bool OnInit() override;
};

wxIMPLEMENT_APP(GUI);

class windowFrame : public wxFrame {
    public:
        windowFrame();
    
    private:
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
};

enum
{
    ID_Hello = 1
};

bool GUI::OnInit()
{
    windowFrame *frame = new windowFrame();
    frame->Show(true);
    return true;
}

windowFrame::windowFrame()
    : wxFrame(nullptr, wxID_ANY, "Scheduler Simulator")
{
 
    Bind(wxEVT_MENU, &windowFrame::OnExit, this, wxID_EXIT);
}

void windowFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

/*
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
    std::cout << "1. FIFO\n2. SJF\n3. RR\n4. RRR\n5. SWQ\n\n";

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
        case 4:
            RedRobinRestaurant(jobs);
            break;
        default:
            SWQ(jobs);
            break;
    }

    return 0;
}
    */