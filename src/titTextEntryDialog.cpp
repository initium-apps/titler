#include"titTextEntryDialog.h"

#include<wx/textfile.h>

titTextEntryDialog::titTextEntryDialog(wxWindow* parent, wxString msg, wxString caption) :
    wxTextEntryDialog(parent, msg, caption, wxEmptyString, wxTextEntryDialogStyle | wxTE_MULTILINE | wxTE_PROCESS_ENTER) {
    this->m_textctrl->Bind(wxEVT_CHAR, &titTextEntryDialog::Char, this);
}

wxTextCtrl* titTextEntryDialog::GetTextCtrl() const {
    return this->m_textctrl;
}

void titTextEntryDialog::Char(wxKeyEvent& ev) {
    if(ev.GetKeyCode() == WXK_RETURN && this->GetTextCtrl()->GetValue().EndsWith(wxTextFile::GetEOL())) {
        wxString val = this->GetTextCtrl()->GetValue();
        this->SetValue(val.Mid(0, val.Length() - 1));

        this->EndModal(wxID_OK);
    } else {
        ev.Skip();
    }
}
