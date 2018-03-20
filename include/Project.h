#ifndef PROJECT_H
#define PROJECT_H

#include<wx/wx.h>
#include<wx/textfile.h>
#include<wx/wfstream.h>
#include"property.h"

#include"titTimeline.h"

class Project {
    public:
        property<wxString> subsLoc;
        property<wxString> mediaLoc;

        Project(wxString);

        void Save(wxFileOutputStream&, titTimeline*);
        void Open(wxString, titTimeline*);
};

#endif // PROJECT_H
