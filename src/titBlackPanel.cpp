#include "titBlackPanel.h"

titBlackPanel::titBlackPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
	this->SetBackgroundColour(*wxBLACK);

    this->Bind(wxEVT_PAINT, &titBlackPanel::PaintEv, this);
}

titBlackPanel::~titBlackPanel() {
}

void titBlackPanel::PaintEv(wxPaintEvent& WXUNUSED(ev)) {
    wxPaintDC dc(this);

    dc.SetBrush(*wxBLACK);
    dc.DrawRectangle(wxPoint(0, 0), this->GetSize());
}
