﻿#include "AprochFlowScene.h"

#include "AprochNode.h"
#include "AprochNodeGraphicsObject.h"
#include "AprochConnection.h"
#include "AprochConnectionGraphicsObject.h"

#include <QSet>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>

APROCH_NAMESPACE_BEGIN

AprochNode* AprochFlowScene::LocateNodeAt(QPointF scenePoint, AprochFlowScene& scene, const QTransform& viewTransform)
{
    // items under cursor
    auto items = scene.items(scenePoint, Qt::IntersectsItemShape, Qt::DescendingOrder, viewTransform);

    // items convertable to AprochNodeGraphicsObject
    QVector<QGraphicsItem*> filteredItems;

    std::copy_if(items.begin(), items.end(), std::back_inserter(filteredItems), [](QGraphicsItem * item)
    {
        return (dynamic_cast<AprochNodeGraphicsObject*>(item) != nullptr);
    });

    AprochNode* resultNode = nullptr;

    if (!filteredItems.empty())
    {
        auto graphicsItem = filteredItems.front();
        auto ngo = dynamic_cast<AprochNodeGraphicsObject*>(graphicsItem);
        resultNode = &ngo->node();
    }

    return resultNode;
}

AprochFlowScene::AprochFlowScene(std::shared_ptr<AprochDataModelRegistry> registry, QObject* parent)
    : QGraphicsScene(parent)
    , mRegistry(std::move(registry))
{
    setItemIndexMethod(QGraphicsScene::NoIndex);

    // This connection should come first
    connect(this, &AprochFlowScene::connectionCreated, this, &AprochFlowScene::setupConnectionSignals);
    connect(this, &AprochFlowScene::connectionCreated, this, &AprochFlowScene::sendConnectionCreatedToNodes);
    connect(this, &AprochFlowScene::connectionDeleted, this, &AprochFlowScene::sendConnectionDeletedToNodes);
}

AprochFlowScene::AprochFlowScene(QObject* parent)
    : AprochFlowScene(std::shared_ptr<AprochDataModelRegistry>(), parent)
{
}

AprochFlowScene::~AprochFlowScene()
{
    clearScene();
}

//------------------------------------------------------------------------------

std::shared_ptr<AprochConnection> AprochFlowScene::createConnection(EPortType connectedPort,
                                                                    AprochNode& node,
                                                                    PortIndex portIndex)
{
    auto connection = std::make_shared<AprochConnection>(connectedPort, node, portIndex);

    auto cgo = make_unique<AprochConnectionGraphicsObject>(*this, *connection);

    // after this function connection points are set to node port
    connection->setGraphicsObject(std::move(cgo));

    mConnections[connection->getId()] = connection;

    // Note: this connection isn't truly created yet. It's only partially created.
    // Thus, don't send the connectionCreated(...) signal.

    connect(connection.get(), &AprochConnection::connectionCompleted,
            this, [this](const AprochConnection & c)
    {
        connectionCreated(c);
    });

    return connection;
}

std::shared_ptr<AprochConnection> AprochFlowScene::createConnection(AprochNode& nodeIn, PortIndex portIndexIn, AprochNode& nodeOut, PortIndex portIndexOut, TypeConverter const& converter)
{
    auto connection = std::make_shared<AprochConnection>(nodeIn, portIndexIn, nodeOut, portIndexOut, converter);

    auto cgo = make_unique<AprochConnectionGraphicsObject>(*this, *connection);

    nodeIn.setConnection(EPortType::Input, portIndexIn, *connection);
    nodeOut.setConnection(EPortType::Output, portIndexOut, *connection);

    // after this function connection points are set to node port
    connection->setGraphicsObject(std::move(cgo));

    // trigger data propagation
    nodeOut.onDataUpdated(portIndexOut);

    mConnections[connection->getId()] = connection;

    connectionCreated(*connection);

    return connection;
}

std::shared_ptr<AprochConnection> AprochFlowScene::restoreConnection(QJsonObject const& connectionJson)
{
    QUuid nodeInId = QUuid(connectionJson["in_id"].toString());
    QUuid nodeOutId = QUuid(connectionJson["out_id"].toString());

    PortIndex portIndexIn = unsigned(connectionJson["in_index"].toInt());
    PortIndex portIndexOut = unsigned(connectionJson["out_index"].toInt());

    auto nodeIn = mNodes[nodeInId].get();
    auto nodeOut = mNodes[nodeOutId].get();

    auto getConverter = [&]()
    {
        QJsonValue converterVal = connectionJson["converter"];

        if (!converterVal.isUndefined())
        {
            QJsonObject converterJson = converterVal.toObject();

            SNodeDataType inType{converterJson["in"].toObject()["id"].toString(),
                                 converterJson["in"].toObject()["name"].toString()};

            SNodeDataType outType{converterJson["out"].toObject()["id"].toString(),
                                  converterJson["out"].toObject()["name"].toString()};

            auto converter = registry().getTypeConverter(outType, inType);

            if (converter)
            {
                return converter;
            }
        }

        return TypeConverter{};
    };

    std::shared_ptr<AprochConnection> connection = createConnection(*nodeIn, portIndexIn, *nodeOut, portIndexOut, getConverter());
    // Note: the connectionCreated(...) signal has already been sent
    // by createConnection(...)

    return connection;
}

