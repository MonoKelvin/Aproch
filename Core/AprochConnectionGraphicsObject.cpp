#include "AprochConnectionGraphicsObject.h"

#include "AprochNode.h"
#include "AprochFlowScene.h"
#include "AprochConnection.h"
#include "AprochNodeGraphicsObject.h"
#include "AprochConnectionPainter.h"
#include "AprochDataModelRegistry.h"
#include "AprochNCInteraction.h"

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

            mConnection.mConnectionGraphicsObject->setGeometryChanged();
            mConnection.mConnectionGraphicsObject->update();
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

    AprochNCInteraction interaction(*node, mConnection, mScene);

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
    mConnection.mIsHovered = true;

    update();
    mScene.connectionHovered(connection(), event->screenPos());
    event->accept();
}

void AprochConnectionGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mConnection.mIsHovered = false;

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

APROCH_NAMESPACE_END
