#ifndef APROCHCONNECTION_H
#define APROCHCONNECTION_H

#include "Utilities.h"
#include "APort.h"
#include "ISerializable.h"

#include <QVariant>
#include <QPointF>

APROCH_NAMESPACE_BEGIN

class ANode;
class INodeData;
class AConnectionGraphicsObject;

struct SConnectionState
{
    EPortType RequiredPort = EPortType::None;
    ANode *LastHoveredNode = nullptr;
};

class APROCH_EXPORT AConnection : public QObject, public ISerializable
{
    Q_OBJECT

    friend class AConnectionGraphicsObject;
public:
    /**
     * New Connection is attached to the port of the given Node.
     * The port has parameters (portType, portIndex).
     * The opposite connection end will require another port.
     */
    AConnection(EPortType portType, ANode *node, PortIndex portIndex, QObject *parent = nullptr);
    AConnection(ANode *nodeIn, PortIndex portIndexIn, ANode *nodeOut, PortIndex portIndexOut, TypeConverter converter = TypeConverter{}, QObject *parent = nullptr);

    AConnection(const AConnection &) = delete;
    AConnection operator=(const AConnection &) = delete;

    ~AConnection();

    // ISerializable interface
public:
    QJsonObject save() const;

public:
    QUuid getId() const { return mUuid; }

    // Remembers the end being dragged.
    // Invalidates Node address.
    // Grabs mouse.
    void setRequiredPort(EPortType portType);
    inline EPortType getRequiredPort() const { return mConnectionState.RequiredPort; }

    inline void setNoRequiredPort() { mConnectionState.RequiredPort = EPortType::None; }
    inline bool isRequirePort() const { return mConnectionState.RequiredPort != EPortType::None; }

    void setGraphicsObject(AConnectionGraphicsObject *graphics);

    /// Assigns a node to the required port.
    /// It is assumed that there is a required port, no extra checks
    void setNodeToPort(ANode *node, EPortType portType, PortIndex portIndex);

    void removeFromNodes() const;

    QPointF const &getEndPoint(EPortType portType) const;
    void setEndPoint(EPortType portType, QPointF const &point);
    void moveEndPoint(EPortType portType, QPointF const &offset);

    QRectF boundingRect() const;

    std::pair<QPointF, QPointF> pointsC1C2() const;

    inline QPointF source() const { return mOutPoint; }
    inline QPointF sink() const { return mInPoint; }
    inline double lineWidth() const { return mLineWidth; }
    inline bool hovered() const { return mIsHovered; }
    inline void setHovered(bool hovered) { mIsHovered = hovered; }

public:
    inline AConnectionGraphicsObject *getConnectionGraphicsObject() const
    {
        return mConnectionGraphicsObject;
    }

    inline SConnectionState const &getConnectionState() const { return mConnectionState; }
    inline SConnectionState &getConnectionState() { return mConnectionState; }

    ANode *getNode(EPortType portType) const;
    ANode *&getNode(EPortType portType);

    PortIndex getPortIndex(EPortType portType) const;

    void clearNode(EPortType portType);

    SNodeDataType dataType(EPortType portType) const;

    void setTypeConverter(TypeConverter converter);

    inline bool complete() const { return mInNode != nullptr && mOutNode != nullptr; }

    void propagateData(INodeData *nodeData) const;
    void propagateEmptyData() const;

    void interactWithNode(ANode *node);

    inline void setLastHoveredNode(ANode *node) { mConnectionState.LastHoveredNode = node; }
    inline ANode *getLastHoveredNode() const { return mConnectionState.LastHoveredNode; }

    void resetLastHoveredNode();

Q_SIGNALS:
    void connectionCompleted(const AConnection &) const;
    void connectionMadeIncomplete(const AConnection &) const;
    void updated(AConnection &conn) const;

private:
    QUuid mUuid;

    QPointF mInPoint;
    QPointF mOutPoint;

    float mLineWidth;
    bool mIsHovered;

    ANode *mOutNode = nullptr;
    ANode *mInNode = nullptr;

    PortIndex mOutPortIndex;
    PortIndex mInPortIndex;

    SConnectionState mConnectionState;

    AConnectionGraphicsObject *mConnectionGraphicsObject;

    TypeConverter mTypeConverter;
};

APROCH_NAMESPACE_END

#endif // APROCHCONNECTION_H
