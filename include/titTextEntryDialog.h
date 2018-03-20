#ifndef TITTEXTENTRYDIALOG_H
#define TITTEXTENTRYDIALOG_H

#include<wx/textdlg.h>

class titTextEntryDialog : public wxTextEntryDialog {
    private:
        void Char(wxKeyEvent&);
    public:
        titTextEntryDialog(wxWindow*, wxString, wxString);

        wxTextCtrl* GetTextCtrl() const;
};

#endif // TITTEXTENTRYDIALOG_H
