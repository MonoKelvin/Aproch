#ifndef APROCHCONNECTIONGRAPHICSOBJECT_H
#define APROCHCONNECTIONGRAPHICSOBJECT_H

#include "APort.h"
#include "Utilities.h"

#include <QGraphicsObject>
#include <QUuid>

APROCH_NAMESPACE_BEGIN

class AFlowScene;
class AConnection;
class ANode;

class APROCH_EXPORT AConnectionGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
public:
    AConnectionGraphicsObject(AFlowScene &scene, AConnection &connection);

    ~AConnectionGraphicsObject() override;

    enum { Type = UserType + 2 };
    int type() const override
    {
        return Type;
    }

public:
    inline AConnection &connection() { return mConnection; }

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
    AFlowScene &mScene;

    AConnection &mConnection;
};

APROCH_NAMESPACE_END

#endif // APROCHCONNECTIONGRAPHICSOBJECT_H
