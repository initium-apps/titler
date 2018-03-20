#ifndef TITSUBTITLE_H
#define TITSUBTITLE_H

#include<wx/wx.h>
#include"property.h"

#include"titBlackPanel.h"

#define TITLER_SUBTITLE_MOUSE_SNAP_EDGE_THINGY_MINIMUM_AMOUNT_TO_DO_IT 15

// forwards declaration
class titTimeline;

class titSubtitle : public wxPanel {
    private:
        titTimeline* timeline;

        wxStaticText* childText;

        bool dragLeft = false, dragRight = false;
        bool currentlyDragging = false;
        wxCoord dragX;
    public:
        wxTimeSpan start;
        wxTimeSpan length;
        property<wxString>* text;

        titSubtitle(titTimeline*, wxTimeSpan, wxTimeSpan, property<wxString>*);

        void PressEv(wxMouseEvent&);
        void DepressEv(wxMouseEvent&);
        void MotionEv(wxMouseEvent&);
        void LeaveEv(wxMouseEvent&);
        void OhFuckEv(wxMouseCaptureLostEvent&);

        void ButtonUpdate();
        void SetModelDataFromVisual();

        void Edit(wxWindow*);

        void PaintEv(wxPaintEvent&);

        bool operator<(const titSubtitle& other) const;

        struct comparator {
            bool operator()(titSubtitle*, titSubtitle*) const;
        };
};

#endif // TITSUBTITLE_H
