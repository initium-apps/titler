#ifndef TITMEDIACTRL_H
#define TITMEDIACTRL_H

#include<wx/mediactrl.h>
#include<wx/timer.h>

class titMediaCtrl : public wxMediaCtrl {
    private:
        bool currentlyDragging = false;
        int dragX;

        void MouseDownEv(wxMouseEvent&);
        void MouseUpEv(wxMouseEvent&);
        void MouseMoveEv(wxMouseEvent&);
        void OhFuckEv(wxMouseCaptureLostEvent&);
    public:
        wxTimer timer;

        titMediaCtrl(wxWindow*);
        virtual ~titMediaCtrl();

        void LoadEv(wxMediaEvent&);
        void PlayEv(wxMediaEvent&);
        void StopEv(wxMediaEvent&);
};

#endif // TITMEDIACTRL_H
