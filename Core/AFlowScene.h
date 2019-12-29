#ifndef APROCHFLOWSCENE_H
#define APROCHFLOWSCENE_H

#include "APort.h"
#include "Utilities.h"

#include <QGraphicsScene>
#include <unordered_map>

APROCH_NAMESPACE_BEGIN

class ANode;
class AConnection;
class INodeDataModel;
class ADataModelRegistry;

class APROCH_EXPORT AFlowScene : public QGraphicsScene
{
    Q_OBJECT
public:
    AFlowScene(std::shared_ptr<ADataModelRegistry> registry, QObject *parent = nullptr);
    AFlowScene(QObject *parent = nullptr);

    ~AFlowScene();

public:
    static ANode *LocateNodeAt(QPointF scenePoint, AFlowScene &scene, const QTransform &viewTransform);

    std::shared_ptr<AConnection> createConnection(EPortType connectedPort, ANode &node, PortIndex portIndex);

    std::shared_ptr<AConnection> createConnection(ANode &nodeIn,
                                                       PortIndex portIndexIn,
                                                       ANode &nodeOut,
                                                       PortIndex portIndexOut,
                                                       const TypeConverter &converter = TypeConverter{});

    std::shared_ptr<AConnection> restoreConnection(const QJsonObject &connectionJson);

    void deleteConnection(AConnection &connection);

    ANode &createNode(std::unique_ptr<INodeDataModel> &&dataModel);

    ANode &restoreNode(const QJsonObject &nodeJson);

    void removeNode(ANode &node);

    ADataModelRegistry &registry() const;

    void setRegistry(std::shared_ptr<ADataModelRegistry> registry);

    void iterateOverNodes(const std::function<void(ANode *)> &visitor);

    void iterateOverNodeData(const std::function<void(INodeDataModel *)> &visitor);

    void iterateOverNodeDataDependentOrder(const std::function<void(INodeDataModel *)> &visitor);

    QPointF getNodePosition(const ANode &node) const;

    void setNodePosition(ANode &node, const QPointF &pos) const;

    QSizeF getNodeSize(const ANode &node) const;

public:
    inline std::unordered_map<QUuid, std::unique_ptr<ANode>> const &getNodes() const
    {
        return mNodes;
    }

    inline std::unordered_map<QUuid, std::shared_ptr<AConnection>> const &getConnections() const
    {
        return mConnections;
    }

    QVector<ANode *> allNodes() const;

    QVector<ANode *> selectedNodes() const;

public:
    void clearScene();

    void save() const;

    void load();

    QByteArray saveToMemory() const;

    void loadFromMemory(const QByteArray &data);

Q_SIGNALS:

    /**
    * @brief ANode has been created but not on the scene yet.
    * @see nodePlaced()
    */
    void nodeCreated(ANode &n);

    /**
    * @brief ANode has been added to the scene.
    * @details Connect to this signal if need a correct position of node.
    * @see nodeCreated()
    */
    void nodePlaced(ANode &n);

    void nodeDeleted(ANode &n);

    void connectionCreated(const AConnection &c);
    void connectionDeleted(const AConnection &c);

    void nodeMoved(ANode &n, const QPointF &newLocation);

    void nodeDoubleClicked(ANode &n);

    void connectionHovered(AConnection &c, QPoint screenPos);

    void nodeHovered(ANode &n, QPoint screenPos);

    void connectionHoverLeft(AConnection &c);

    void nodeHoverLeft(ANode &n);

    void nodeContextMenu(ANode &n, const QPointF &pos);

private Q_SLOTS:
    void setupConnectionSignals(const AConnection &c);
    void sendConnectionCreatedToNodes(const AConnection &c);
    void sendConnectionDeletedToNodes(const AConnection &c);

private:
    using SharedConnection = std::shared_ptr<AConnection>;
    using UniqueNode = std::unique_ptr<ANode>;

    std::unordered_map<QUuid, SharedConnection> mConnections;
    std::unordered_map<QUuid, UniqueNode> mNodes;
    std::shared_ptr<ADataModelRegistry> mRegistry;
};

APROCH_NAMESPACE_END

#endif // APROCHFLOWSCENE_H
