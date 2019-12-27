#include "AprochConnectionGraphicsObject.h"

#include "AprochNode.h"
#include "AprochConnection.h"
#include "AprochFlowScene.h"
#include "AprochConnection.h"
#include "AprochNodeGraphicsObject.h"

#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsBlurEffect>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneMouseEvent>

APROCH_NAMESPACE_BEGIN

AprochConnectionGraphicsObject::AprochConnectionGraphicsObject(AprochFlowScene& scene, AprochConnection& connection)
    : _scene(scene)
    , _connection(connection)
{
    _scene.addItem(this);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setAcceptHoverEvents(true);

    // addGraphicsEffect();

    setZValue(-1.0);
}

QRectF AprochConnectionGraphicsObject::boundingRect() const
{
    return _connection.boundingRect();
}

QPainterPath AprochConnectionGraphicsObject::shape() const
{
#ifdef APROCH_DEBUG_DRAWING

    // QPainterPath path;

    // path.addRect(boundingRect());
    // return path;

#else
    return AprochConnectionPainter::getPainterStroke(_connection);
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
        if (auto node = _connection.getNode(portType))
        {
            auto const& nodeGraphics = node->getNodeGraphicsObject();

            QPointF scenePos = node->getPortScenePosition(_connection.getPortIndex(portType), portType, nodeGraphics.sceneTransform());

            QTransform sceneTransform = this->sceneTransform();

            QPointF connectionPos = sceneTransform.inverted().map(scenePos);

            _connection.setEndPoint(portType, connectionPos);

            _connection.getConnectionGraphicsObject().setGeometryChanged();
            _connection.getConnectionGraphicsObject().update();
        }
    }
}

void AprochConnectionGraphicsObject::lock(bool locked)
{
    setFlag(QGraphicsItem::ItemIsMovable, !locked);
    setFlag(QGraphicsItem::ItemIsFocusable, !locked);
    setFlag(QGraphicsItem::ItemIsSelectable, !locked);
}

void AprochConnectionGraphicsObject::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget*)
{
    painter->setClipRect(option->exposedRect);

    AprochConnectionPainter::paint(painter, _connection);
}

void AprochConnectionGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mousePressEvent(event);
    // event->ignore();
}

void AprochConnectionGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    prepareGeometryChange();

    auto view = static_cast<QGraphicsView*>(event->widget());
    auto node = AprochFlowScene::LocateNodeAt(event->scenePos(), _scene, view->transform());

    _connection.interactWithNode(node);
    if (node)
    {
        node->reactToPossibleConnection(_connection.getRequiredPort(),
                                        _connection.dataType(AprochPort::OppositePort(_connection.getRequiredPort())),
                                        event->scenePos());
    }

    //-------------------

    QPointF offset = event->pos() - event->lastPos();

    auto requiredPort = _connection.getRequiredPort();

    if (requiredPort != EPortType::None)
    {
        _connection.moveEndPoint(requiredPort, offset);
    }

    //-------------------

    update();

    event->accept();
}

void AprochConnectionGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    ungrabMouse();
    event->accept();

    auto node = AprochFlowScene::LocateNodeAt(event->scenePos(), _scene, _scene.views()[0]->transform());

    NodeConnectionInteraction interaction(*node, _connection, _scene);

    if (node && interaction.tryConnect())
    {
        node->resetReactionToConnection();
    }

    if (_connection.isRequirePort())
    {
        _scene.deleteConnection(_connection);
    }
}

void AprochConnectionGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    _connection.setHovered(true);

    update();
    _scene.connectionHovered(connection(), event->screenPos());
    event->accept();
}

void AprochConnectionGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    _connection.setHovered(false);

    update();
    _scene.connectionHoverLeft(connection());
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
