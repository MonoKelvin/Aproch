#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include <QJsonObject>

class ISerializable
{
public:
    virtual ~ISerializable() = default;

    virtual QJsonObject save() const = 0;

    virtual void restore(const QJsonObject &) {}
};

#endif // ISERIALIZABLE_H
