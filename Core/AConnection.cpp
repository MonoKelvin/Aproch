#include "AConnection.h"

#include "ANode.h"
#include "AConnectionGraphicsObject.h"

APROCH_NAMESPACE_BEGIN

AConnection::AConnection(EPortType portType, ANode *node, PortIndex portIndex, QObject *parent)
    : QObject(parent)
    , mUuid(QUuid::createUuid())
    , mInPoint(0, 0)
    , mOutPoint(0, 0)
    , mLineWidth(3.0)
    , mIsHovered(false)
    , mOutPortIndex(INVALID_PORT_INDEX)
    , mInPortIndex(INVALID_PORT_INDEX)
    , mConnectionState()
{
    setNodeToPort(node, portType, portIndex);
    setRequiredPort(APort::OppositePort(portType));
    qDebug() << "new Connection:" << this;
}

AConnection::AConnection(ANode *nodeIn, PortIndex portIndexIn, ANode *nodeOut, PortIndex portIndexOut, TypeConverter typeConverter, QObject *parent)
    : QObject(parent)
    , mUuid(QUuid::createUuid())
    , mOutNode(nodeOut)
    , mInNode(nodeIn)
    , mOutPortIndex(portIndexOut)
    , mInPortIndex(portIndexIn)
    , mConnectionState()
    , mTypeConverter(std::move(typeConverter))
{
    setNodeToPort(nodeIn, EPortType::Input, portIndexIn);
    setNodeToPort(nodeOut, EPortType::Output, portIndexOut);
}

AConnection::~AConnection()
{
    if (complete())
    {
        emit connectionMadeIncomplete(*this);
    }

    propagateEmptyData();

    if (mInNode)
    {
        mInNode->getNodeGraphicsObject()->update();
    }

    if (mOutNode)
    {
        mOutNode->getNodeGraphicsObject()->update();
    }

    resetLastHoveredNode();
}

QJsonObject AConnection::save() const
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
                typeJson["id"] = nodeType.ID;
                typeJson["name"] = nodeType.Name;

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

void AConnection::setRequiredPort(EPortType dragging)
{
    mConnectionState.RequiredPort = dragging;

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

void AConnection::setGraphicsObject(AConnectionGraphicsObject *graphics)
{
    mConnectionGraphicsObject = graphics;

    // This function is only called when the ConnectionGraphicsObject
    // is newly created. At this moment both end coordinates are (0, 0)
    // in AConnection G.O. coordinates. The position of the whole
    // AConnection G. O. in scene coordinate system is also (0, 0).
    // By moving the whole object to the Node Port position
    // we position both connection ends correctly.

    if (getRequiredPort() != EPortType::None)
    {

        EPortType attachedPort = APort::OppositePort(getRequiredPort());

        PortIndex attachedPortIndex = getPortIndex(attachedPort);

        auto node = getNode(attachedPort);

        QTransform nodeSceneTransform = node->getNodeGraphicsObject()->sceneTransform();

        QPointF pos = node->getPortScenePosition(attachedPortIndex, attachedPort, nodeSceneTransform);

        mConnectionGraphicsObject->setPos(pos);
    }

    mConnectionGraphicsObject->move();
}

PortIndex AConnection::getPortIndex(EPortType portType) const
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

void AConnection::setNodeToPort(ANode *node, EPortType portType, PortIndex portIndex)
{
    bool wasIncomplete = !complete();

    auto &nodeWeak = getNode(portType);

    nodeWeak = node;

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

void AConnection::removeFromNodes() const
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

QPointF const &AConnection::getEndPoint(EPortType portType) const
{
    Q_ASSERT(portType != EPortType::None);

    return (portType == EPortType::Output ? mOutPoint : mInPoint);
}

void AConnection::setEndPoint(EPortType portType, QPointF const &point)
{
    switch (portType)
    {
    case EPortType::Output:
        mOutPoint = point;
        break;

    case EPortType::Input:
        mInPoint = point;
        break;

    default:
        break;
    }
}

void AConnection::moveEndPoint(EPortType portType, QPointF const &offset)
{
    switch (portType)
    {
    case EPortType::Output:
        mOutPoint += offset;
        break;

    case EPortType::Input:
        mInPoint += offset;
        break;

    default:
        break;
    }
}

QRectF AConnection::boundingRect() const
{
    auto points = pointsC1C2();

    QRectF basicRect = QRectF(mOutPoint, mInPoint).normalized();

    QRectF c1c2Rect = QRectF(points.first, points.second).normalized();

    auto const &connectionStyle = AStyle::GetConnectionStyle();

    double const diam = double(connectionStyle.PointDiameter);

    QRectF commonRect = basicRect.united(c1c2Rect);

    QPointF const cornerOffset(diam, diam);

    commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
    commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);

    return commonRect;
}

std::pair<QPointF, QPointF> AConnection::pointsC1C2() const
{
    const double defaultOffset = 200;

    double xDistance = mInPoint.x() - mOutPoint.x();

    double horizontalOffset = qMin(defaultOffset, std::abs(xDistance));

    double verticalOffset = 0;

    double ratioX = 0.5;

    if (xDistance <= 0)
    {
        double yDistance = mInPoint.y() - mOutPoint.y() + 20;

        double vector = yDistance < 0 ? -1.0 : 1.0;

        verticalOffset = qMin(defaultOffset, std::abs(yDistance)) * vector;

        ratioX = 1.0;
    }

    horizontalOffset *= ratioX;

    QPointF c1(mOutPoint.x() + horizontalOffset, mOutPoint.y() + verticalOffset);
    QPointF c2(mInPoint.x() - horizontalOffset, mInPoint.y() - verticalOffset);

    return std::pair<QPointF, QPointF>(c1, c2);
}

ANode *AConnection::getNode(EPortType portType) const
{
    switch (portType)
    {
    case EPortType::Input:
        return mInNode;
//        break;
    case EPortType::Output:
        return mOutNode;
//        break;
    default:
        // not possible
        break;
    }

    return nullptr;
}

ANode *&AConnection::getNode(EPortType portType)
{
    switch (portType)
    {
    case EPortType::Input:
        return mInNode;
//        break;
    case EPortType::Output:
        return mOutNode;
//        break;
    default:
        // not possible
        break;
    }

    Q_UNREACHABLE();
}

void AConnection::
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

SNodeDataType AConnection::dataType(EPortType portType) const
{
    if (mInNode && mOutNode)
    {
        auto const &model = (portType == EPortType::Input) ? mInNode->getNodeDataModel() : mOutNode->getNodeDataModel();
        PortIndex index = (portType == EPortType::Input) ? mInPortIndex : mOutPortIndex;

        return model->dataType(portType, index);
    }
    else
    {
        ANode *validNode;
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
            return validNode->getNodeDataModel()->dataType(portType, index);
        }
    }

    Q_UNREACHABLE();
}

void AConnection::setTypeConverter(TypeConverter converter)
{
    mTypeConverter = std::move(converter);
}

void AConnection::propagateData(INodeData *nodeData) const
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

void AConnection::propagateEmptyData() const
{
    propagateData(nullptr);
}

void AConnection::interactWithNode(ANode *node)
{
    if (node)
    {
        mConnectionState.LastHoveredNode = node;
    }
    else
    {
        resetLastHoveredNode();
    }
}

void AConnection::resetLastHoveredNode()
{
    if (mConnectionState.LastHoveredNode)
    {
        mConnectionState.LastHoveredNode->resetReactionToConnection();
    }

    mConnectionState.LastHoveredNode = nullptr;
}


APROCH_NAMESPACE_END
