#ifndef APROCHNODEGRAPHICSOBJECT_H
#define APROCHNODEGRAPHICSOBJECT_H

#include "Aproch.h"

#include <QGraphicsObject>
#include <unordered_map>

class QGraphicsProxyWidget;

APROCH_NAMESPACE_BEGIN

class ANode;
class AConnection;
class AFlowScene;

class APROCH_EXPORT ANodeGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
public:
    ANodeGraphicsObject(AFlowScene &scene, ANode &getNode);

    virtual ~ANodeGraphicsObject() override;

    using ConnectionPtrSet = std::unordered_map<QUuid, AConnection *>;

    inline ANode &getNode() { return mNode; }
    inline ANode const &getNode() const { return mNode;  }

    inline void setGeometryChanged()
    {
        prepareGeometryChange();
    }

    void lock(bool locked);

    enum { Type = UserType + 1 };

    inline int type() const override { return Type; }

    QRectF boundingRect() const override;

    void moveConnections() const;

protected:
    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget = nullptr) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
    void embedQWidget();

private:
    AFlowScene  &mScene;
    ANode &mNode;

    bool mIsLocked;

    // either nullptr or owned by parent QGraphicsItem
    QGraphicsProxyWidget *mProxyWidget;

};

APROCH_NAMESPACE_END

#endif // APROCHNODEGRAPHICSOBJECT_H
