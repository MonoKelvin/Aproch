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
    AFlowScene(QSharedPointer<ADataModelRegistry> getRegistry, QObject *parent = nullptr);
    AFlowScene(QObject *parent = nullptr);

    ~AFlowScene();

public:
    static ANode *LocateNodeAt(QPointF scenePoint, AFlowScene &scene, const QTransform &viewTransform);

    AConnection *createConnection(EPortType connectedPort, ANode &node, PortIndex portIndex);

    AConnection *createConnection(ANode &nodeIn,
                                  PortIndex portIndexIn,
                                  ANode &nodeOut,
                                  PortIndex portIndexOut,
                                  const TypeConverter &converter = TypeConverter{});

    AConnection *restoreConnection(const QJsonObject &connectionJson);

    void deleteConnection(AConnection &connection);

    ANode &createNode(INodeDataModel *dataModel);

    ANode &restoreNode(const QJsonObject &nodeJson);

    void removeNode(ANode &node);

    inline QSharedPointer<ADataModelRegistry> &getRegistry()
    {
        return mRegistry;
    }

    inline void setRegistry(QSharedPointer<ADataModelRegistry> registry)
    {
        mRegistry = registry;
    }

    void iterateOverNodes(const std::function<void(ANode *)> &visitor);

    void iterateOverNodeData(const std::function<void(INodeDataModel *)> &visitor);

    void iterateOverNodeDataDependentOrder(const std::function<void(INodeDataModel *)> &visitor);

    QPointF getNodePosition(const ANode &node) const;

    void setNodePosition(ANode &node, const QPointF &pos) const;

    QSizeF getNodeSize(const ANode &node) const;

public:
    inline std::unordered_map<QUuid, ANode *> const &getNodes() const
    {
        return mNodes;
    }

    inline std::unordered_map<QUuid, AConnection *> const &getConnections() const
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

    std::unordered_map<QUuid, AConnection *> mConnections;
    std::unordered_map<QUuid, ANode *> mNodes;
    QSharedPointer<ADataModelRegistry> mRegistry;
};

APROCH_NAMESPACE_END

#endif // APROCHFLOWSCENE_H
