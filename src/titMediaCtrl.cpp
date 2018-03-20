#include "titMediaCtrl.h"

#include"Titler.h"
#include"utils.h"

#include<algorithm>

titMediaCtrl::titMediaCtrl(wxWindow* parent) : wxMediaCtrl(parent, ID_MEDIA) {
    this->Bind(wxEVT_MEDIA_LOADED, &titMediaCtrl::LoadEv, this);
    this->Bind(wxEVT_MEDIA_PLAY, &titMediaCtrl::PlayEv, this);
    this->Bind(wxEVT_MEDIA_STOP, &titMediaCtrl::StopEv, this);
    this->Bind(wxEVT_MEDIA_PAUSE, &titMediaCtrl::StopEv, this);

    this->Bind(wxEVT_LEFT_DOWN, &titMediaCtrl::MouseDownEv, this);
    this->Bind(wxEVT_LEFT_UP, &titMediaCtrl::MouseUpEv, this);
    this->Bind(wxEVT_MOTION, &titMediaCtrl::MouseMoveEv, this);
    this->Bind(wxEVT_MOUSE_CAPTURE_LOST, &titMediaCtrl::OhFuckEv, this);
}

titMediaCtrl::~titMediaCtrl() {
}

void titMediaCtrl::LoadEv(wxMediaEvent& WXUNUSED(ev)) {
    this->Play();
}

void titMediaCtrl::PlayEv(wxMediaEvent& WXUNUSED(ev)) {
    if(!this->timer.IsRunning()) {
        this->timer.Start(10, wxTIMER_CONTINUOUS);
    }
}

void titMediaCtrl::StopEv(wxMediaEvent& WXUNUSED(ev)) {
}

void titMediaCtrl::MouseDownEv(wxMouseEvent& ev) {
    CaptureMouse();
    this->dragX = ev.GetX();
    this->currentlyDragging = true;
}

void titMediaCtrl::MouseUpEv(wxMouseEvent& WXUNUSED(ev)) {
    if(!this->currentlyDragging)
        return;
    ReleaseMouse();
    this->currentlyDragging = false;
}

void titMediaCtrl::MouseMoveEv(wxMouseEvent& ev) {
    if(this->currentlyDragging) {
        wxFileOffset goTo = this->Tell() - 5 * (ev.GetX() - this->dragX);
        this->Seek(clip(goTo, (wxFileOffset)0, this->Length() - 1), wxFromStart);

        this->dragX = ev.GetX();
    }
}

void titMediaCtrl::OhFuckEv(wxMouseCaptureLostEvent& ev) {
    this->currentlyDragging = false;
}
