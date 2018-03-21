#include"Titler.h"
#include<stdio.h>
#include<wx/filefn.h>
#include<wx/wfstream.h>

/* XPM */
static char *icon[] = {
/* columns rows colors chars-per-pixel */
"32 32 2 1 ",
"  c #33B83A",
". c None",
/* pixels */
"................................",
"................................",
".........              .........",
".........              .........",
".........              .........",
".........              .........",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"..............    ..............",
"................................",
"................................"
};

IMPLEMENT_APP(Titler)

bool Titler::OnInit() {
    this->mainFrame = new wxFrame(NULL, wxID_ANY, "Titler", wxDefaultPosition, wxSize(500, 500));

    this->menubar = new titMenuBar(this);
    this->mainFrame->SetMenuBar(this->menubar);

    this->splitter = new wxSplitterWindow(this->mainFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
    this->splitter->SetMinimumPaneSize(20);
    this->splitter->SetSashGravity(0.5);

    this->mainSizer = new wxBoxSizer(wxVERTICAL);
    this->mainSizer->Add(this->splitter, 1, wxEXPAND, 0);

    this->mediaPanel = new titBlackPanel(this->splitter);
    this->mediaSizer = new wxBoxSizer(wxVERTICAL);
    this->media = new titMediaCtrl(this->mediaPanel);
    this->currentSubtitle = new wxStaticText(this->mediaPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTRANSPARENT_WINDOW);
	this->currentSubtitle->SetForegroundColour(*wxWHITE);
    this->currentSubtitle->SetFont(this->currentSubtitle->GetFont().MakeLarger().MakeLarger());
    this->mediaSizer->Add(this->media, 1, wxEXPAND | wxGROW, 0);
    this->mediaSizer->Add(this->currentSubtitle, 0, wxALIGN_CENTER_HORIZONTAL, 0);
    this->mediaPanel->SetSizer(this->mediaSizer);

    this->media->timer.Bind(wxEVT_TIMER, &Titler::TimerNotifyTest, this, this->media->timer.GetId());

    this->timeline = new titTimeline(this->splitter, this->media);

    this->splitter->SplitHorizontally(this->mediaPanel, this->timeline);

    this->mainFrame->Bind(wxEVT_MENU, &Titler::QuitEv, this, wxID_EXIT);
    this->mainFrame->Bind(wxEVT_MENU, [this](wxCommandEvent&WXUNUSED(ev))->void{
        this->SaveProject(wxEmptyString);
    }, wxID_SAVE);
    this->mainFrame->Bind(wxEVT_MENU, [this](wxCommandEvent&WXUNUSED(ev))->void{
        this->OpenProject(wxEmptyString);
    }, wxID_OPEN);

    this->mainFrame->SetSizer(this->mainSizer);
    this->mainFrame->SetIcon(wxIcon(icon));
    this->mainFrame->Centre();
    this->mainFrame->Show(true);

    this->openDg = new wxFileDialog(this->mainFrame, "Open a video file", wxEmptyString, wxEmptyString, "MP4 files (*.mp4)|*.mp4", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    this->saveDg = new wxFileDialog(this->mainFrame, "Save next to a video file", wxEmptyString, wxEmptyString, "MP4 files (*.mp4)|*.mp4", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    this->OpenProject(wxEmptyString);

    return true;
}

int Titler::OnExit() {
    delete this->curProj;
    return 0;
}

bool Titler::SaveProject(wxString filepath) {
    if(filepath.IsEmpty()) {
        if(this->saveDg->ShowModal() == wxID_OK)
            filepath = this->saveDg->GetPath();
        else
            return false;
    }

    filepath = filepath.BeforeLast('.') + ".srt";
    wxFileOutputStream fos(filepath);

    this->curProj->Save(fos, this->timeline);

    fos.Close();

    return true;
}

bool Titler::OpenProject(wxString filepath) {
    if(filepath.IsEmpty()) {
        if(this->openDg->ShowModal() == wxID_OK)
            filepath = this->openDg->GetPath();
        else
            return false;
    }

    if(this->curProj)
        delete this->curProj;
    this->curProj = new Project(filepath);

    filepath = filepath.BeforeLast('.') + ".srt";

    this->Reset();
    this->curProj->Open(filepath.ToStdWstring(), this->timeline);

    return true;
}

void Titler::Reset() {
    this->timeline->Reset();
    this->media->Load(this->curProj->mediaLoc.get());
}

void Titler::QuitEv(wxCommandEvent& WXUNUSED(ev)) {
    this->mainFrame->Close(true);
}

int Titler::FilterEvent(wxEvent& ev) {
    // fixes a bug where events were filtered even in text controls
    if(dynamic_cast<wxTextCtrl*>(ev.GetEventObject()) != NULL)
        return -1;

    if(ev.GetEventType() == wxEVT_KEY_DOWN) {
        switch(static_cast<wxKeyEvent&>(ev).GetKeyCode()) {
            case WXK_SPACE:
                if(this->media->GetState() == wxMEDIASTATE_PLAYING)
                    this->media->Pause();
                else
                    this->media->Play();
                break;
            case 'n':
            case 'N':
                if(this->timeline->nSubtitle == NULL) {
                    this->timeline->CreateNSubtitle("N Subtitle");
                }
                break;
            default:
                return -1;
        }
        return true;
    } else if(ev.GetEventType() == wxEVT_KEY_UP) {
        switch(static_cast<wxKeyEvent&>(ev).GetKeyCode()) {
            case 'n':
            case 'N':
                this->media->Pause();
                this->timeline->FinishNSubtitle()->Edit(this->mainFrame);
                this->media->Play();
                break;
            default:
                return -1;
        }
        return true;
    }
    return -1;
}

void Titler::TimerNotifyTest(wxTimerEvent& WXUNUSED(ev)) {
    this->timeline->handle->SetPosition(this->timeline->CalcScrolledPosition(wxPoint((int) (this->media->Tell() * this->timeline->zoom - 2), 0)));

    this->timeline->UpdateNSubtitle();

    titSubtitle* subtitle = this->timeline->DetermineCurrentSubtitle(this->media->Tell());
    this->currentSubtitle->SetLabelMarkup(subtitle ? subtitle->text->get() : wxString(wxEmptyString));
}
