#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "Message.h"

#include <iostream>
#include <vector>


/* RFC 9112 HTTP/1.1 https://datatracker.ietf.org/doc/html/rfc9112
* 
* The normal procedure for parsing an HTTP message is 
*		- to read the start - line into a structure, 
*		- read each header field line into a hash table by 
*		  field name until the empty line, 
*		- and then use the parsed data to determine if a 
*		  message body is expected.
* If a message body has been indicated,
* then it is read as a stream until an amount of
* octets equal to the message body length is read 
* or the connection is closed.
*/

class HttpParser
{
public:

    Message parse(std::string& raw, Direction direction);

private:
    std::string getFirstLine(std::string& raw);
    void parseHeaders(
        std::string& raw, 
        size_t start,
        std::unordered_map<std::string, std::string>& headers);
    std::string parseBody(std::string& raw);
};


#endif 