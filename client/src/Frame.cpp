#include <utility>

#include "../include/Frame.h"
#include "../include/ConnectionHandler.h"

Frame::Frame(string command, map<string, string> headers, string body) : command(std::move(command)),
                                                                         headers(std::move(headers)),
                                                                         body(std::move(body)) {}

Frame::Frame(const string &frame) : command(""), headers(map<string, string>{}), body("") {
    vector<string> lines = split(frame, '\n');
    command = lines[0];

    int startOfBody = -1;

    for (int i = 1; i < (int) lines.size(); i++) {
        if (lines[i].empty()) {
            startOfBody = i + 1;
            break;
        }
        vector<string> header = split(lines[i], ':');
        headers[header[0]] = header[1];
    }
    if (startOfBody != -1) {
        for (int i = startOfBody; i < (int) lines.size(); i++) {
            body += lines[i];
            if (i != (int) lines.size() - 1)
                body += '\n';
        }
    }

}


string Frame::toString() const {
    string frame = command + '\n';

    for (auto &header: headers)
        frame += header.first + ":" + header.second + '\n';
    frame += '\n'; // gap
    frame += body.empty() ? "" : body;

    return frame;
}

string Frame::getCommand() const {
    return command;
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


ConnectFrame::ConnectFrame(string &acceptVersion, string &host, string &login, string &passcode) : Frame("CONNECT",
                                                                                                         map<string, string>{},
                                                                                                         "") {
    headers["accept-version"] = acceptVersion;
    headers["host"] = host;
    headers["login"] = login;
    headers["passcode"] = passcode;
}

DisconnectFrame::DisconnectFrame(string &receiptId) : Frame("DISCONNECT", map<string, string>{}, "") {
    headers["receipt"] = receiptId;
}

SubscribeFrame::SubscribeFrame(string &destination, string &subscribeId, string &receiptId) : Frame("SUBSCRIBE",
                                                                                                    map<string, string>{},
                                                                                                    "") {
    headers["destination"] = destination;
    headers["id"] = subscribeId;
    headers["receipt"] = receiptId;
}

UnsubscribeFrame::UnsubscribeFrame(string &subscribeId, string &receiptId) : Frame("UNSUBSCRIBE", map<string, string>{},
                                                                                   "") {
    headers["id"] = subscribeId;
    headers["receipt"] = receiptId;
}

SendFrame::SendFrame(string &destination, string &receiptId, string &body) : Frame("SEND", map<string, string>{},
                                                                                   body) {
    headers["destination"] = destination;
    headers["receipt"] = receiptId;
}

