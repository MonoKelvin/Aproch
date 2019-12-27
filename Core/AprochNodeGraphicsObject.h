#ifndef APROCHNODEGRAPHICSOBJECT_H
#define APROCHNODEGRAPHICSOBJECT_H

#include "Aproch.h"

#include <QGraphicsObject>
#include <unordered_map>

class QGraphicsProxyWidget;

APROCH_NAMESPACE_BEGIN

class AprochNode;
class AprochConnection;
class AprochFlowScene;

class APROCH_EXPORT AprochNodeGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
public:
    AprochNodeGraphicsObject(AprochFlowScene& scene, AprochNode& node);

    virtual ~AprochNodeGraphicsObject() override;

    using ConnectionPtrSet = std::unordered_map<QUuid, AprochConnection*>;

    AprochNode& node();
    AprochNode const& node() const;

    void setGeometryChanged();

    void lock(bool locked);

    enum { Type = UserType + 1 };

    inline int type() const override { return Type; }

    QRectF boundingRect() const override;

    void moveConnections() const;

protected:
    void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent*) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
    void embedQWidget();

private:
    AprochFlowScene&  mScene;
    AprochNode& mNode;

    bool mIsLocked;

    // either nullptr or owned by parent QGraphicsItem
    QGraphicsProxyWidget* mProxyWidget;

};

APROCH_NAMESPACE_END

#endif // APROCHNODEGRAPHICSOBJECT_H
