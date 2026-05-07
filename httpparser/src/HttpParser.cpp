#ifndef HTTP_PARSER_CPP
#define HTTP_PARSER_CPP

#include "HttpParser.h"
#include <stdlib.h>

Message HttpParser::parse(std::string& raw, Direction direction)
{
    Message msg;
    msg.direction = direction;

    std::string firstLine = getFirstLine(raw);
    
    if(firstLine.substr(0, 5) == "HTTP/")
    {
        StatusLine sl;
        sscanf(firstLine.c_str(), "%s %d %[^\r\n]", 
                sl.version.data(), &sl.statusCode, sl.statusText.data());
        msg.startLine = sl;
    }
    else
    {
        RequestLine rl;
        sscanf(firstLine.c_str(), "%s %d %s", 
            rl.method.data(), rl.path.data(), rl.version.data());
        msg.startLine = rl;
    }

    parseHeaders(raw, firstLine.size(), msg.headers);
    //if(msg.headers["Content-Length"] > 0)
    //   msg.body = parseBody(raw);

    return msg;
}

std::string getFirstLine(std::string& raw)
{
    size_t end = raw.find("\r\n");
    std::string firstLine (raw.begin(), raw.begin() + end); 

    return firstLine;
}

void HttpParser::parseHeaders(std::string& raw, size_t start, 
        std::unordered_map<std::string, std::string>& headers)
{
    size_t end;

    while(true)
    {
        end = raw.find("\r\n");
        std::string line = raw.substr(start, end);
        
        if(line == "\r\n")
            break;
        
        size_t endKey = line.find(":");
        headers.insert({
            line.substr(start, endKey), line.substr(endKey, end)
        });

        start = end + 4;
    }   
}

std::string HttpParser::parseBody(std::string& raw)
{

}

#endif
