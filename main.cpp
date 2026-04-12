#include "main.h"
std::list<process*> jobs;
wxPanel* setupPage;
wxListView* taskList;
wxListView* taskQueue;
wxListView* schedulerList;
wxTextCtrl* schedulerDesc;
wxPanel* simuPage;
wxButton* startBtn;
wxButton* exportBtn;
mpWindow* graph;
wxTextCtrl* simuOutput;

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
    // If dev flag is set, run cmd testing function
    for (int i = 1; i < argc; i++){
        wxString arg(argv[i]);
        if (arg == "-dev" || arg == "--dev"){
            devTest();
            return false;
        }
    }

    // Else, run the GUI
    windowFrame *frame = new windowFrame();
    frame->SetIcon(wxIcon("ss.xpm"));
    frame->Show(true);
    return true;
}

windowFrame::windowFrame()
    : wxFrame(nullptr, wxID_ANY, "Scheduler Sim", 
        wxDefaultPosition, wxSize(800, 720),
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)){

    Bind(wxEVT_MENU, &windowFrame::OnExit, this, wxID_EXIT);

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);

    setupPage = new wxPanel(notebook, wxID_ANY);
    wxBoxSizer *setupSkeleton = new wxBoxSizer(wxVERTICAL);

// Step 1: Process Queue Creation
    wxStaticText* step1Lbl = new wxStaticText(setupPage, wxID_ANY, "Step 1: Create a Process Queue", wxPoint(10, 10));
    step1Lbl->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    setupSkeleton->Add(step1Lbl, 0, wxALL, 10);

    wxBoxSizer *step1Skeleton = new wxBoxSizer(wxHORIZONTAL);
    taskList = new wxListView(setupPage, wxID_ANY, wxDefaultPosition, wxSize(200, 250), wxLC_REPORT | wxLC_SINGLE_SEL);
    taskList->InsertColumn(0, "Process Type");
    taskList->SetColumnWidth(0, 200);
    taskList->InsertItem(0, "AI Desktop Assistant");
    taskList->InsertItem(1, "Antivirus Scanner");
    taskList->InsertItem(2, "File Compressor");
    taskList->InsertItem(3, "Image Editor");
    taskList->InsertItem(4, "Media Player");
    taskList->InsertItem(5, "OS Function");
    taskList->InsertItem(6, "Text Editor");
    taskList->InsertItem(7, "Web Browser");
    taskList->InsertItem(8, "Video Streamer");

    step1Skeleton->Add(taskList, 0, wxLEFT, 10);

    wxBoxSizer* taskButtons = new wxBoxSizer(wxVERTICAL);
    wxButton* addTaskBtn = new wxButton(setupPage, wxID_ANY, "Add");
    wxButton* removeTaskBtn = new wxButton(setupPage, wxID_ANY, "Remove");
    wxButton* clearTasksBtn = new wxButton(setupPage, wxID_ANY, "Clear All");
    wxStaticLine* btnDivider = new wxStaticLine(setupPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    wxButton* importTasksBtn = new wxButton(setupPage, wxID_ANY, "Import");
    wxButton* exportTasksBtn = new wxButton(setupPage, wxID_ANY, "Export");
    taskButtons->Add(addTaskBtn, 0, wxALL, 5);
    taskButtons->Add(removeTaskBtn, 0, wxALL, 5);
    taskButtons->Add(clearTasksBtn, 0, wxALL, 5);
    taskButtons->Add(btnDivider, 0, wxEXPAND | wxALL, 5);
    taskButtons->Add(importTasksBtn, 0, wxALL, 5);
    taskButtons->Add(exportTasksBtn, 0, wxALL, 5);
    step1Skeleton->Add(taskButtons, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
    Bind(wxEVT_BUTTON, [=](wxCommandEvent&){ addToJobs(); }, addTaskBtn->GetId());
    Bind(wxEVT_BUTTON, [=](wxCommandEvent&){ removeFromJobs(); }, removeTaskBtn->GetId());
    Bind(wxEVT_BUTTON, [=](wxCommandEvent&){ clearQueue(); }, clearTasksBtn->GetId());
    Bind(wxEVT_BUTTON, [=](wxCommandEvent&){ importJobs(); }, importTasksBtn->GetId());
    Bind(wxEVT_BUTTON, [=](wxCommandEvent&){ exportJobs(); }, exportTasksBtn->GetId());
    taskQueue = new wxListView(setupPage, wxID_ANY, wxDefaultPosition, wxSize(250, 250), wxLC_REPORT);
    taskQueue->InsertColumn(0, "Process ID");
    taskQueue->InsertColumn(1, "Type");
    taskQueue->SetColumnWidth(0, 100);
    taskQueue->SetColumnWidth(1, 150);
    step1Skeleton->Add(taskQueue, 0, wxRIGHT, 10);

    setupSkeleton->Add(step1Skeleton, 0, wxALIGN_CENTER_HORIZONTAL, 10);

    wxStaticLine* divider = new wxStaticLine(setupPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    setupSkeleton->Add(divider, 0, wxEXPAND | wxALL, 5);

// Step 2: Scheduler Selection
    wxStaticText* step2Lbl = new wxStaticText(setupPage, wxID_ANY, "Step 2: Select a Scheduler", wxPoint(10, 10));
    step2Lbl->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    setupSkeleton->Add(step2Lbl, 0, wxALL, 10);

    wxBoxSizer *step2Skeleton = new wxBoxSizer(wxHORIZONTAL);
    schedulerList = new wxListView(setupPage, wxID_ANY, wxDefaultPosition, wxSize(200, 250), wxLC_REPORT | wxLC_SINGLE_SEL);
    schedulerList->InsertColumn(0, "Scheduler");
    schedulerList->SetColumnWidth(0, 200);
    schedulerList->InsertItem(0, "First In First Out");
    schedulerList->InsertItem(1, "Shortest Job First");
    schedulerList->InsertItem(2, "Round Robin");
    schedulerList->InsertItem(3, "Red Robin Restaurant");
    schedulerList->InsertItem(4, "Red Robin Batman");
    schedulerList->InsertItem(5, "Dead Robin");
    schedulerList->InsertItem(6, "Smart Window Queue");

    step2Skeleton->Add(schedulerList, 0, wxALL | wxEXPAND, 10);

    schedulerDesc = new wxTextCtrl(setupPage, wxID_ANY, "", wxDefaultPosition, wxSize(400, 250), wxTE_MULTILINE | wxTE_READONLY);   
    schedulerDesc->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    schedulerDesc->SetMargins(10, 5);
    step2Skeleton->Add(schedulerDesc, 1, wxALL| wxEXPAND, 10);

    setupSkeleton->Add(step2Skeleton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    setupPage->SetSizer(setupSkeleton);
    notebook->AddPage(setupPage, "Setup");

    simuPage = new wxPanel(notebook, wxID_ANY);
    wxBoxSizer *simuSkeleton = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *controlButtons = new wxBoxSizer(wxHORIZONTAL);
    startBtn = new wxButton(simuPage, wxID_ANY, "Start");
    Bind(wxEVT_BUTTON, [=](wxCommandEvent&){ startSimulation(); }, startBtn->GetId());
    exportBtn = new wxButton(simuPage, wxID_ANY, "Export");
    exportBtn->Disable();
    Bind(wxEVT_BUTTON, [=](wxCommandEvent&){ exportGraph(); }, exportBtn->GetId());
    controlButtons->Add(startBtn, 0, wxALL, 5);
    controlButtons->Add(exportBtn, 0, wxALL, 5);
    simuSkeleton->Add(controlButtons, 0, wxALL, 5);

// Graph
    graph = new mpWindow(simuPage, wxID_ANY, wxDefaultPosition, wxSize(750, 400));
    
    mpLayer* axisX = new mpScaleX("Jobs Handled (Total %)", mpALIGN_BOTTOM, true);
    mpLayer* axisY = new mpScaleY("Average Time (Seconds)", mpALIGN_LEFT, true);
    graph->AddLayer(axisX);
    graph->AddLayer(axisY);
    graph->SetScaleX(10);
    graph->SetPosX(0);
    graph->SetScaleY(10, 0);
    graph->SetColourTheme(wxColour(0xFF, 0xFF, 0xFF, 0xCC), *wxBLACK, *wxBLACK);
    simuSkeleton->Add((mpWindow*)graph, 1, wxALL | wxEXPAND, 10);

// Simulation Output
    simuOutput = new wxTextCtrl(simuPage, wxID_ANY, "", wxDefaultPosition, wxSize(780, 200), wxTE_MULTILINE | wxTE_READONLY);   
    simuOutput->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    simuOutput->SetMargins(10, 5);
    simuSkeleton->Add(simuOutput, 0, wxALL | wxEXPAND, 10);

    simuPage->SetSizer(simuSkeleton);
    notebook->AddPage(simuPage, "Simulation");

    Bind(wxEVT_LIST_ITEM_SELECTED, [=](wxCommandEvent&){ selectScheduler(); }, schedulerList->GetId());
    schedulerList->Select(0);
}

void windowFrame::OnExit(wxCommandEvent& event)
{
    clearQueue();
    Close(true);
}

// CMD Testing Function
void devTest(){
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
    std::cout << "1. FIFO\n2. SJF\n3. RR\n4. RRR\n5. RRB\n6. DR\n7. SWQ\n\n";

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
        case 5:
            RedRobinBatman(jobs);
            break;
        case 6:
            DeadRobin(jobs);
            break;
        default:
            SWQ(jobs);
            break;
    }
}