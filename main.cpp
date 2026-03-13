#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
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

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);

    wxPanel* setupPage = new wxPanel(notebook, wxID_ANY);
    wxBoxSizer *setupSkeleton = new wxBoxSizer(wxVERTICAL);

    wxStaticText* selectionLbl = new wxStaticText(setupPage, wxID_ANY, "Step 1: Create a Process Queue:", wxPoint(10, 10));
    selectionLbl->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    setupSkeleton->Add(selectionLbl, 0, wxALL, 10);

    wxBoxSizer *step1Skeleton = new wxBoxSizer(wxHORIZONTAL);
    wxListView* taskList = new wxListView(setupPage, wxID_ANY, wxDefaultPosition, wxSize(300, 300), wxLC_REPORT);
    taskList->InsertColumn(0, "Process Type");
    taskList->InsertColumn(1, "Speed");
    taskList->SetColumnWidth(0, 150);
    taskList->SetColumnWidth(1, 150);
    step1Skeleton->Add(taskList, 0, wxLEFT, 10);

    wxListView* taskQueue = new wxListView(setupPage, wxID_ANY, wxDefaultPosition, wxSize(250, 300), wxLC_REPORT);
    taskQueue->InsertColumn(0, "Process ID");
    taskQueue->InsertColumn(1, "Type");
    taskQueue->SetColumnWidth(0, 100);
    taskQueue->SetColumnWidth(1, 150);
    step1Skeleton->Add(taskQueue, 0, wxRIGHT, 10);

    setupSkeleton->Add(step1Skeleton, 0, wxALL, 10);
    setupPage->SetSizer(setupSkeleton);
    notebook->AddPage(setupPage, "Setup");
    windowFrame::SetMinSize(wxSize(750, 720));
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