/*
 * Built by Sylvestr Lozynskyy, heavily borrowed code from saurabhshri/simple-yet-powerful-srt-subtitle-parser-cpp on GitHub.
 */

#ifndef SRTPARSER_H
#define SRTPARSER_H

#include<wx/stream.h>
#include<wx/txtstrm.h>
#include<wx/tokenzr.h>
#include<wx/wxchar.h>

static inline void split(wxString what, wxString delim, std::vector<wxString>& vec) {
    wxString::size_type i;

    while((i = what.Find(delim)) != wxString::npos) {
        vec.push_back(what.substr(0, i));
        what = what.substr(i + delim.size());
    }

    vec.push_back(what);
}

struct SRTSub {
    wxString text;
    wxTimeSpan start;
    wxTimeSpan length;
};

/**
* Only supports sorted subtitles.
 */
class SRTParser {
    private:
        std::vector<SRTSub> subs;
    public:
        void parse(wxInputStream&);
        const std::vector<SRTSub>& getSubtitles();
};

inline void SRTParser::parse(wxInputStream& is) {
    wxTextInputStream ts(is, " \t", wxMBConvUTF8());

    while(is.IsOk() && !is.Eof()) {
        ts.ReadLine(); // ignore the IDs

        wxString startAndEndStr = ts.ReadLine();
        std::vector<wxString> startAndEnd;
        split(startAndEndStr, "-->", startAndEnd);

        if(startAndEnd.size() == 1)
            break;

        long h, m, s, ms;

        std::vector<wxString> start;
        split(startAndEnd[0], ":", start);
        wxString sms = start.back();
        start.pop_back();
        split(sms, ",", start);

        start[0].ToLong(&h);
        start[1].ToLong(&m);
        start[2].ToLong(&s);
        start[3].ToLong(&ms);
        wxTimeSpan startSpan(h, m, s, ms);

        std::vector<wxString> end;
        split(startAndEnd[1], ":", end);
        sms = end.back();
        end.pop_back();
        split(sms, ",", end);

        end[0].ToLong(&h);
        end[1].ToLong(&m);
        end[2].ToLong(&s);
        end[3].ToLong(&ms);
        wxTimeSpan endSpan(h, m, s, ms);

        wxString msg = "";
        wxString line;
        while(!(line = ts.ReadLine()).Trim(false).Trim(true).IsEmpty())
            msg += line;

        this->subs.push_back(SRTSub{msg, startSpan, wxTimeSpan::Milliseconds(endSpan.GetValue() - startSpan.GetValue())});
    }
}

inline const std::vector<SRTSub>& SRTParser::getSubtitles() {
    return this->subs;
}

#endif //SRTPARSER_H
