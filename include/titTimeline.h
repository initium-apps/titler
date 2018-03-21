#ifndef TITTIMELINE_H
#define TITTIMELINE_H

#include<wx/wx.h>
#include<wx/sizer.h>

#include<vector>

#include"titSubtitle.h"
#include"titHandle.h"
#include"property.h"
#include"titMediaCtrl.h"

class titTimeline : public wxScrolledWindow {
    private:
        titMediaCtrl* media = NULL; // used to pass to the handle in ResetHandle

        void ResetHandle();
    public:
        titHandle* handle = NULL;
        std::vector<titSubtitle*> subs;
        titSubtitle* nSubtitle = NULL;

        float zoom = 0.1f;

        titTimeline(wxWindow*, titMediaCtrl*);

        titSubtitle* insertSubtitle(wxTimeSpan, wxTimeSpan, property<wxString>*);
        void CreateNSubtitle(wxString);
        void UpdateNSubtitle();
        titSubtitle* FinishNSubtitle();

        void ResizeEv(wxSizeEvent&);
        void ScrollEv(wxMouseEvent&);

        titSubtitle* DetermineCurrentSubtitle(double);

        void UpdateVirtualSize();

        void Reset();
};

#endif // TITTIMELINE_H
