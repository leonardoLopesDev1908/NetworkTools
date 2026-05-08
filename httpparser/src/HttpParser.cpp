#ifndef HTTP_PARSER_CPP
#define HTTP_PARSER_CPP

#include "HttpParser.h"
#include <stdlib.h>
#include <string>

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

    size_t endHeaders = parseHeaders(raw, firstLine.size(), msg.headers);

    try {
        if (msg.headers.contains("Content-Length") &&
            stoi(msg.headers["Content-Length"]) > 0)
        {
            parseBody(raw, endHeaders, msg.headers["Content-Length"]);
        }
    }
    catch (std::invalid_argument& e)
    {
        std::cout << "Invalid argument: " << e.what() << "\n";
    }
    return msg;
}

std::string HttpParser::getFirstLine(std::string& raw)
{
    size_t end = raw.find("\r\n");
    std::string firstLine (raw.begin(), raw.begin() + end); 

    return firstLine;
}

size_t HttpParser::parseHeaders(std::string& raw, size_t start, 
        std::unordered_map<std::string, std::string>& headers)
{
    size_t end;
     
    while(true)
    {
        end = raw.find("\r\n", start);
        std::string line = raw.substr(start, end - start);
        
        if(line == "") //end of headers
            break;
        
        size_t endKey = line.find(":");
        headers.insert({
            line.substr(0, endKey), 
            line.substr(endKey + 1, line.size() - endKey - 1)
        });

        start = end + 2;
    }   
    return end;
}

std::string HttpParser::parseBody(std::string& raw, size_t start, std::string len)
{
    size_t size;
    sscanf(len.c_str(), "%zu", &size);
    
    std::string body(raw.data(), start, size);

    return body;
}

#endif
