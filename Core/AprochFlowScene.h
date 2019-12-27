#ifndef APROCHFLOWSCENE_H
#define APROCHFLOWSCENE_H

#include "AprochPort.h"
#include "Utilities.h"

#include <QGraphicsScene>
#include <unordered_map>

APROCH_NAMESPACE_BEGIN

class AprochNode;
class AprochConnection;
class INodeDataModel;
class AprochDataModelRegistry;

class APROCH_EXPORT AprochFlowScene : public QGraphicsScene {
    Q_OBJECT
public:
    AprochFlowScene(QSharedPointer<AprochDataModelRegistry> registry, QObject *parent = nullptr);
    AprochFlowScene(QObject *parent = nullptr);

    ~AprochFlowScene();

public:
    static AprochNode *LocateNodeAt(QPointF scenePoint, AprochFlowScene &scene, const QTransform &viewTransform);

    QSharedPointer<AprochConnection> createConnection(EPortType connectedPort, AprochNode &node, PortIndex portIndex);

    QSharedPointer<AprochConnection> createConnection(AprochNode &nodeIn,
                                                      PortIndex portIndexIn,
                                                      AprochNode &nodeOut,
                                                      PortIndex portIndexOut,
                                                      const TypeConverter &converter = TypeConverter{});

    QSharedPointer<AprochConnection> restoreConnection(const QJsonObject &connectionJson);

    void deleteConnection(AprochConnection &connection);

    AprochNode &createNode(QScopedPointer<INodeDataModel> &&dataModel);

    AprochNode &restoreNode(const QJsonObject &nodeJson);

    void removeNode(AprochNode &node);

    AprochDataModelRegistry &registry() const;

    void setRegistry(QSharedPointer<AprochDataModelRegistry> registry);

    void iterateOverNodes(const std::function<void(AprochNode *)> &visitor);

    void iterateOverNodeData(const std::function<void(INodeDataModel *)> &visitor);

    void iterateOverNodeDataDependentOrder(const std::function<void(INodeDataModel *)> &visitor);

    QPointF getNodePosition(const AprochNode &node) const;

    void setNodePosition(AprochNode &node, const QPointF &pos) const;

    QSizeF getNodeSize(const AprochNode &node) const;

public:
    std::unordered_map<QUuid, QScopedPointer<AprochNode>> const &nodes() const;

    std::unordered_map<QUuid, QSharedPointer<AprochConnection>> const &connections() const;

    QVector<AprochNode *> allNodes() const;

    QVector<AprochNode *> selectedNodes() const;

public:
    void clearScene();

    void save() const;

    void load();

    QByteArray saveToMemory() const;

    void loadFromMemory(const QByteArray &data);

Q_SIGNALS:

    /**
    * @brief AprochNode has been created but not on the scene yet.
    * @see nodePlaced()
    */
    void nodeCreated(AprochNode &n);

    /**
    * @brief AprochNode has been added to the scene.
    * @details Connect to this signal if need a correct position of node.
    * @see nodeCreated()
    */
    void nodePlaced(AprochNode &n);

    void nodeDeleted(AprochNode &n);

    void connectionCreated(const AprochConnection &c);
    void connectionDeleted(const AprochConnection &c);

    void nodeMoved(AprochNode &n, const QPointF &newLocation);

    void nodeDoubleClicked(AprochNode &n);

    void connectionHovered(AprochConnection &c, QPoint screenPos);

    void nodeHovered(AprochNode &n, QPoint screenPos);

    void connectionHoverLeft(AprochConnection &c);

    void nodeHoverLeft(AprochNode &n);

    void nodeContextMenu(AprochNode &n, const QPointF &pos);

private:
    using SharedConnection = QSharedPointer<AprochConnection>;
    using UniqueNode = QScopedPointer<AprochNode>;

    std::unordered_map<QUuid, SharedConnection> _connections;
    std::unordered_map<QUuid, UniqueNode> _nodes;
    QSharedPointer<AprochDataModelRegistry> _registry;

private Q_SLOTS:

    void setupConnectionSignals(const AprochConnection &c);

    void sendConnectionCreatedToNodes(const AprochConnection &c);
    void sendConnectionDeletedToNodes(const AprochConnection &c);
};

APROCH_NAMESPACE_END

#endif // APROCHFLOWSCENE_H
