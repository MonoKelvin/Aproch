#ifndef APROCHCONNECTIONGRAPHICSOBJECT_H
#define APROCHCONNECTIONGRAPHICSOBJECT_H

#include "Aproch.h"

#include <QGraphicsObject>
#include <QtCore/QUuid>

APROCH_NAMESPACE_BEGIN

class AprochFlowScene;
class AprochConnection;
class AprochNode;

class AprochConnectionGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
public:
    AprochConnectionGraphicsObject(AprochFlowScene& scene, AprochConnection& connection);

    virtual ~AprochConnectionGraphicsObject() override;

    enum { Type = UserType + 2 };
    int type() const override
    {
        return Type;
    }

public:
    inline AprochConnection& connection() { return _connection; }

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void setGeometryChanged();

    // Updates the position of both ends
    void move();

    void lock(bool locked);

protected:
    void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    void addGraphicsEffect();

private:
    AprochFlowScene& _scene;

    AprochConnection& _connection;
};

APROCH_NAMESPACE_END

#endif // APROCHCONNECTIONGRAPHICSOBJECT_H
