#ifndef APROCHPORT_H
#define APROCHPORT_H

#include "Aproch.h"

APROCH_NAMESPACE_BEGIN

enum class EPortType
{
    None,
    Input,
    Output
};

using PortIndex = unsigned int;

class AprochPort
{
public:
    AprochPort()
        : Type(EPortType::None)
        , Index(INVALID_PORT_INDEX)
    {}

    AprochPort(EPortType type, PortIndex index)
        : Type(type)
        , Index(index)
    {}

    inline bool indexIsValid() { return Index != INVALID_PORT_INDEX; }

    inline bool portTypeIsValid() { return Type != EPortType::None; }

    inline static EPortType OppositePort(const EPortType &port)
    {
        if(EPortType::Input == port)
        {
            return EPortType::Output;
        }
        if(EPortType::Output == port)
        {
            return EPortType::Input;
        }
        return EPortType::None;
    }

public:
    EPortType Type;

    PortIndex Index;
};


APROCH_NAMESPACE_END

#endif // APROCHPORT_H