void AprochFlowScene::deleteConnection(AprochConnection& connection)
{
    auto it = mConnections.find(connection.getId());
    if (it != mConnections.end())
    {
        connection.removeFromNodes();
        mConnections.erase(it);
    }
}

AprochNode& AprochFlowScene::createNode(std::unique_ptr<INodeDataModel>&& dataModel)
{
    auto node = make_unique<AprochNode>(std::move(dataModel));
    auto ngo = make_unique<AprochNodeGraphicsObject>(*this, *node);

    node->setNodeGraphicsObject(std::move(ngo));

    auto nodePtr = node.get();
    mNodes[node->getId()] = std::move(node);

    nodeCreated(*nodePtr);
    return *nodePtr;
}

AprochNode& AprochFlowScene::restoreNode(QJsonObject const& nodeJson)
{
    QString modelName = nodeJson["model"].toObject()["name"].toString();

    auto dataModel = registry().create(modelName);

    if (!dataModel)
    {
        throw std::logic_error(std::string("No registered model with name ") + modelName.toLocal8Bit().data());
    }

    auto node = make_unique<AprochNode>(std::move(dataModel));
    auto ngo = make_unique<AprochNodeGraphicsObject>(*this, *node);
    node->setGraphicsObject(std::move(ngo));

    node->restore(nodeJson);

    auto nodePtr = node.get();
    mNodes[node->id()] = std::move(node);

    nodePlaced(*nodePtr);
    nodeCreated(*nodePtr);
    return *nodePtr;
}

void AprochFlowScene::removeNode(AprochNode& node)
{
    // call signal
    nodeDeleted(node);

    for (auto portType : {EPortType::Input, EPortType::Output})
    {
        auto const& nodeEntries = node.getEntries(portType);

        for (auto& connections : nodeEntries)
        {
            for (auto const& pair : connections)
            {
                deleteConnection(*pair.second);
            }
        }
    }

    mNodes.erase(node.getId());
}

AprochDataModelRegistry& AprochFlowScene::registry() const
{
    return *mRegistry;
}

void AprochFlowScene::setRegistry(std::shared_ptr<AprochDataModelRegistry> registry)
{
    mRegistry = std::move(registry);
}

void AprochFlowScene::iterateOverNodes(const std::function<void(AprochNode*)>& visitor)
{
    for (const auto& _node : mNodes)
    {
        visitor(_node.second.get());
    }
}

void AprochFlowScene::iterateOverNodeData(const std::function<void(INodeDataModel*)>& visitor)
{
    for (const auto& _node : mNodes)
    {
        visitor(_node.second->getNodeDataModel());
    }
}

void AprochFlowScene::iterateOverNodeDataDependentOrder(const std::function<void(INodeDataModel*)>& visitor)
{
    QSet<QUuid> visitedNodesSet;

    //A leaf node is a node with no input ports, or all possible input ports empty
    auto isNodeLeaf = [](AprochNode const & node, INodeDataModel const & model)
    {
        for (unsigned int i = 0; i < model.nPorts(EPortType::Input); ++i)
        {
            auto connections = node.connections(EPortType::Input, i);
            if (!connections.empty())
            {
                return false;
            }
        }

        return true;
    };

    //Iterate over "leaf" nodes
    for (auto const& _node : mNodes)
    {
        auto const& node = _node.second;
        auto model = node->getNodeDataModel();

        if (isNodeLeaf(*node, *model))
        {
            visitor(model);
            visitedNodesSet.insert(node->getId());
        }
    }

    auto areNodeInputsVisitedBefore = [&](AprochNode const & node, INodeDataModel const & model)
    {
        for (unsigned int i = 0; i < model.nPorts(EPortType::Input); ++i)
        {
            auto connections = node.connections(EPortType::Input, i);

            for (auto& conn : connections)
            {
                if (visitedNodesSet.find(conn.second->getNode(EPortType::Output)->getId()) == visitedNodesSet.end())
                {
                    return false;
                }
            }
        }

        return true;
    };

    //Iterate over dependent nodes
    while (int(mNodes.size()) != visitedNodesSet.size())
    {
        for (auto const& _node : mNodes)
        {
            auto const& node = _node.second;
            if (visitedNodesSet.find(node->getId()) != visitedNodesSet.end())
            {
                continue;
            }

            auto model = node->getNodeDataModel();

            if (areNodeInputsVisitedBefore(*node, *model))
            {
                visitor(model);
                visitedNodesSet.insert(node->getId());
            }
        }
    }
}

