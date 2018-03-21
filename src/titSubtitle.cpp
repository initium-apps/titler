#include "titSubtitle.h"

#include"titTimeline.h"
#include"utils.h"
#include"titTextEntryDialog.h"

titSubtitle::titSubtitle(titTimeline* timeline, wxTimeSpan start, wxTimeSpan length, property<wxString>* text) : wxPanel(timeline, wxID_ANY), timeline(timeline), start(start), length(length), text(text) {
    ButtonUpdate();

    text->listen([this](wxString overriden) {
        this->childText->SetLabel(overriden);
    });
	
	this->SetBackgroundColour(wxGREY_BRUSH->GetColour());

    this->childText = new wxStaticText(this, wxID_ANY, text->get(), wxDefaultPosition, wxDefaultSize, wxTRANSPARENT_WINDOW);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(this->childText);
    this->SetSizer(sizer);

    this->Bind(wxEVT_LEFT_DOWN, &titSubtitle::PressEv, this);
    this->Bind(wxEVT_RIGHT_DOWN, &titSubtitle::PressEv, this);
    this->Bind(wxEVT_LEFT_UP, &titSubtitle::DepressEv, this);
    this->Bind(wxEVT_RIGHT_UP, &titSubtitle::DepressEv, this);
    this->Bind(wxEVT_MOTION, &titSubtitle::MotionEv, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &titSubtitle::LeaveEv, this);
    this->Bind(wxEVT_MOUSE_CAPTURE_LOST, &titSubtitle::OhFuckEv, this);

    this->Bind(wxEVT_PAINT, &titSubtitle::PaintEv, this);
}

void titSubtitle::PressEv(wxMouseEvent& ev) {
    switch(ev.GetButton()) {
        case wxMOUSE_BTN_LEFT:
            CaptureMouse();
            this->dragX = ev.GetX();
            this->currentlyDragging = true;
            break;
        case wxMOUSE_BTN_RIGHT:
            this->timeline->RemoveChild(this);
            this->Destroy();
            removeFromVector(this->timeline->subs, this);
            break;
    }
}

void titSubtitle::DepressEv(wxMouseEvent& WXUNUSED(ev)) {
    if(!this->currentlyDragging) {
        Edit(this);
    } else {
        ReleaseMouse();
    }

    this->currentlyDragging = false;
    this->dragLeft = false;
    this->dragRight = false;
}

void titSubtitle::MotionEv(wxMouseEvent& ev) {
    if(!this->currentlyDragging) {
        wxCoord x = ev.GetX();
        if(x < TITLER_SUBTITLE_MOUSE_SNAP_EDGE_THINGY_MINIMUM_AMOUNT_TO_DO_IT) {
            this->dragLeft = true;
            wxSetCursor(wxCURSOR_POINT_LEFT);
        } else if(x >= GetSize().x - TITLER_SUBTITLE_MOUSE_SNAP_EDGE_THINGY_MINIMUM_AMOUNT_TO_DO_IT) {
            this->dragRight = true;
            wxSetCursor(wxCURSOR_POINT_RIGHT);
        } else {
            this->dragLeft = false;
            this->dragRight = false;
            wxSetCursor(wxCURSOR_DEFAULT);
        }
    } else {
        wxSize size = this->GetSize();

        if(this->dragLeft) {
            wxPoint mousecoord = wxGetMousePosition();
            mousecoord.x -= this->dragX;

            wxPoint client = this->timeline->ScreenToClient(mousecoord);
            client.x = clip(client.x, this->timeline->CalcScrolledPosition(wxPoint(0, 0)).x, this->timeline->GetSize().x);
            client.y = 0;

            this->Move(client);
            size.x += this->dragX - ev.GetX();
            this->SetSize(size);
        } else if(this->dragRight) {
            size.x += ev.GetX() - this->dragX;
            this->dragX = ev.GetX();
            this->SetSize(size);
        } else {
            wxPoint mousecoord = wxGetMousePosition();
            mousecoord.x -= this->dragX;

            wxPoint client = this->timeline->ScreenToClient(mousecoord);
            client.x = clip(client.x, this->timeline->CalcScrolledPosition(wxPoint(0, 0)).x, this->timeline->GetSize().x);
            client.y = 0;

            this->SetPosition(client);
        }

        SetModelDataFromVisual();
    }
}

void titSubtitle::LeaveEv(wxMouseEvent& WXUNUSED(ev)) {
    wxSetCursor(wxCURSOR_DEFAULT);
}

void titSubtitle::OhFuckEv(wxMouseCaptureLostEvent& WXUNUSED(ev)) {
    this->currentlyDragging = false;
    this->dragLeft = false;
    this->dragRight = false;
}

void titSubtitle::SetModelDataFromVisual() {
    this->start = wxTimeSpan(0, 0, 0, this->timeline->CalcUnscrolledPosition(this->GetPosition()).x / this->timeline->zoom);
    this->length = wxTimeSpan(0, 0, 0, this->GetSize().x / this->timeline->zoom);
}

void titSubtitle::ButtonUpdate() {
    this->SetPosition(this->timeline->CalcScrolledPosition(wxPoint(this->start.GetValue().ToLong() * this->timeline->zoom, 0)));
    this->SetSize(this->length.GetValue().ToLong() * this->timeline->zoom, this->timeline->GetSize().y);
}

void titSubtitle::Edit(wxWindow* parent) {
    titTextEntryDialog dlg(parent, wxEmptyString, wxT("Replace Subtitle Text"));
    dlg.SetValue(this->text->get());
    dlg.GetTextCtrl()->SelectAll();

    if(dlg.ShowModal() == wxID_OK) {
        this->text->set(dlg.GetValue());
    }
}

void titSubtitle::PaintEv(wxPaintEvent& WXUNUSED(ev)) {
    wxPaintDC dc(this);

    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawRectangle(wxPoint(0, 0), this->GetSize());

    dc.SetBrush(*wxGREY_BRUSH);
    dc.SetPen(*wxGREY_PEN);
    dc.DrawRectangle(wxPoint(1, 0), this->GetSize() - wxSize(2, 0));
}

// used for sorting
bool titSubtitle::operator<(const titSubtitle& other) const {
    return start < other.start;
}

bool titSubtitle::comparator::operator()(titSubtitle* left, titSubtitle* right) const {
    return (*left) < (*right);
}
