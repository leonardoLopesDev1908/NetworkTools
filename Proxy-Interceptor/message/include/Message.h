#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <unordered_map>
#include <variant>

// According with RFC 7230 https://datatracker.ietf.org/doc/html/rfc7230#section-3.1.1
/*
   "An HTTP message can be either a request from client to server or a
   response from server to client.  Syntactically, the two types of
   message differ only in the start-line, which is either a request-line
   (for requests) or a status-line (for responses)."
*/

enum class Type
{
    Inbound,
    Outbound
};

struct RequestLine
{
    std::string method;
    std::string path;
    std::string version;
};

struct StatusLine
{
    std::string version;
    int statusCode{};
    std::string statusText;
};

struct Message
{
    Type type{};
    std::variant<StatusLine, RequestLine> startLine;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    Message() noexcept = default;

    Message(const Message& other) noexcept = default;
    Message& operator=(const Message& other) noexcept = default;

    Message(Message&& other) noexcept = default;
    Message& operator=(Message&& other) noexcept = default;

    bool isRequest() const { return std::holds_alternative<RequestLine>(startLine); }

    RequestLine request() { return std::get<RequestLine>(startLine); }

    StatusLine response() { return std::get<StatusLine>(startLine); }
};

#endif