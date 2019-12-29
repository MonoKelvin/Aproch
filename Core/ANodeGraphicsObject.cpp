#include "ANodeGraphicsObject.h"

#include "ANode.h"
#include "AConnection.h"
#include "AConnectionGraphicsObject.h"
#include "AFlowScene.h"
#include "ANodePainter.h"
#include "ANCInteraction.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QCursor>

APROCH_NAMESPACE_BEGIN

ANodeGraphicsObject::ANodeGraphicsObject(AFlowScene &scene, ANode& node)
    : mScene(scene)
    , mNode(node)
    , mIsLocked(false)
    , mProxyWidget(nullptr)
{
    mScene.addItem(this);

    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

    setCacheMode( QGraphicsItem::DeviceCoordinateCache );

    auto const &style = node.mNodeDataModel->getNodeStyle();

    {
        auto effect = new QGraphicsDropShadowEffect;
        effect->setOffset(0.0, 0.0);
        effect->setBlurRadius(20.0);
        effect->setColor(style.ShadowColor);
        setGraphicsEffect(effect);
    }

    setOpacity(double(style.Opacity));

    setAcceptHoverEvents(true);

    setZValue(0.0);

    embedQWidget();

    // connect to the move signals to emit the move signals in FlowScene
    auto onMoveSlot = [this] {
        mScene.nodeMoved(mNode, pos());
    };

    connect(this, &QGraphicsObject::xChanged, this, onMoveSlot);
    connect(this, &QGraphicsObject::yChanged, this, onMoveSlot);
}

ANodeGraphicsObject::~ANodeGraphicsObject()
{
    mScene.removeItem(this);
}

ANode &ANodeGraphicsObject::node()
{
    return mNode;
}

ANode const &ANodeGraphicsObject::node() const
{
    return mNode;
}

void ANodeGraphicsObject::embedQWidget()
{
    if (auto w = mNode.getNodeDataModel()->getEmbeddedWidget())
    {
        mProxyWidget = new QGraphicsProxyWidget(this);

        mProxyWidget->setWidget(w);

        mProxyWidget->setPreferredWidth(5);

        mNode.resize();

        if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag)
        {
            // If the widget wants to use as much vertical space as possible, set it to have the geom's equivalentWidgetHeight.
            mProxyWidget->setMinimumHeight(mNode.equivalentWidgetHeight());
        }

        mProxyWidget->setPos(mNode.getWidgetPosition());

        update();

        mProxyWidget->setOpacity(1.0);
        mProxyWidget->setFlag(QGraphicsItem::ItemIgnoresParentOpacity);
    }
}


QRectF ANodeGraphicsObject::boundingRect() const
{
    return mNode.boundingRect();
}


void ANodeGraphicsObject::setGeometryChanged()
{
    prepareGeometryChange();
}


void ANodeGraphicsObject::moveConnections() const
{
    for (EPortType portType: {EPortType::Input, EPortType::Output})
    {
        auto const & connectionEntries = mNode.getEntries(portType);

        for (auto const & connections : connectionEntries)
        {
            for (auto & con : connections)
            {
                con.second->getConnectionGraphicsObject().move();
            }
        }
    }
}

void ANodeGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setClipRect(option->exposedRect);
    ANodePainter::Paint(painter, mNode, mScene);
}

void ANodeGraphicsObject::lock(bool locked)
{
    mIsLocked = locked;

    setFlag(QGraphicsItem::ItemIsMovable, !locked);
    setFlag(QGraphicsItem::ItemIsFocusable, !locked);
    setFlag(QGraphicsItem::ItemIsSelectable, !locked);
}


QVariant ANodeGraphicsObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        moveConnections();
    }

    return QGraphicsItem::itemChange(change, value);
}


void ANodeGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    // bring all the colliding nodes to background
    for (QGraphicsItem *item : collidingItems())
    {
        if (item->zValue() > 0.0)
        {
            item->setZValue(0.0);
        }
    }

    // bring this node forward
    setZValue(1.0);

    if (mIsLocked)
    {
        return;
    }

    // deselect all other items after this one is selected
    if (!isSelected() && !(event->modifiers() & Qt::ControlModifier))
    {
        mScene.clearSelection();
    }

    for (EPortType portToCheck: {EPortType::Input, EPortType::Output})
    {
        // TODO do not pass sceneTransform
        PortIndex portIndex = mNode.checkHitScenePoint(portToCheck, event->scenePos(), sceneTransform());

        if (portIndex != INVALID_PORT_INDEX)
        {
            ConnectionPtrSet connections = mNode.getConnections(portToCheck, portIndex);

            // start dragging existing connection
            if (!connections.empty() && portToCheck == EPortType::Input)
            {
                auto con = connections.begin()->second;

                ANCInteraction interaction(mNode, *con, mScene);

                interaction.disconnect(portToCheck);
            }
            else // initialize new Connection
            {
                if (portToCheck == EPortType::Output)
                {
                    auto const outPolicy = mNode.mNodeDataModel->portOutConnectionPolicy(portIndex);
                    if (!connections.empty() && outPolicy == INodeDataModel::EConnectionPolicy::One)
                    {
                        mScene.deleteConnection( *connections.begin()->second );
                    }
                }

                // todo add to FlowScene
                auto connection = mScene.createConnection(portToCheck, mNode, portIndex);

                mNode.setConnection(portToCheck, portIndex, *connection);

                connection->getConnectionGraphicsObject().grabMouse();
            }
        }
    }

    auto pos = event->pos();

    if (mNode.mNodeDataModel->resizable() && mNode.resizeRect().contains(QPoint(pos.x(), pos.y())))
    {
        mNode.setResizing(true);
    }
}


void ANodeGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (mNode.resizing())
    {
        auto diff = event->pos() - event->lastPos();

        if (auto w = mNode.mNodeDataModel->getEmbeddedWidget())
        {
            prepareGeometryChange();

            auto oldSize = w->size();

            oldSize += QSize(int(diff.x()), int(diff.y()));

            w->setFixedSize(oldSize);

            mProxyWidget->setMinimumSize(oldSize);
            mProxyWidget->setMaximumSize(oldSize);
            mProxyWidget->setPos(mNode.getWidgetPosition());

            mNode.resize();
            update();

            moveConnections();

            event->accept();
        }
    }
    else
    {
        QGraphicsObject::mouseMoveEvent(event);

        if (event->lastPos() != event->pos())
        {
            moveConnections();
        }

        event->ignore();
    }

    QRectF r = scene()->sceneRect();

    r = r.united(mapToScene(boundingRect()).boundingRect());

    scene()->setSceneRect(r);
}


void ANodeGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    mNode.setResizing(false);

    QGraphicsObject::mouseReleaseEvent(event);

    // position connections precisely after fast node move
    moveConnections();
}


void ANodeGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    mNode.setHovered(true);
    update();
    mScene.nodeHovered(node(), event->screenPos());
    event->accept();
}


void ANodeGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    mNode.setHovered(false);
    update();
    mScene.nodeHoverLeft(node());
    event->accept();
}


void ANodeGraphicsObject::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
    auto pos = event->pos();

    if (mNode.getNodeDataModel()->resizable() && mNode.resizeRect().contains(QPoint(pos.x(), pos.y())))
    {
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else
    {
        setCursor(QCursor());
    }

    event->accept();
}


void ANodeGraphicsObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);

    mScene.nodeDoubleClicked(node());
}


void ANodeGraphicsObject::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    mScene.nodeContextMenu(node(), mapToScene(event->pos()));
}

APROCH_NAMESPACE_END
