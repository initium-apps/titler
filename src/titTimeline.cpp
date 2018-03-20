#include "titTimeline.h"

#include<algorithm>

titTimeline::titTimeline(wxWindow* parent, titMediaCtrl* media) : wxScrolledWindow(parent, wxID_ANY), media(media) {
    this->SetScrollRate(5, 0);

    this->Bind(wxEVT_SIZE, &titTimeline::ResizeEv, this);
    this->Bind(wxEVT_MOUSEWHEEL, &titTimeline::ScrollEv, this);

//    insertSubtitle(wxTimeSpan((wxLongLong) 1000), wxTimeSpan((wxLongLong) 500), new property<wxString>("Subtitle one"));
//    insertSubtitle(wxTimeSpan((wxLongLong) 2000), wxTimeSpan((wxLongLong) 500), new property<wxString>(wxString::FromUTF8("титр другий")));
}

void titTimeline::ResetHandle() {
    if(this->handle != NULL) {
        this->RemoveChild(this->handle);
        this->handle->Destroy();
    }

    this->handle = new titHandle(this, media);
    this->handle->SetSize(wxSize(4, wxSystemSettings::GetMetric (wxSYS_SCREEN_Y)));
}

titSubtitle* titTimeline::insertSubtitle(wxTimeSpan start, wxTimeSpan length, property<wxString>* text) {
    if(text->get().IsEmpty())
        return NULL;

    titSubtitle* button = new titSubtitle(this, start, length, text);

    this->subs.push_back(button);
    std::sort(this->subs.begin(), this->subs.end(), titSubtitle::comparator());

    this->ResetHandle();

    UpdateVirtualSize();

    return button;
}

void titTimeline::CreateNSubtitle(wxString text) {
    this->nSubtitle = this->insertSubtitle(wxTimeSpan::Milliseconds(media->Tell()), wxTimeSpan::Milliseconds(1), new property<wxString>(text));
}

void titTimeline::UpdateNSubtitle() {
    if(this->nSubtitle != NULL) {
        this->nSubtitle->length = wxTimeSpan::Milliseconds(this->media->Tell() - this->nSubtitle->start.GetValue());
        this->nSubtitle->ButtonUpdate();
    }
}

titSubtitle* titTimeline::FinishNSubtitle() {
    titSubtitle* ret = this->nSubtitle;

    this->nSubtitle = NULL;

    return ret;
}

void titTimeline::ResizeEv(wxSizeEvent& WXUNUSED(ev)) {
    for(auto& btn : this->subs) {
        btn->ButtonUpdate();
    }
    Refresh();
}

void titTimeline::ScrollEv(wxMouseEvent& ev) {
    this->zoom *= ev.GetWheelRotation() < 0 ? 1.0 / 1.1 : 1.1;
    for(auto& btn : this->subs) {
        btn->ButtonUpdate();
    }

    UpdateVirtualSize();
}

titSubtitle* titTimeline::DetermineCurrentSubtitle(double curmillis) {
    wxTimeSpan cur = wxTimeSpan::Milliseconds((wxLongLong) curmillis);
    for(auto& sub : this->subs) {
        wxTimeSpan end(sub->start.GetValue() + sub->length.GetValue());
        if(sub->start.IsShorterThan(cur) && end.IsLongerThan(cur)) {
            return sub;
        }
    }
    return NULL;
}

void titTimeline::UpdateVirtualSize() {
    this->SetVirtualSize(wxSize(this->media->Length() * this->zoom, this->GetSize().y));
}

void titTimeline::Reset() {
    for(auto& sub : this->subs) {
        this->RemoveChild(sub);
        sub->Destroy();
    }
    this->subs.clear();

    this->ResetHandle();
}
