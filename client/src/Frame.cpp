#include <utility>

#include "../include/Frame.h"

Frame::Frame(string command, map<string, string> headers, string body) : command(std::move(command)),
                                                                         headers(std::move(headers)),
                                                                         body(std::move(body)) {}

Frame::Frame(const string &frame) {
    vector<string> lines = split(frame, '\n');

    command = lines[0];
    headers = map<string, string>{};

    unsigned int startOfBody = 2;

    for (unsigned int i = 1; i < lines.size(); i++) {
        if (lines[i].empty()) {
            startOfBody = i + 1;
            break;
        }
        vector<string> header = split(lines[i], ':');
        headers[header[0]] = header[1];
    }

    body = "";
    for (unsigned int i = startOfBody; i < lines.size(); i++) {
        body += lines[i];
        if (i != lines.size() - 1) {
            body += '\n';
        }
    }
}


string Frame::toString() const {
    string frame = command + '\n';

    for (auto &header : headers) {
        frame += header.first + ":" + header.second + '\n';
    }

    frame += '\n' + body + '\n' + endOfLine;

    return frame;
}

string Frame::getCommand() const {
    return command;
}

map<string, string> Frame::getHeaders() const {
    return headers;
}

string Frame::getHeader(const string &header) {
    auto it = headers.find(header);
    if (it != headers.end())
        return it->second;
    return "";
}

string Frame::getBody() const {
    return body;
}

vector<string> Frame::split(const string &stringToSplit, char splitChar) {
    vector<string> output;
    auto ss = std::stringstream{stringToSplit};
    for (string line; getline(ss, line, splitChar);)
        output.push_back(line);
    return output;
}


ConnectFrame::ConnectFrame(string &acceptVersion, string &host, string &login, string &passcode) : Frame("CONNECT", map<string, string>{}, ""){
    headers["accept-version"] = acceptVersion;


}