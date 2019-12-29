#ifndef APROCHPORT_H
#define APROCHPORT_H

#include "Aproch.h"

APROCH_NAMESPACE_BEGIN

/// 端口类型，分为输入和输出
enum class EPortType
{
    None,
    Input,
    Output
};

/// 端口索引，对每一种端口类型（EPortType），都是从上到下，从0开始编号
using PortIndex = unsigned int;

class APort
{
public:
    APort()
        : Type(EPortType::None)
        , Index(INVALID_PORT_INDEX)
    {}

    APort(EPortType type, PortIndex index)
        : Type(type)
        , Index(index)
    {}

    inline bool indexIsValid() { return Index != INVALID_PORT_INDEX; }

    inline bool portTypeIsValid() { return Type != EPortType::None; }

    inline static EPortType OppositePort(const EPortType &port)
    {
        if (EPortType::Input == port)
        {
            return EPortType::Output;
        }
        if (EPortType::Output == port)
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
