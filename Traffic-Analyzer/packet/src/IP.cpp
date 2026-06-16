#include "IP.h"

const std::string& IP::getSource() const { return src; }
const std::string& IP::getDestiny() const { return dst; }

TranportProtocol IP::getProtocol() const { return transport; }
uint16_t IP::getPayloadLen() const { return payloadLen; }


explicit IPv4::IPv4(const u_char* data)
{ 
    ipHdr = data;
    char transportStr;

	//from half of the first byte
	ipHdrLen = (data[0] << 2) >> 2; 
	
	//from byte 3 to byte 4
    memcpy(&payloadLen, data + 3, 2);
    payloadLen -= ipHdrLen;	

	//10th byte
    transportStr = data[9];

	//All the fourth line (4 bytes)
    memcpy(&src, data + 12, 4);

	//All the fifth line (4 bytes)
	memcpy(&dst, data + 16, 4);

}

uint16_t IPv4::getSrcPort() const override { return srcPort; }

uint16_t IPv4::getDestPort() const override { return dstPort; }

void IPv4::handleTcp() override
{

}

void IPv4::handleUdp() override
{

}

void IPv4::handleIcmp() override
{

}

void IPv4::handleIcmpv6() override
{

}

void IPv4::handleIgmp() override
{

}
