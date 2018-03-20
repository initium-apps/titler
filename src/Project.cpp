#include"Project.h"

#include<sstream>
#include<iomanip>
#include<string>
#include"srtparser.h"

static void trim(std::string& s) {
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(' ') + 1);
}

static void splitString(std::string str, std::string delim, bool doTrim, std::vector<std::string>& vec) {
    std::string::size_type p;

    while((p = str.find(delim)) != std::string::npos) {
        if(doTrim) {
            std::string cop = str.substr(0, p);
            trim(cop);
            vec.push_back(cop);
        } else {
            vec.push_back(str.substr(0, p));
        }
        str = str.substr(p + delim.length());
    }

    if(doTrim) {
        std::string cop = str;
        trim(cop);
        vec.push_back(cop);
    } else {
        vec.push_back(str);
    }
}

Project::Project(wxString locPath) : subsLoc(locPath), mediaLoc(locPath) {
}

void Project::Save(wxFileOutputStream& fos, titTimeline* timeline) {
    std::stringstream str;
    unsigned long nextTitle = 1;

    for(auto& subtitle : timeline->subs) {
        str << nextTitle++ << "\n";

        str << subtitle->start.Format("%H:%M:%S,%l").ToUTF8();
        str << " --> ";
        wxTimeSpan copy(subtitle->length);
        copy += subtitle->start;
        str << copy.Format("%H:%M:%S,%l").ToUTF8();
        str << "\n";

        str << subtitle->text->get().ToStdString();
        str << "\n\n";
    }

    std::string s = str.str();
    fos.Write(s.c_str(), s.size());
}

void Project::Open(wxString filename, titTimeline* timeline) {
    wxFileInputStream fis(filename);

    SRTParser parser;
    parser.parse(fis);

    std::vector<SRTSub> subs = parser.getSubtitles();
    for(auto& sub : subs) {
        timeline->insertSubtitle(sub.start, sub.length, new property<wxString>(sub.text));
    }
}
