#ifndef APROCHCONNECTIONGRAPHICSOBJECT_H
#define APROCHCONNECTIONGRAPHICSOBJECT_H

#include "Aproch.h"

#include <QtCore/QUuid>
#include <QGraphicsObject>

class QGraphicsSceneMouseEvent;

APROCH_NAMESPACE_BEGIN

class FlowScene;
class Connection;
class ConnectionGeometry;
class Node;

class AprochConnectionGraphicsObject : public QGraphicsObject {
    Q_OBJECT
public:
    AprochConnectionGraphicsObject(FlowScene &scene, Connection &connection);

    virtual ~AprochConnectionGraphicsObject() override;

    enum { Type = UserType + 2 };
    int type() const override {
        return Type;
    }

public:

    Connection &connection();

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void setGeometryChanged();

    /// Updates the position of both ends
    void move();

    void lock(bool locked);

protected:

    void paint(QPainter *painter,
               QStyleOptionGraphicsItem const *option,
               QWidget *widget = 0) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:

    void addGraphicsEffect();

private:

    FlowScene &_scene;

    Connection &_connection;
};

APROCH_NAMESPACE_END

#endif // APROCHCONNECTIONGRAPHICSOBJECT_H
