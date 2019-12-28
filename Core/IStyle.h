#ifndef ISTYLE_H
#define ISTYLE_H

#include "Aproch.h"
#include <QJsonObject>

APROCH_NAMESPACE_BEGIN

class APROCH_EXPORT IStyle
{
    virtual void setStyle(const QJsonObject& obj) = 0;
};

APROCH_NAMESPACE_END

#endif // ISTYLE_H
