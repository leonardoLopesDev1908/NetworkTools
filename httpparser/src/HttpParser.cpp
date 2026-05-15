#ifndef HTTP_PARSER_CPP
#define HTTP_PARSER_CPP

#include "HttpParser.h"

#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>

Message HttpParser::parse(std::string& raw, Direction direction)
{
    //std::ofstream logFile("C:\\Users\\teste\\Desktop\\logfile.txt", std::ios::app);
    //if (logFile.is_open())
    //{
    //    logFile << "Raw message\n";
    //    logFile << raw;
    //    logFile << "\n";
    //}

    Message msg{};
    msg.direction = direction;

    std::string firstLine = getFirstLine(raw);
    
    if(firstLine.substr(0, 5) == "HTTP/")
    {
        StatusLine sl;
        std::istringstream iss(firstLine);
        iss >> sl.version >> sl.statusCode >> sl.version;

        msg.startLine = sl;

        //Log
       /* logFile << "Parsed message\n";
        logFile << msg.response().version << " ";
        logFile << msg.response().statusText << " ";
        logFile << msg.response().version << " ";
        logFile << "\n";*/
    }
    else
    {
        RequestLine rl;
        std::istringstream iss(firstLine);
        iss >> rl.method >> rl.path >> rl.version;
        msg.startLine = rl;

        //Log
        //logFile << "Parsed message\n";
        //logFile << msg.request().method << " ";
        //logFile << msg.request().path << " ";
        //logFile << msg.request().version << " ";
        //logFile << "\n";
    }

    size_t endHeaders = parseHeaders(raw, firstLine.size() + 2, msg.headers);

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

   /* for (auto& [key, value] : msg.headers)
        logFile << key << ": " << value << "\n";*/

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
        if (end == std::string::npos)
            break;

        std::string line = raw.substr(start, end - start);
        
        if(line == "") //end of headers
            break;
        
        size_t endKey = line.find(":");
        headers.insert({
            line.substr(0, endKey), 
            line.substr(endKey + 2)
        });

        start = end + 2;
    }   
    return end + 2;
}

//The rules for determining when a message body is present in an HTTP/1.1 message differ for requests and responses.

//The presence of a message body in a request is signaled by a Content-Length or Transfer-Encoding header field. Request message framing is independent of method semantics.

std::string HttpParser::parseBody(std::string& raw, size_t start, std::string len)
{
    size_t size;
    
    std::istringstream iss(len);
    iss >> size;
    
    std::string body(raw.data(), start, size);

    return body;
}

#endif
