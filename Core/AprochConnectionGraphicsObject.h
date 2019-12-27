#ifndef APROCHCONNECTIONGRAPHICSOBJECT_H
#define APROCHCONNECTIONGRAPHICSOBJECT_H

#include "AprochPort.h"
#include "Utilities.h"

#include <QGraphicsObject>
#include <QUuid>

APROCH_NAMESPACE_BEGIN

class AprochFlowScene;
class AprochConnection;
class AprochNode;

class APROCH_EXPORT AprochConnectionGraphicsObject : public QGraphicsObject
{
    Q_OBJECT

private:

    // Class performs various operations on the Node and Connection pair.
    // An instance should be created on the stack and destroyed when
    // the operation is completed
    class NCInteraction
    {
        friend class AprochConnectionGraphicsObject;
    public:
        NCInteraction(AprochNode &node, AprochConnection &connection, AprochFlowScene &scene);

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

        EPortType connectionRequiredPort() const;

        QPointF connectionEndScenePosition(EPortType) const;

        QPointF nodePortScenePosition(EPortType portType, PortIndex portIndex) const;

        PortIndex nodePortIndexUnderScenePoint(EPortType portType, const QPointF &p) const;

        bool nodePortIsEmpty(EPortType portType, PortIndex portIndex) const;

    private:
        AprochNode *mNode;
        AprochConnection *mConnection;
        AprochFlowScene *mScene;
    };

public:
    AprochConnectionGraphicsObject(AprochFlowScene &scene, AprochConnection &connection);

    virtual ~AprochConnectionGraphicsObject() override;

    enum { Type = UserType + 2 };
    int type() const override
    {
        return Type;
    }

public:
    inline AprochConnection &connection() { return mConnection; }

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void setGeometryChanged();

    // Updates the position of both ends
    void move();

    void lock(bool locked);

protected:
    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void addGraphicsEffect();

private:
    AprochFlowScene &mScene;

    AprochConnection &mConnection;
};

APROCH_NAMESPACE_END

#endif // APROCHCONNECTIONGRAPHICSOBJECT_H
