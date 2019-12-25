#include "AprochConnection.h"

#include "AprochNode.h"
#include "AprochNodeGraphicsObject.h"

APROCH_NAMESPACE_BEGIN


AprochConnection::AprochConnection(EPortType portType, AprochNode& node, PortIndex portIndex)
    : mUuid(QUuid::createUuid())
    , mOutPortIndex(INVALID_PORT_INDEX)
    , mInPortIndex(INVALID_PORT_INDEX)
    , mConnectionState()
{
    setNodeToPort(node, portType, portIndex);
    setRequiredPort(AprochPort::OppositePort(portType));
}

AprochConnection::AprochConnection(AprochNode& nodeIn, PortIndex portIndexIn, AprochNode& nodeOut, PortIndex portIndexOut, TypeConverter typeConverter)
    : mUuid(QUuid::createUuid())
    , mOutNode(&nodeOut)
    , mInNode(&nodeIn)
    , mOutPortIndex(portIndexOut)
    , mInPortIndex(portIndexIn)
    , mConnectionState()
    , mTypeConverter(std::move(typeConverter))
{
    setNodeToPort(nodeIn, EPortType::Input, portIndexIn);
    setNodeToPort(nodeOut, EPortType::Output, portIndexOut);
}

AprochConnection::~AprochConnection()
{
    if (complete())
    {
        connectionMadeIncomplete(*this);
    }

    propagateEmptyData();

    if (mInNode)
    {
        mInNode->getNodeGraphicsObject().update();
    }

    if (mOutNode)
    {
        mOutNode->getNodeGraphicsObject().update();
    }
}


QJsonObject AprochConnection::save() const
{
    QJsonObject connectionJson;

    if (mInNode && mOutNode)
    {
        connectionJson["in_id"] = mInNode->getId().toString();
        connectionJson["in_index"] = int(mInPortIndex);

        connectionJson["out_id"] = mOutNode->getId().toString();
        connectionJson["out_index"] = int(mOutPortIndex);

        if (mTypeConverter)
        {
            auto getTypeJson = [this](EPortType type)
            {
                QJsonObject typeJson;
                SNodeDataType nodeType = this->dataType(type);
                typeJson["id"] = nodeType.id;
                typeJson["name"] = nodeType.name;

                return typeJson;
            };

            QJsonObject converterTypeJson;

            converterTypeJson["in"] = getTypeJson(EPortType::Input);
            converterTypeJson["out"] = getTypeJson(EPortType::Output);
            connectionJson["converter"] = converterTypeJson;
        }
    }

    return connectionJson;
}

void AprochConnection::setRequiredPort(EPortType dragging)
{
    mRequiredPort = dragging;

    switch (dragging)
    {
    case EPortType::Output:
        mOutNode = nullptr;
        mOutPortIndex = INVALID_PORT_INDEX;
        break;

    case EPortType::Input:
        mInNode = nullptr;
        mInPortIndex = INVALID_PORT_INDEX;
        break;

    default:
        break;
    }
}

void AprochConnection::setGraphicsObject(QScopedPointer<AprochConnectionGraphicsObject>&& graphics)
{
    mConnectionGraphicsObject.reset(graphics.take());

    // This function is only called when the ConnectionGraphicsObject
    // is newly created. At this moment both end coordinates are (0, 0)
    // in AprochConnection G.O. coordinates. The position of the whole
    // AprochConnection G. O. in scene coordinate system is also (0, 0).
    // By moving the whole object to the Node Port position
    // we position both connection ends correctly.

    if (getRequiredPort() != EPortType::None)
    {

        EPortType attachedPort = AprochPort::OppositePort(getRequiredPort());

        PortIndex attachedPortIndex = getPortIndex(attachedPort);

        auto node = getNode(attachedPort);

        QTransform nodeSceneTransform =
        node->getNodeGraphicsObject().sceneTransform();

        QPointF pos = node->getPortScenePosition(attachedPortIndex, attachedPort, nodeSceneTransform);

        mConnectionGraphicsObject->setPos(pos);
    }

    mConnectionGraphicsObject.take();
}

