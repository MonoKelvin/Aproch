#include "AConnectionGraphicsObject.h"

#include "ANode.h"
#include "AFlowScene.h"
#include "AConnection.h"
#include "ANodeGraphicsObject.h"
#include "AConnectionPainter.h"
#include "ADataModelRegistry.h"
#include "ANCInteraction.h"

#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsBlurEffect>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

APROCH_NAMESPACE_BEGIN

AConnectionGraphicsObject::AConnectionGraphicsObject(AFlowScene &scene, AConnection &connection)
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

    connection.mConnectionGraphicsObject = this;
}

AConnectionGraphicsObject::~AConnectionGraphicsObject()
{
    mScene.removeItem(this);
}

QRectF AConnectionGraphicsObject::boundingRect() const
{
    return mConnection.boundingRect();
}

QPainterPath AConnectionGraphicsObject::shape() const
{
#ifdef APROCH_DEBUG_DRAWING

    QPainterPath path;

    path.addRect(boundingRect());
    return path;

#else
    return AConnectionPainter::GetPainterStroke(mConnection);
#endif
}

void AConnectionGraphicsObject::setGeometryChanged()
{
    prepareGeometryChange();
}

void AConnectionGraphicsObject::move()
{
    for (EPortType portType : {EPortType::Input, EPortType::Output})
    {
        if (auto node = mConnection.getNode(portType))
        {
            auto const &nodeGraphics = node->getNodeGraphicsObject();

            QPointF scenePos = node->getPortScenePosition(mConnection.getPortIndex(portType), portType, nodeGraphics->sceneTransform());

            QTransform sceneTransform = this->sceneTransform();

            QPointF connectionPos = sceneTransform.inverted().map(scenePos);

            mConnection.setEndPoint(portType, connectionPos);

            mConnection.mConnectionGraphicsObject->setGeometryChanged();
            mConnection.mConnectionGraphicsObject->update();
        }
    }
}

void AConnectionGraphicsObject::lock(bool locked)
{
    setFlag(QGraphicsItem::ItemIsMovable, !locked);
    setFlag(QGraphicsItem::ItemIsFocusable, !locked);
    setFlag(QGraphicsItem::ItemIsSelectable, !locked);
}

void AConnectionGraphicsObject::paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *)
{
    painter->setClipRect(option->exposedRect);

    AConnectionPainter::Paint(painter, mConnection);
}

void AConnectionGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    // event->ignore();
}

void AConnectionGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    prepareGeometryChange();

    setCursor(Qt::CrossCursor);

    auto view = static_cast<QGraphicsView *>(event->widget());
    auto node = AFlowScene::LocateNodeAt(event->scenePos(), mScene, view->transform());

    mConnection.interactWithNode(node);
    if (node)
    {
        node->reactToPossibleConnection(mConnection.getRequiredPort(),
                                        mConnection.dataType(APort::OppositePort(mConnection.getRequiredPort())),
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

void AConnectionGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);

    ungrabMouse();
    event->accept();

    auto node = AFlowScene::LocateNodeAt(event->scenePos(), mScene, mScene.views()[0]->transform());

    ANCInteraction interaction(*node, mConnection, mScene);

    if (node && interaction.tryConnect())
    {
        node->resetReactionToConnection();
    }

    if (mConnection.isRequirePort())
    {
        mScene.deleteConnection(&mConnection);
    }
}

void AConnectionGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    mConnection.mIsHovered = true;

    update();
    mScene.connectionHovered(getConnection(), event->screenPos());
    event->accept();
}

void AConnectionGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mConnection.mIsHovered = false;

    update();
    mScene.connectionHoverLeft(getConnection());
    event->accept();
}

void AConnectionGraphicsObject::addGraphicsEffect()
{
    auto effect = new QGraphicsBlurEffect;

    effect->setBlurRadius(5);
    setGraphicsEffect(effect);

    // auto effect = new QGraphicsDropShadowEffect;
    // auto effect = new ConnectionBlurEffect(this);
    // effect->setOffset(4, 4);
    // effect->setColor(QColor(Qt::gray).darker(800));
}

APROCH_NAMESPACE_END
