#ifndef TITMENUBAR_H
#define TITMENUBAR_H

#include<wx/wx.h>

class Titler; // forward declaring

class titMenuBar : public wxMenuBar {
    private:
        Titler* titler;

        wxMenu* file;
        wxMenu* help;
    public:
        titMenuBar(Titler*);
        virtual ~titMenuBar();
};

#endif // TITMENUBAR_H
