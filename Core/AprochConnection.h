#ifndef APROCHCONNECTION_H
#define APROCHCONNECTION_H

#include "Utilities.h"
#include "AprochPort.h"
#include "ISerializable.h"

#include <QtCore/QUuid>
#include <QtCore/QVariant>

class QPointF;

APROCH_NAMESPACE_BEGIN

class AprochNode;
class INodeData;
class AprochConnectionGraphicsObject;

struct SConnectionState
{

};

class APROCH_EXPORT AprochConnection : public QObject, public ISerializable
{
    Q_OBJECT
public:
    /**
     * New Connection is attached to the port of the given Node.
     * The port has parameters (portType, portIndex).
     * The opposite connection end will require another port.
     */
    AprochConnection(EPortType portType, AprochNode &node, PortIndex portIndex);
    AprochConnection(AprochNode &nodeIn, PortIndex portIndexIn, AprochNode &nodeOut, PortIndex portIndexOut, TypeConverter converter = TypeConverter{});

    AprochConnection(const AprochConnection &) = delete;
    AprochConnection operator=(const AprochConnection &) = delete;

    ~AprochConnection();

    // ISerializable interface
public:
    QJsonObject save() const;

public:
    QUuid getId() const{ return mUuid; }

    // Remembers the end being dragged.
    // Invalidates Node address.
    // Grabs mouse.
    void setRequiredPort(EPortType portType);
    inline EPortType getRequiredPort() const { return mRequiredPort; }

    inline void setNoRequiredPort() { mRequiredPort = EPortType::None; }

    void setGraphicsObject(QScopedPointer<AprochConnectionGraphicsObject> &&graphics);

    /// Assigns a node to the required port.
    /// It is assumed that there is a required port, no extra checks
    void setNodeToPort(AprochNode &node, EPortType portType, PortIndex portIndex);

    void removeFromNodes() const;

public:
    AprochConnectionGraphicsObject &getConnectionGraphicsObject() const;

    inline SConnectionState const &connectionState() const { return mConnectionState; }
    inline SConnectionState &connectionState() { return mConnectionState; }

    AprochNode *getNode(EPortType portType) const;
    AprochNode *&getNode(EPortType portType);

    PortIndex getPortIndex(EPortType portType) const;

    void clearNode(EPortType portType);

    SNodeDataType dataType(EPortType portType) const;

    void setTypeConverter(TypeConverter converter);

    inline bool complete() const {return mInNode != nullptr && mOutNode != nullptr;}

    void propagateData(QSharedPointer<INodeData> nodeData) const;
    void propagateEmptyData() const;

Q_SIGNALS:
    void connectionCompleted(const AprochConnection &) const;
    void connectionMadeIncomplete(const AprochConnection &) const;
    void updated(AprochConnection &conn) const;

private:
    QUuid mUuid;

    AprochNode *mOutNode = nullptr;
    AprochNode *mInNode = nullptr;

    PortIndex mOutPortIndex;
    PortIndex mInPortIndex;

    SConnectionState mConnectionState;

    QScopedPointer<AprochConnectionGraphicsObject> mConnectionGraphicsObject;

    TypeConverter mTypeConverter;

    EPortType mRequiredPort;

    AprochNode* mLastHoveredNode{nullptr};
};

APROCH_NAMESPACE_END

#endif // APROCHCONNECTION_H
