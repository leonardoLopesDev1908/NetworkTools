#ifndef IP_H
#define IP_H

#include "Packet.h"
#include "Platform.h"

#include <string>

class IP
{
protected:
    virtual void handleTcp() = 0;
    virtual void handleUdp() = 0;
    virtual void handleIcmp() = 0;
    virtual void handleIcmp6() = 0;
    virtual void handleIgmp() = 0;
    
    uint16_t payloadLen = 0;
    TransportProtocol transport = TransportProtocol::UNKNOWN;
    std::string src;
    std::string dst;

public:
    const std::string& getSource() const;
    const std::string& getDestiny() const;

    virtual uint16_t getSourcePort() = 0;
    virtual uint16_t getDestinyPort() = 0;

    TransportProtocol getProtocol() const;
    uint16_t getPayloadLen() const;
    
    const uint8_t *payloadPtr = nullptr;
    const uint8_t *getPayloadPtr() const { return payloadPtr; }

    virtual ~IP() = default;
};

class IPv4 : public IP
{
private:
	const ip* ipHdr = nullptr;
	uint8_t ipHdrLen = 0;
	uint16_t srcPort = 0;
	uint16_t destPort = 0;

protected:
	void handleTcp() override;
	void handleUdp() override;
	void handleIcmp() override;
	void handleIcmp6() override;
	void handleIgmp() override;

public:
	uint16_t getSourcePort() override;
	uint16_t getDestinyPort() override;

	explicit IPv4(const uint8_t* data);

};

class IPv6 : public IP
{
protected:
	void handleTcp() override;
	void handleUdp() override;
	void handleIcmp() override;
	void handleIcmp6() override;
	void handleIgmp() override;

private:
    const ip6_hdr *ipHdr = nullptr;
	int ipHdrLen = 40;

	in6_addr ipSource = {};
	in6_addr ipDest = {};

	uint16_t srcPort = 0;
	uint16_t destPort = 0;

	const uint8_t *ptr = nullptr;

public:
	uint16_t getSourcePort() override;
	uint16_t getDestinyPort() override;

	explicit IPv6(const u_char *data);
};


#endif
