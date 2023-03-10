#pragma once

#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;

class Frame {
protected:
    string command;
    map<string, string> headers;
    string body;

public:
    Frame(string, map<string, string>, string);

    explicit Frame(const string &);

    string toString() const;

    string getCommand() const;

    string getHeader(const string &);

    string getBody() const;

    static vector<string> split(const string &, char);

};

class ConnectFrame : public Frame {
public:
    ConnectFrame(string &acceptVersion, string &host, string &login, string &passcode);
};

class DisconnectFrame : public Frame {
public:
    DisconnectFrame(string &receiptId);
};

class SubscribeFrame : public Frame {
public:
    SubscribeFrame(string &destination, string &subscribeId, string &receiptId);
};

class UnsubscribeFrame : public Frame {
public:
    UnsubscribeFrame(string &subscribeId, string &receiptId);
};

class SendFrame : public Frame {
public:
    SendFrame(string &destination, string &receiptId, string &body);
};