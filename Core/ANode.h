#ifndef APROCHNODE_H
#define APROCHNODE_H

#include "ANodeGraphicsObject.h"
#include "ANodeDataModel.h"
#include "AConnection.h"

#include <QUuid>
#include <QFontMetrics>
#include <QTransform>

APROCH_NAMESPACE_BEGIN

class ANodePainter;

using ConnectionPtrSet = std::unordered_map<QUuid, AConnection *>;

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

class APROCH_EXPORT ANode : public QObject, public ISerializable
{
    Q_OBJECT

    friend class ANodeGraphicsObject;
public:
    ANode(INodeDataModel *dataModel, QObject *parent = nullptr);
    virtual ~ANode(void) override = default;

public:
    QJsonObject save(void) const override;
    void restore(const QJsonObject &json) override;

public:
    QRectF boundingRect(void) const;

    /** 获得端口在场景中的位置
     * @param index 端口的索引
     * @param portType 端口的类型
     * @param transform 场景中的变换
     * @todo 不要使用transform了
     */
    QPointF getPortScenePosition(PortIndex index, EPortType portType, const QTransform &transform = QTransform()) const;

    /** 检查场景点的位置是否在该节点的某个端口中，在则返回端口的索引
     */
    PortIndex checkHitScenePoint(EPortType portType, QPointF scenePoint, const QTransform &sceneTransform = QTransform()) const;

    /** 获得节点的头部（可拖拽部位）的高度
     */
    int getCaptionHeight(void) const;

    /** 获得节点的头部（可拖拽部位）的宽度
     */
    int getCaptionWidth(void) const;

    /** 获得节点有效的高度
     */
    int getValidationHeight(void) const;

    /** 获得节点有效的宽度
     */
    int getValidationWidth(void) const;

    /** 获得节点等同于内嵌的widget的高度
     * @note 这个高度是指内嵌的widget不会让节点进行尺寸的调整时的高度
     */
    int getEquivalentWidgetHeight(void) const;

    /** 获得内嵌widget相对于节点的位置
     */
    QPointF getWidgetPosition(void) const;

    inline QUuid getId(void) const { return mUuid; }

    inline int getWidth(void) const { return mWidth; }
    inline void setWidth(int width) { mWidth = width; }

    inline int getHeight(void) const { return mHeight; }
    inline void setHeight(int height) { mHeight = height; }

    inline int getSpacing(void) const { return mSpacing; }
    inline void setSpacing(int spacing) { mSpacing = spacing; }

    inline bool isHovered(void) const { return mIsHovered; }
    inline void setHovered(bool isHovered) { mIsHovered = isHovered; }

    inline const ANodeGraphicsObject &getNodeGraphicsObject(void) const {return *mNodeGraphicsObject; }
    inline ANodeGraphicsObject *getNodeGraphicsObject(void) {return mNodeGraphicsObject; }
    inline void setNodeGraphicsObject(ANodeGraphicsObject *graphicsObj)
    {
        mNodeGraphicsObject = graphicsObj;
        resize();
    }

    inline INodeDataModel *getNodeDataModel(void) const { return mNodeDataModel; }
    inline void setNodeDataModel(INodeDataModel *dataModel) { mNodeDataModel = dataModel; }

    /** 获得输入端口还是输出端口的所有连线
     * @param portType 输入端口还是输出端口
     * @return QVector<ConnectionPtrSet> 返回每个端口的所有连线，可以为空
     */
    const QVector<ConnectionPtrSet> &getEntries(EPortType portType) const;

    /** 获得输入端口还是输出端口的所有连线
     * @param portType 输入端口还是输出端口
     * @return QVector<ConnectionPtrSet> 返回每个端口的所有连线，可以为空
     */
    QVector<ConnectionPtrSet> &getEntries(EPortType);

    /** 获得输某个端口的所有连线
     * @param portType 输入端口还是输出端口
     * @param portInDex 端口索引
     * @return ConnectionPtrSet 返回某个端口的所有连线，可以为空
     */
    ConnectionPtrSet getConnections(EPortType portType, PortIndex portIndex) const;

    /** 为该节点分配连线
     * @param portType 输入端口还是输出端口
     * @param portInDex 要创建连线的端口索引
     * @param 分配的连线
     */
    void setConnection(EPortType portType, PortIndex portIndex, AConnection &connection);

    /** 移除连线
     * @param portType 输入端口还是输出端口
     * @param portInDex 要创建连线的端口索引
     * @param 要移除连线的UUID
     */
    void eraseConnection(EPortType portType, PortIndex portIndex, QUuid id);

    SNodeState::EReactToConnectionState reaction() const;

    /** 对可能连接的连线做出反应
     * @param reactingPortType 能够做出反应的端口类型
     */
    void reactToPossibleConnection(EPortType reactingPortType, const SNodeDataType &reactingDataType, const QPointF &scenePoint);

    inline EPortType getReactingPortType(void) const { return mNodeState.ReactingPortType; }
    SNodeDataType getReactingDataType(void) const { return mNodeState.ReactingDataType; }

    void setReaction(SNodeState::EReactToConnectionState reaction,
                     EPortType reactingPortType = EPortType::None,
                     SNodeDataType reactingDataType = SNodeDataType());

    bool isReacting(void) const { return mNodeState.IsResizing;}

    void setResizing(bool resizing) { mNodeState.IsResizing = resizing; }
    inline bool resizing(void) const { return mNodeState.IsResizing; }

    SNodeState const &getNodeState(void) const {return mNodeState;}

    QPointF getWidgetPosition();

    void resetReactionToConnection();

    inline QPointF const &getDraggingPos() const { return mDraggingPos; }
    inline void setDraggingPosition(QPointF const &pos) { mDraggingPos = pos; }

    QRect resizeRect() const;

    void recalculateSize() const;

    void recalculateSize(const QFont &font) const;

public Q_SLOTS:

    /// Propagates incoming data to the underlying model.
    void propagateData(INodeData *nodeData, PortIndex inPortIndex) const;

    /// Fetches data from model's OUT #index port
    /// and propagates it to the connection
    void onDataUpdated(PortIndex index);

    /// update the graphic part if the size of the embeddedwidget changes
    void onNodeSizeUpdated();

private:
    void resize(void) const;

    int getPortWidth(EPortType portType) const;

private:
    QUuid mUuid;

    mutable int mWidth;
    mutable int mHeight;
    mutable int mPortWidth;
    int mSpacing;

    mutable QFontMetrics mFontMetrics;
    mutable QFontMetrics mBoldFontMetrics;

    bool mIsHovered;

    unsigned int mSinks;
    unsigned int mSources;

    QPointF mDraggingPos;
    SNodeState mNodeState;

    INodeDataModel *mNodeDataModel;
    ANodeGraphicsObject *mNodeGraphicsObject;
};

APROCH_NAMESPACE_END

#endif // APROCHNODE_H
