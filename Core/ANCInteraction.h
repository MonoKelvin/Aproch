#ifndef APROCHNCITERACTION_H
#define APROCHNCITERACTION_H

#include "APort.h"
#include "Utilities.h"

class QPointF;

APROCH_NAMESPACE_BEGIN

class ANode;
class AConnection;
class AFlowScene;

// Class performs various operations on the Node and Connection pair.
// An instance should be created on the stack and destroyed when
// the operation is completed
class ANCInteraction
{
    friend class AConnectionGraphicsObject;
    friend class ANodeGraphicsObject;
public:
    ANCInteraction(ANode &node, AConnection &connection, AFlowScene &scene);

    /// Can connect when following conditions are met:
    /// 1) Connection 'requires' a port
    /// 2) Connection's vacant end is above the node port
    /// 3) Node port is vacant
    /// 4) Connection type equals node port type, or there is a registered type conversion that can translate between the two
    bool canConnect(PortIndex &portIndex, TypeConverter &converter) const;

    /// 1)   Check conditions from 'canConnect'
    /// 1.5) If the connection is possible but a type conversion is needed, add a converter node to the scene, and connect it properly
    /// 2)   Assign node to required port in Connection
    /// 3)   Assign Connection to empty port in NodeState
    /// 4)   Adjust Connection geometry
    /// 5)   Poke model to initiate data transfer
    bool tryConnect() const;

    /// 1) Node and Connection should be already connected
    /// 2) If so, clear Connection entry in the NodeState
    /// 3) Propagate invalid data to IN node
    /// 4) Set Connection end to 'requiring a port'
    bool disconnect(EPortType portToDisconnect) const;

    EPortType getConnectionRequiredPort() const;

    QPointF getConnectionEndScenePosition(EPortType) const;

    QPointF getNodePortScenePosition(EPortType portType, PortIndex portIndex) const;

    PortIndex getNodePortIndexUnderScenePoint(EPortType portType, const QPointF &p) const;

    bool isNodePortEmpty(EPortType portType, PortIndex portIndex) const;

private:
    ANode *mNode;
    AConnection *mConnection;
    AFlowScene *mScene;
};


APROCH_NAMESPACE_END

#endif // APROCHNCITERACTION_H
