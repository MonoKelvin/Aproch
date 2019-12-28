#ifndef APROCHNODE_H
#define APROCHNODE_H

#include "AprochNodeGraphicsObject.h"
#include "AprochNodeDataModel.h"
#include "AprochConnection.h"

#include <QUuid>
#include <QFontMetrics>
#include <QTransform>

APROCH_NAMESPACE_BEGIN

using ConnectionPtrSet = std::unordered_map<QUuid, AprochConnection *>;

struct SNodeState
{

    enum EReactToConnectionState
    {
        REACTING,
        NOT_REACTING
    };

    QVector<ConnectionPtrSet> InConnections;
    QVector<ConnectionPtrSet> OutConnections;

    EReactToConnectionState Reaction;
    EPortType ReactingPortType;
    SNodeDataType ReactingDataType;

    bool IsResizing;
};

class APROCH_EXPORT AprochNode : public QObject, public ISerializable
{
    Q_OBJECT

    friend class AprochNodeGraphicsObject;
public:
    AprochNode(std::unique_ptr<INodeDataModel> &&dataModel);
    virtual ~AprochNode(void) override = default;

public:
    QJsonObject save(void) const override;
    void restore(const QJsonObject &json) override;

    QRectF boundingRect(void) const;

    QPointF getPortScenePosition(PortIndex index, EPortType portType, const QTransform &transform = QTransform()) const;

    PortIndex checkHitScenePoint(EPortType portType, QPointF scenePoint, const QTransform &sceneTransform = QTransform()) const;

    int captionHeight(void) const;
    int captionWidth(void) const;

    int validationHeight() const;
    int validationWidth() const;

    // Returns the maximum height a widget can be without causing the node to grow.
    int equivalentWidgetHeight() const;

    QPointF getWidgetPosition() const;

    inline QUuid getId(void) const { return mUuid; }
    inline int getWidth(void) const { return mWidth; }
    inline void setWidth(int width) { mWidth = width; }
    inline int getHeight(void) const { return mHeight; }
    inline void setHeight(int height) { mHeight = height; }
    inline bool isHovered(void) const { return mIsHovered; }
    inline void setHovered(bool isHovered) { mIsHovered = isHovered; }

    inline AprochNodeGraphicsObject const &getNodeGraphicsObject(void) const {return *mNodeGraphicsObject.get(); }
    inline AprochNodeGraphicsObject &getNodeGraphicsObject(void) {return *mNodeGraphicsObject.get(); }
    inline INodeDataModel *getNodeDataModel(void) const { return mNodeDataModel.get(); }

    inline void setNodeGraphicsObject(std::unique_ptr<AprochNodeGraphicsObject> &&graphicsObj)
    {
        mNodeGraphicsObject = std::move(graphicsObj);
        resize();
    }

    // Returns vector of connections ID. Some of them can be empty (null)
    QVector<ConnectionPtrSet> const &getEntries(EPortType) const;
    QVector<ConnectionPtrSet> &getEntries(EPortType);

    ConnectionPtrSet getConnections(EPortType portType, PortIndex portIndex) const;

    void setConnection(EPortType portType, PortIndex portIndex, AprochConnection &connection);
    void eraseConnection(EPortType portType, PortIndex portIndex, QUuid id);

    SNodeState::EReactToConnectionState reaction() const;

    void reactToPossibleConnection(EPortType reactingPortType, const SNodeDataType &reactingDataType, const QPointF &scenePoint);

    EPortType reactingPortType(void) const;
    SNodeDataType reactingDataType(void) const;

    void setReaction(SNodeState::EReactToConnectionState reaction,
                     EPortType reactingPortType = EPortType::None,
                     SNodeDataType reactingDataType = SNodeDataType());

    bool isReacting(void) const { return mNodeState.IsResizing;}

    void setResizing(bool resizing) { mNodeState.IsResizing = resizing; }

    bool resizing(void) const;

    SNodeState const &getNodeState(void) const {return mNodeState;}

    QPoint getWidgetPosition();

    void resetReactionToConnection();

    inline QPointF const &getDraggingPos() const { return mDraggingPos; }
    inline void setDraggingPosition(QPointF const &pos) { mDraggingPos = pos; }

    QRect resizeRect() const;

public Q_SLOTS:

    /// Propagates incoming data to the underlying model.
    void propagateData(std::shared_ptr<INodeData> nodeData, PortIndex inPortIndex) const;

    /// Fetches data from model's OUT #index port
    /// and propagates it to the connection
    void onDataUpdated(PortIndex index);

    /// update the graphic part if the size of the embeddedwidget changes
    void onNodeSizeUpdated();

private:
    void resize(void) const;

    int getPortWidth(EPortType portType) const;

private:
    mutable int mWidth;
    mutable int mHeight;
    mutable int mPortWidth;
    int mSpacing;

    mutable QFontMetrics mFontMetrics;
    mutable QFontMetrics mBoldFontMetrics;

    bool mIsHovered;
    QPointF mDraggingPos;

    QUuid mUuid;

    SNodeState mNodeState;

    std::unique_ptr<INodeDataModel> const &mNodeDataModel;
    std::unique_ptr<AprochNodeGraphicsObject> mNodeGraphicsObject;
};

APROCH_NAMESPACE_END

#endif // APROCHNODE_H
