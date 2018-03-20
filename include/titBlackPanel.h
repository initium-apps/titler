#ifndef TITBLACKPANEL_H
#define TITBLACKPANEL_H

#include <wx/wx.h>

class titBlackPanel : public wxPanel
{
    public:
        titBlackPanel(wxWindow*);
        virtual ~titBlackPanel();

        void PaintEv(wxPaintEvent&);
};

#endif // TITBLACKPANEL_H
