#ifndef TITHANDLE_H
#define TITHANDLE_H

#include"titBlackPanel.h"
#include"titMediaCtrl.h"

// forwards declaration
class titTimeline;

class titHandle : public titBlackPanel {
    private:
        titTimeline* timeline;
        titMediaCtrl* media;

        bool currentlyDragging = false;
        wxCoord dragX;
    public:
        titHandle(titTimeline*, titMediaCtrl*);

        void PressEv(wxMouseEvent&);
        void DepressEv(wxMouseEvent&);
        void MotionEv(wxMouseEvent&);
        void LeaveEv(wxMouseEvent&);
        void OhFuckEv(wxMouseCaptureLostEvent&);
};

#endif // TITHANDLE_H
