#include"titHandle.h"

#include"titTimeline.h"
#include"utils.h"

titHandle::titHandle(titTimeline* timeline, titMediaCtrl* media) : titBlackPanel(timeline), timeline(timeline), media(media) {
    this->Bind(wxEVT_LEFT_DOWN, &titHandle::PressEv, this);
    this->Bind(wxEVT_LEFT_UP, &titHandle::DepressEv, this);
    this->Bind(wxEVT_MOTION, &titHandle::MotionEv, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &titHandle::LeaveEv, this);
    this->Bind(wxEVT_MOUSE_CAPTURE_LOST, &titHandle::OhFuckEv, this);
}

void titHandle::PressEv(wxMouseEvent& ev) {
    CaptureMouse();
    this->dragX = ev.GetX();
    this->currentlyDragging = true;
}

void titHandle::DepressEv(wxMouseEvent& WXUNUSED(ev)) {
    this->currentlyDragging = false;
    wxSetCursor(wxCURSOR_DEFAULT);
    ReleaseMouse();
}

void titHandle::MotionEv(wxMouseEvent& ev) {
    if(!this->currentlyDragging) {
        wxSetCursor(wxCURSOR_SIZEWE);
    } else {
        this->media->Seek(this->timeline->CalcUnscrolledPosition(GetPosition()).x / this->timeline->zoom - (this->dragX - ev.GetX()) / this->timeline->zoom, wxFromStart);
    }
}

void titHandle::LeaveEv(wxMouseEvent& WXUNUSED(ev)) {
    wxSetCursor(wxCURSOR_DEFAULT);
}

void titHandle::OhFuckEv(wxMouseCaptureLostEvent& WXUNUSED(ev)) {
    this->currentlyDragging = false;
}
