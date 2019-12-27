#include "AprochConnectionGraphicsObject.h"

#include "AprochNode.h"
#include "AprochFlowScene.h"
#include "AprochConnection.h"
#include "AprochNodeGraphicsObject.h"
#include "AprochConnectionPainter.h"
#include "AprochDataModelRegistry.h"

#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsBlurEffect>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

APROCH_NAMESPACE_BEGIN

AprochConnectionGraphicsObject::AprochConnectionGraphicsObject(AprochFlowScene &scene, AprochConnection &connection)
    : mScene(scene)
    , mConnection(connection)
{
    mScene.addItem(this);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setAcceptHoverEvents(true);

    // addGraphicsEffect();

    setZValue(-1.0);
}

QRectF AprochConnectionGraphicsObject::boundingRect() const
{
    return mConnection.boundingRect();
}

QPainterPath AprochConnectionGraphicsObject::shape() const
{
#ifdef APROCH_DEBUG_DRAWING

    // QPainterPath path;

    // path.addRect(boundingRect());
    // return path;

#else
    return AprochConnectionPainter::GetPainterStroke(mConnection);
#endif
}

void AprochConnectionGraphicsObject::setGeometryChanged()
{
    prepareGeometryChange();
}

void AprochConnectionGraphicsObject::move()
{
    for (EPortType portType : {EPortType::Input, EPortType::Output})
    {
        if (auto node = mConnection.getNode(portType))
        {
            auto const &nodeGraphics = node->getNodeGraphicsObject();

            QPointF scenePos = node->getPortScenePosition(mConnection.getPortIndex(portType), portType, nodeGraphics.sceneTransform());

            QTransform sceneTransform = this->sceneTransform();

            QPointF connectionPos = sceneTransform.inverted().map(scenePos);

            mConnection.setEndPoint(portType, connectionPos);

            mConnection.getConnectionGraphicsObject().setGeometryChanged();
            mConnection.getConnectionGraphicsObject().update();
        }
    }
}

void AprochConnectionGraphicsObject::lock(bool locked)
{
    setFlag(QGraphicsItem::ItemIsMovable, !locked);
    setFlag(QGraphicsItem::ItemIsFocusable, !locked);
    setFlag(QGraphicsItem::ItemIsSelectable, !locked);
}

void AprochConnectionGraphicsObject::paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *)
{
    painter->setClipRect(option->exposedRect);

    AprochConnectionPainter::Paint(painter, mConnection);
}

void AprochConnectionGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    // event->ignore();
}

void AprochConnectionGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    prepareGeometryChange();

    auto view = static_cast<QGraphicsView *>(event->widget());
    auto node = AprochFlowScene::LocateNodeAt(event->scenePos(), mScene, view->transform());

    mConnection.interactWithNode(node);
    if (node)
    {
        node->reactToPossibleConnection(mConnection.getRequiredPort(),
                                        mConnection.dataType(AprochPort::OppositePort(mConnection.getRequiredPort())),
                                        event->scenePos());
    }

    //-------------------

    QPointF offset = event->pos() - event->lastPos();

    auto requiredPort = mConnection.getRequiredPort();

    if (requiredPort != EPortType::None)
    {
        mConnection.moveEndPoint(requiredPort, offset);
    }

    //-------------------

    update();

    event->accept();
}

void AprochConnectionGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ungrabMouse();
    event->accept();

    auto node = AprochFlowScene::LocateNodeAt(event->scenePos(), mScene, mScene.views()[0]->transform());

    NCInteraction interaction(*node, mConnection, mScene);

    if (node && interaction.tryConnect())
    {
        node->resetReactionToConnection();
    }

    if (mConnection.isRequirePort())
    {
        mScene.deleteConnection(mConnection);
    }
}

void AprochConnectionGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    mConnection.setHovered(true);

    update();
    mScene.connectionHovered(connection(), event->screenPos());
    event->accept();
}

void AprochConnectionGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mConnection.setHovered(false);

    update();
    mScene.connectionHoverLeft(connection());
    event->accept();
}

void AprochConnectionGraphicsObject::addGraphicsEffect()
{
    auto effect = new QGraphicsBlurEffect;

    effect->setBlurRadius(5);
    setGraphicsEffect(effect);

    // auto effect = new QGraphicsDropShadowEffect;
    // auto effect = new ConnectionBlurEffect(this);
    // effect->setOffset(4, 4);
    // effect->setColor(QColor(Qt::gray).darker(800));
}

//--------------------------------------------------------------
// NCInteraction
//--------------------------------------------------------------

AprochConnectionGraphicsObject::NCInteraction::NCInteraction(AprochNode &node, AprochConnection &connection, AprochFlowScene &scene)
    : mNode(&node)
    , mConnection(&connection)
    , mScene(&scene)
{}


bool AprochConnectionGraphicsObject::NCInteraction::canConnect(PortIndex &portIndex, TypeConverter &converter) const
{
    // 1) AprochConnection requires a port

    EPortType requiredPort = connectionRequiredPort();

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

    QPointF connectionPoint = connectionEndScenePosition(requiredPort);

    portIndex = nodePortIndexUnderScenePoint(requiredPort, connectionPoint);

    if (portIndex == INVALID_PORT_INDEX)
    {
        return false;
    }

    // 3) AprochNode port is vacant

    // port should be empty
    if (!nodePortIsEmpty(requiredPort, portIndex))
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


bool AprochConnectionGraphicsObject::NCInteraction::tryConnect() const
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
    EPortType requiredPort = connectionRequiredPort();
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
bool AprochConnectionGraphicsObject::NCInteraction::disconnect(EPortType portToDisconnect) const
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

EPortType AprochConnectionGraphicsObject::NCInteraction::connectionRequiredPort() const
{
    return mConnection->getRequiredPort();
}


QPointF AprochConnectionGraphicsObject::NCInteraction::connectionEndScenePosition(EPortType portType) const
{
    QPointF endPoint = mConnection->getEndPoint(portType);

    return mConnection->getConnectionGraphicsObject().mapToScene(endPoint);
}


QPointF AprochConnectionGraphicsObject::NCInteraction::nodePortScenePosition(EPortType portType, PortIndex portIndex) const
{
    QPointF p = mNode->getPortScenePosition(portIndex, portType);

    return mNode->getNodeGraphicsObject().sceneTransform().map(p);
}


PortIndex AprochConnectionGraphicsObject::NCInteraction::nodePortIndexUnderScenePoint(EPortType portType, const QPointF &scenePoint) const
{
    QTransform sceneTransform =  mNode->getNodeGraphicsObject().sceneTransform();
    PortIndex portIndex = mNode->checkHitScenePoint(portType, scenePoint, sceneTransform);

    return portIndex;
}


bool AprochConnectionGraphicsObject::NCInteraction::nodePortIsEmpty(EPortType portType, PortIndex portIndex) const
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
