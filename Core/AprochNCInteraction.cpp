#include "AprochNCInteraction.h"

#include "AprochNode.h"
#include "AprochConnection.h"
#include "AprochFlowScene.h"
#include "AprochDataModelRegistry.h"
#include "AprochNodeGraphicsObject.h"
#include "AprochConnectionGraphicsObject.h"

APROCH_NAMESPACE_BEGIN

AprochNCInteraction::AprochNCInteraction(AprochNode &node, AprochConnection &connection, AprochFlowScene &scene)
    : mNode(&node)
    , mConnection(&connection)
    , mScene(&scene)
{}


bool AprochNCInteraction::canConnect(PortIndex &portIndex, TypeConverter &converter) const
{
    // 1) AprochConnection requires a port

    EPortType requiredPort = getConnectionRequiredPort();

    if (requiredPort == EPortType::None)
    {
        return false;
    }

    // 1.5) Forbid connecting the node to itself
    AprochNode *node = mConnection->getNode(AprochPort::OppositePort(requiredPort));

    if (node == mNode)
    {
        return false;
    }

    // 2) connection point is on top of the node port

    QPointF connectionPoint = getConnectionEndScenePosition(requiredPort);

    portIndex = getNodePortIndexUnderScenePoint(requiredPort, connectionPoint);

    if (portIndex == INVALID_PORT_INDEX)
    {
        return false;
    }

    // 3) AprochNode port is vacant

    // port should be empty
    if (!isNodePortEmpty(requiredPort, portIndex))
    {
        return false;
    }

    // 4) AprochConnection type equals node port type, or there is a registered type conversion that can translate between the two

    auto connectionDataType = mConnection->dataType(AprochPort::OppositePort(requiredPort));

    auto const &modelTarget = mNode->getNodeDataModel();
    SNodeDataType candidateNodeDataType = modelTarget->dataType(requiredPort, portIndex);

    if (connectionDataType.id != candidateNodeDataType.id)
    {
        if (requiredPort == EPortType::Input)
        {
            converter = mScene->registry().getTypeConverter(connectionDataType, candidateNodeDataType);
        }
        else if (requiredPort == EPortType::Output)
        {
            converter = mScene->registry().getTypeConverter(candidateNodeDataType, connectionDataType);
        }

        return (converter != nullptr);
    }

    return true;
}


bool AprochNCInteraction::tryConnect() const
{
    // 1) Check conditions from 'canConnect'
    PortIndex portIndex = INVALID_PORT_INDEX;

    TypeConverter converter;

    if (!canConnect(portIndex, converter))
    {
        return false;
    }

    // 1.5) If the connection is possible but a type conversion is needed,
    //      assign a convertor to connection
    if (converter)
    {
        mConnection->setTypeConverter(converter);
    }

    // 2) Assign node to required port in AprochConnection
    EPortType requiredPort = getConnectionRequiredPort();
    mNode->setConnection(requiredPort, portIndex, *mConnection);

    // 3) Assign AprochConnection to empty port in NodeState
    // The port is not longer required after this function
    mConnection->setNodeToPort(*mNode, requiredPort, portIndex);

    // 4) Adjust AprochConnection geometry

    mNode->getNodeGraphicsObject().moveConnections();

    // 5) Poke model to intiate data transfer

    auto outNode = mConnection->getNode(EPortType::Output);
    if (outNode)
    {
        PortIndex outPortIndex = mConnection->getPortIndex(EPortType::Output);
        outNode->onDataUpdated(outPortIndex);
    }

    return true;
}


/// 1) AprochNode and AprochConnection should be already connected
/// 2) If so, clear AprochConnection entry in the NodeState
/// 3) Set AprochConnection end to 'requiring a port'
bool AprochNCInteraction::disconnect(EPortType portToDisconnect) const
{
    PortIndex portIndex = mConnection->getPortIndex(portToDisconnect);

    // clear pointer to AprochConnection in the NodeState
    mNode->getEntries(portToDisconnect)[int(portIndex)].clear();

    // 4) Propagate invalid data to IN node
    mConnection->propagateEmptyData();

    // clear AprochConnection side
    mConnection->clearNode(portToDisconnect);

    mConnection->setRequiredPort(portToDisconnect);

    mConnection->getConnectionGraphicsObject().grabMouse();

    return true;
}


// ------------------ util functions below

EPortType AprochNCInteraction::getConnectionRequiredPort() const
{
    return mConnection->getRequiredPort();
}


QPointF AprochNCInteraction::getConnectionEndScenePosition(EPortType portType) const
{
    QPointF endPoint = mConnection->getEndPoint(portType);

    return mConnection->getConnectionGraphicsObject().mapToScene(endPoint);
}


QPointF AprochNCInteraction::getNodePortScenePosition(EPortType portType, PortIndex portIndex) const
{
    QPointF p = mNode->getPortScenePosition(portIndex, portType);

    return mNode->getNodeGraphicsObject().sceneTransform().map(p);
}


PortIndex AprochNCInteraction::getNodePortIndexUnderScenePoint(EPortType portType, const QPointF &scenePoint) const
{
    QTransform sceneTransform =  mNode->getNodeGraphicsObject().sceneTransform();
    PortIndex portIndex = mNode->checkHitScenePoint(portType, scenePoint, sceneTransform);

    return portIndex;
}


bool AprochNCInteraction::isNodePortEmpty(EPortType portType, PortIndex portIndex) const
{
    auto const &entries = mNode->getEntries(portType);

    if (entries[int(portIndex)].empty())
    {
        return true;
    }

    const auto outPolicy = mNode->getNodeDataModel()->portOutConnectionPolicy(portIndex);
    return (portType == EPortType::Output && outPolicy == INodeDataModel::EConnectionPolicy::Many);
}


APROCH_NAMESPACE_END
