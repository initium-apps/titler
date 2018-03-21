#ifndef TITLER_H
#define TITLER_H

#include<wx/wx.h>
#include<wx/textfile.h>
#include<wx/mediactrl.h>
#include<wx/splitter.h>
#include<wx/sizer.h>
#include<wx/html/htmlwin.h>

#include"property.h"

#include"Project.h"
#include"titTimeline.h"
#include"titMediaCtrl.h"
#include"titBlackPanel.h"
#include"titMenuBar.h"

#define wxUSE_MOUSEWHEEL 1

#define ID_QUIT wxID_EXIT
#define ID_MEDIA 10

class Titler : public wxApp {
    private:
        wxFrame* mainFrame = NULL;
        wxBoxSizer* mainSizer = NULL;

        titMenuBar* menubar = NULL;

        wxSplitterWindow* splitter = NULL;

        titBlackPanel* mediaPanel = NULL;
        wxBoxSizer* mediaSizer = NULL;
        titMediaCtrl* media = NULL;
        wxStaticText* currentSubtitle = NULL;

        titTimeline* timeline = NULL;

        wxFileDialog* openDg = NULL;
        wxFileDialog* saveDg = NULL;

        Project* curProj = NULL;
    public:
        virtual bool OnInit();
        virtual int OnExit();

        bool OpenProject(wxString filepath);
        bool SaveProject(wxString filepath);
        void Reset();

        void QuitEv(wxCommandEvent&);

        // hacky hacky
        int FilterEvent(wxEvent&);

        void TimerNotifyTest(wxTimerEvent&);
};

#endif // TITLER_H
