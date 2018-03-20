#include "titMenuBar.h"

#include"Titler.h"

titMenuBar::titMenuBar(Titler* titler) : wxMenuBar(), titler(titler) {
    this->file = new wxMenu();
    this->file->Append(wxID_OPEN, _("&Open"));
    this->file->Append(wxID_SAVE, _("&Save"));
    this->file->AppendSeparator();
    this->file->Append(wxID_EXIT, _("&Quit"));
    this->Append(file, _("&File"));

    this->help = new wxMenu();
    this->help->Append(wxID_ABOUT, _("&About"));
    this->Append(help, _("&Help"));
}

titMenuBar::~titMenuBar() {
}
