#ifndef APROCHNODE_H
#define APROCHNODE_H

#include "AprochNodeDataModel.h"

#include <QUuid>
#include <QFontMetrics>

#include <unordered_map>

APROCH_NAMESPACE_BEGIN

class AprochConnection;
class AprochNodeGraphicsObject;

using ConnectionPtrSet = std::unordered_map<QUuid, AprochConnection*>;

enum EReactToConnectionState
{
    REACTING,
    NOT_REACTING
};

struct SNodeState
{
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
    AprochNode(QScopedPointer<INodeDataModel> && dataModel);
    virtual ~AprochNode(void) override = default;

public:
    QJsonObject save(void) const override;
    void restore(const QJsonObject &json) override;

    QRectF boundingRect(void) const;

    QPointF getPortScenePosition(PortIndex index, EPortType portType,const QTransform &transform) const;

    int captionHeight(void) const;
    int captionWidth(void) const;

    inline QUuid getId(void) const { return mUuid; }
    inline int getWidth(void) const { return mWidth; }
    inline void setWidth(int width) { mWidth = width; }
    inline int getHeight(void) const { return mHeight; }
    inline void setHeight(int height) { mHeight = height; }
    inline bool isHovered(void) const { return mHovered; }

    inline AprochNodeGraphicsObject const &getNodeGraphicsObject(void) const {return *mNodeGraphicsObject.get(); }
    inline AprochNodeGraphicsObject & getNodeGraphicsObject(void) {return *mNodeGraphicsObject.get(); }
    inline INodeDataModel *getNodeDataModel(void) const { return mNodeDataModel.get(); }

    inline void setNodeGraphicsObject(QScopedPointer<AprochNodeGraphicsObject>&& graphicsObj)
    {
        mNodeGraphicsObject.reset(graphicsObj.take());
        resize();
    }

    // Returns vector of connections ID. Some of them can be empty (null)
    QVector<ConnectionPtrSet> const &getEntries(EPortType) const;
    QVector<ConnectionPtrSet> &getEntries(EPortType);

    inline ConnectionPtrSet connections(EPortType portType, PortIndex portIndex) const;

    void setConnection(EPortType portType, PortIndex portIndex, AprochConnection& connection);
    void eraseConnection(EPortType portType, PortIndex portIndex, QUuid id);

    EReactToConnectionState reaction() const;

    EPortType reactingPortType(void) const;

    SNodeDataType reactingDataType(void) const;

    void setReaction(EReactToConnectionState reaction, EPortType reactingPortType = EPortType::None, SNodeDataType reactingDataType = SNodeDataType());

//    bool isReacting(void) const { return mNodeState.IsResizing;}

    void setResizing(bool resizing) { mNodeState.IsResizing = resizing; }

    bool resizing(void) const;

    SNodeState const &getNodeState(void) const{return mNodeState;}

    QPoint getWidgetPosition();

public Q_SLOTS:

  /// Propagates incoming data to the underlying model.
  void propagateData(QSharedPointer<INodeData> nodeData, PortIndex inPortIndex) const;

  /// Fetches data from model's OUT #index port
  /// and propagates it to the connection
  void onDataUpdated(PortIndex index);

  /// update the graphic part if the size of the embeddedwidget changes
  void onNodeSizeUpdated();

private:
    void resize(void) const;

private:
    mutable int mWidth;
    mutable int mHeight;
    mutable int mPortWidth;
    int mSpacing;

    mutable QFontMetrics mFontMetrics;
    mutable QFontMetrics mBoldFontMetrics;

    bool mHovered;
    QPointF mDraggingPos;

    QUuid mUuid;

    SNodeState mNodeState;

    QScopedPointer<INodeDataModel> const &mNodeDataModel;
    QScopedPointer<AprochNodeGraphicsObject> mNodeGraphicsObject;
};

APROCH_NAMESPACE_END

#endif // APROCHNODE_H
