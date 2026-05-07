#ifndef MESSAGE_H
#define MESSAGE_G

#include <iostream>
#include <unordered_map>
#include <variant>

enum class Direction {Inbound, Outbound};

struct RequestLine
{
    std::string method;
    std::string path;
    std::string version;
};

struct StatusLine
{
    std::string version;
    int statusCode;
    std::string statusText;
};

struct Message
{
    Direction direction;
    std::variant<StatusLine, RequestLine> startLine;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    bool isRequest()
    {
        return std::holds_alternative<RequestLine>(startLine);
    }

    RequestLine request()
    {
        return std::get<RequestLine>(startLine);
    }

    StatusLine respose()
    {
        return std::get<StatusLine>(startLine);
    }
};

#endif 