QPointF AprochFlowScene::getNodePosition(const AprochNode& node) const
{
    return node.getNodeGraphicsObject().pos();
}

void AprochFlowScene::setNodePosition(AprochNode& node, const QPointF& pos) const
{
    node.getNodeGraphicsObject().setPos(pos);
    node.getNodeGraphicsObject().moveConnections();
}

QSizeF AprochFlowScene::getNodeSize(const AprochNode& node) const
{
    return QSizeF(node.getWidth(), node.getHeight());
}

QVector<AprochNode*> AprochFlowScene::allNodes() const
{
    QVector<AprochNode*> nodes;

    std::transform(mNodes.begin(), mNodes.end(), std::back_inserter(nodes),
                   [](std::pair<QUuid const, std::unique_ptr<AprochNode>> const & p)
    {
        return p.second.get();
    });

    return nodes;
}

QVector<AprochNode*> AprochFlowScene::selectedNodes() const
{
    QList<QGraphicsItem*> graphicsItems = selectedItems();

    QVector<AprochNode*> ret;
    ret.reserve(graphicsItems.size());

    for (QGraphicsItem* item : graphicsItems)
    {
        auto ngo = qgraphicsitem_cast<AprochNodeGraphicsObject*>(item);

        if (ngo != nullptr)
        {
            ret.push_back(&ngo->node());
        }
    }

    return ret;
}

//------------------------------------------------------------------------------

void AprochFlowScene::clearScene()
{
    //Manual node cleanup. Simply clearing the holding datastructures doesn't work, the code crashes when
    // there are both nodes and connections in the scene. (The data propagation internal logic tries to propagate
    // data through already freed connections.)
    while (mConnections.size() > 0)
    {
        deleteConnection(*mConnections.begin()->second);
    }

    while (mNodes.size() > 0)
    {
        removeNode(*mNodes.begin()->second);
    }
}

void AprochFlowScene::save() const
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, tr("Open Flow Scene"), QDir::homePath(), tr("Flow Scene Files (*.flow)"));

    if (!fileName.isEmpty())
    {
        if (!fileName.endsWith("flow", Qt::CaseInsensitive))
        {
            fileName += ".flow";
        }

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(saveToMemory());
        }
    }
}

void AprochFlowScene::load()
{
    clearScene();

    //-------------

    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Flow Scene"), QDir::homePath(), tr("Flow Scene Files (*.flow)"));

    if (!QFileInfo::exists(fileName))
    {
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray wholeFile = file.readAll();

    loadFromMemory(wholeFile);
}

QByteArray AprochFlowScene::saveToMemory() const
{
    QJsonObject sceneJson;

    QJsonArray nodesJsonArray;

    for (auto const& pair : mNodes)
    {
        auto const& node = pair.second;

        nodesJsonArray.append(node->save());
    }

    sceneJson["nodes"] = nodesJsonArray;

    QJsonArray connectionJsonArray;
    for (auto const& pair : mConnections)
    {
        auto const& connection = pair.second;

        QJsonObject connectionJson = connection->save();

        if (!connectionJson.isEmpty())
        {
            connectionJsonArray.append(connectionJson);
        }
    }

    sceneJson["connections"] = connectionJsonArray;

    QJsonDocument document(sceneJson);

    return document.toJson();
}

void AprochFlowScene::loadFromMemory(const QByteArray& data)
{
    QJsonObject const jsonDocument = QJsonDocument::fromJson(data).object();

    QJsonArray nodesJsonArray = jsonDocument["nodes"].toArray();

    for (QJsonValueRef node : nodesJsonArray)
    {
        restoreNode(node.toObject());
    }

    QJsonArray connectionJsonArray = jsonDocument["connections"].toArray();

    for (QJsonValueRef connection : connectionJsonArray)
    {
        restoreConnection(connection.toObject());
    }
}

void AprochFlowScene::setupConnectionSignals(AprochConnection const& c)
{
    connect(&c, &AprochConnection::connectionMadeIncomplete, this, &AprochFlowScene::connectionDeleted, Qt::UniqueConnection);
}

void AprochFlowScene::sendConnectionCreatedToNodes(const AprochConnection& c)
{
    AprochNode* from = c.getNode(EPortType::Output);
    AprochNode* to = c.getNode(EPortType::Input);

    Q_ASSERT(from != nullptr);
    Q_ASSERT(to != nullptr);

    from->getNodeDataModel()->outputConnectionCreated(c);
    to->getNodeDataModel()->inputConnectionCreated(c);
}

void AprochFlowScene::sendConnectionDeletedToNodes(const AprochConnection& c)
{
    AprochNode* from = c.getNode(EPortType::Output);
    AprochNode* to = c.getNode(EPortType::Input);

    Q_ASSERT(from != nullptr);
    Q_ASSERT(to != nullptr);

    from->getNodeDataModel()->outputConnectionDeleted(c);
    to->getNodeDataModel()->inputConnectionDeleted(c);
}

APROCH_NAMESPACE_END
