#ifndef ALERTS_H
#define ALERTS_H

#include <iostream>

enum class AnomalyType
{
    SynFlood,
    PortScan,
    HighVolume,
    InvalidFlags,
    Beacon,
    UnknownPort
};

enum class Risk
{
    Low,
    Medium,
    High
};

struct Alert
{
    AnomalyType type;
    Risk severity;
    std::string ipSource;
    std::string description;

    Alert(AnomalyType anomaly, Risk severity, const std::string& src, const std::string& desc)
    {
        this->type = anomaly;
        this->severity = severity;
        this->ipSource = src;
        this->description = desc;
    }
};

#endif