PortIndex AprochConnection::getPortIndex(EPortType portType) const
{
    PortIndex result = INVALID_PORT_INDEX;

    switch (portType)
    {
    case EPortType::Input:
        result = mInPortIndex;
        break;
    case EPortType::Output:
        result = mOutPortIndex;
        break;
    default:
        break;
    }

    return result;
}


void
AprochConnection::setNodeToPort(AprochNode& node, EPortType portType, PortIndex portIndex)
{
    bool wasIncomplete = !complete();

    auto& nodeWeak = getNode(portType);

    nodeWeak = &node;

    if (portType == EPortType::Output)
    {
        mOutPortIndex = portIndex;
    }
    else
    {
        mInPortIndex = portIndex;
    }

    setNoRequiredPort();

    updated(*this);

    if (complete() && wasIncomplete)
    {
        connectionCompleted(*this);
    }
}


void AprochConnection::removeFromNodes() const
{
    if (mInNode)
    {
        mInNode->eraseConnection(EPortType::Input, mInPortIndex, getId());
    }
    if (mOutNode)
    {
        mOutNode->eraseConnection(EPortType::Output, mOutPortIndex, getId());
    }
}


AprochConnectionGraphicsObject &AprochConnection::getConnectionGraphicsObject() const
{
    return *mConnectionGraphicsObject;
}

AprochNode *AprochConnection::getNode(EPortType portType) const
{
    switch (portType)
    {
    case EPortType::Input:
        return mInNode;
        break;
    case EPortType::Output:
        return mOutNode;
        break;
    default:
        // not possible
        break;
    }
    return nullptr;
}


AprochNode *&AprochConnection::getNode(EPortType portType)
{
    switch (portType)
    {
    case EPortType::Input:
        return mInNode;
        break;
    case EPortType::Output:
        return mOutNode;
        break;
    default:
      // not possible
      break;
    }

    Q_UNREACHABLE();
}


void
AprochConnection::
clearNode(EPortType portType)
{
    if (complete())
    {
        connectionMadeIncomplete(*this);
    }

    getNode(portType) = nullptr;

    if (portType == EPortType::Input)
    {
        mInPortIndex = INVALID_PORT_INDEX;
    }
    else
    {
        mOutPortIndex = INVALID_PORT_INDEX;
    }
}


SNodeDataType AprochConnection::dataType(EPortType portType) const
{
    if (mInNode && mOutNode)
    {
        auto const & model = (portType == EPortType::Input) ? mInNode->getNodeDataModel() : mOutNode->getNodeDataModel();
        PortIndex index = (portType == EPortType::Input) ? mInPortIndex : mOutPortIndex;

        return model->dataType(portType, index);
    }
    else
    {
        AprochNode* validNode;
        PortIndex index = INVALID_PORT_INDEX;

        if ((validNode = mInNode))
        {
            index = mInPortIndex;
            portType = EPortType::Input;
        }
        else if ((validNode = mOutNode))
        {
            index = mOutPortIndex;
            portType = EPortType::Output;
        }

        if (validNode)
        {
            auto const &model = validNode->getNodeDataModel();
            return model->dataType(portType, index);
        }
    }

    Q_UNREACHABLE();
}


void AprochConnection::setTypeConverter(TypeConverter converter)
{
    mTypeConverter = std::move(converter);
}


void AprochConnection::propagateData(QSharedPointer<INodeData> nodeData) const
{
    if (mInNode)
    {
        if (mTypeConverter)
        {
            nodeData = mTypeConverter(nodeData);
        }

        mInNode->propagateData(nodeData, mInPortIndex);
    }
}


void AprochConnection::propagateEmptyData() const
{
    QSharedPointer<INodeData> emptyData;
    propagateData(emptyData);
}


APROCH_NAMESPACE_END
