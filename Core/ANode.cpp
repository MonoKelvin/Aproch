#include "ANode.h"

#include "AConnectionGraphicsObject.h"

#include <QWidget>
#include <QVector>
#include <QDebug>

APROCH_NAMESPACE_BEGIN

ANode::ANode(INodeDataModel *dataModel, QObject *parent)
    : QObject(parent)
    , mUuid(QUuid::createUuid())
    , mWidth(100)
    , mHeight(150)
    , mPortWidth(30)
    , mSpacing(20)
    , mFontMetrics(QFont())
    , mBoldFontMetrics(QFont())
    , mIsHovered(false)
    , mDraggingPos(-1000, -1000)
    , mNodeDataModel(std::move(dataModel))
{
    mSources = dataModel->nPorts(EPortType::Output);
    mSinks = dataModel->nPorts(EPortType::Input);

    mNodeState.InConnections.resize(int(mNodeDataModel->nPorts(EPortType::Input)));
    mNodeState.OutConnections.resize(int(mNodeDataModel->nPorts(EPortType::Output)));
    mNodeState.Reaction = SNodeState::EReactToConnectionState::NOT_REACTING;
    mNodeState.ReactingPortType = EPortType::None;
    mNodeState.IsResizing = false;

    QFont f;
    f.setBold(true);

    mBoldFontMetrics = QFontMetrics(f);

    recalculateSize();

    // propagate data: model => node
    connect(mNodeDataModel, &INodeDataModel::dataUpdated, this, &ANode::onDataUpdated);
    connect(mNodeDataModel, &INodeDataModel::embeddedWidgetSizeUpdated, this, &ANode::onNodeSizeUpdated);
}

ANode::~ANode()
{

}

QRectF ANode::boundingRect(void) const
{
    double addon = double(AStyle::GetNodeStyle().ConnectionPointDiameter);

    return QRectF(-addon, -addon, mWidth + 2 * addon, mHeight + 2 * addon);
}

void ANode::resize(void) const
{
    if (auto w = mNodeDataModel->getEmbeddedWidget())
    {
        mHeight = AMax(mHeight, w->height());
    }

    mHeight += getCaptionHeight();

    mWidth = (mPortWidth + mSpacing) * 2;

    if (auto w = mNodeDataModel->getEmbeddedWidget())
    {
        mWidth += w->width();
    }

    mWidth = AMax(mWidth, getCaptionWidth());

    if (mNodeDataModel->validationState() != ENodeValidationState::Valid)
    {
        mWidth = AMax(mWidth, getValidationWidth());
        mHeight += getValidationHeight() + mSpacing;
    }
}

QRect ANode::resizeRect() const
{
    const int rectSize = 7;
    return QRect(mWidth - rectSize, mHeight - rectSize, rectSize, rectSize);
}


void ANode::recalculateSize() const
{
    mHeight = mSpacing * int(AMax(mSinks, mSources));

    if (auto w = mNodeDataModel->getEmbeddedWidget())
    {
        mHeight = AMax(mHeight, w->height());
    }

    mHeight += getCaptionHeight();

    mPortWidth = getPortWidth(EPortType::Input);

    mWidth = (mPortWidth + mSpacing) * 2;

    if (auto w = mNodeDataModel->getEmbeddedWidget())
    {
        mWidth += w->width();
    }

    mWidth = AMax(mWidth, getCaptionWidth());

    if (mNodeDataModel->validationState() != ENodeValidationState::Valid)
    {
        mWidth = AMax(mWidth, getValidationWidth());
        mHeight += getValidationHeight() + mSpacing;
    }
}

void ANode::recalculateSize(QFont const &font) const
{
    QFontMetrics fontMetrics(font);
    QFont boldFont = font;

    boldFont.setBold(true);

    QFontMetrics boldFontMetrics(boldFont);

    if (mBoldFontMetrics != boldFontMetrics)
    {
        mFontMetrics = fontMetrics;
        mBoldFontMetrics = boldFontMetrics;

        recalculateSize();
    }
}

int ANode::getPortWidth(EPortType portType) const
{
    int width = 0;

    for (auto i = 0u; i < mNodeDataModel->nPorts(portType); ++i)
    {
        QString name;

        if (mNodeDataModel->isPortCaptionVisible(portType, i))
        {
            name = mNodeDataModel->portCaption(portType, i);
        }
        else
        {
            name = mNodeDataModel->dataType(portType, i).Name;
        }

        // width(name) is deprecated, so use horizontalAdvance(name).
        width = AMax(mFontMetrics.horizontalAdvance(name), width);
    }

    return width;
}

QPointF ANode::getPortScenePosition(PortIndex index, EPortType portType, const QTransform &transform) const
{
    auto const &nodeStyle = AStyle::GetNodeStyle();

    QPointF result;

    double totalHeight = getCaptionHeight();
    totalHeight += (double(mSpacing) * index) + mSpacing * 0.5;

    switch (portType)
    {
    case EPortType::Output:
    {
        float x = mWidth + nodeStyle.ConnectionPointDiameter;
        result = QPointF(double(x), totalHeight);
        break;
    }
    case EPortType::Input:
    {
        double x = double(-nodeStyle.ConnectionPointDiameter);
        result = QPointF(x, totalHeight);
        break;
    }
    default:
        break;
    }

    return transform.map(result);
}

PortIndex ANode::checkHitScenePoint(EPortType portType, QPointF scenePoint, const QTransform &sceneTransform) const
{
    auto const &nodeStyle = AStyle::GetNodeStyle();

    PortIndex result = INVALID_PORT_INDEX;

    if (portType == EPortType::None)
    {
        return result;
    }

    float const tolerance = 2.0f * nodeStyle.ConnectionPointDiameter;

    unsigned int const nItems = mNodeDataModel->nPorts(portType);

    for (unsigned int i = 0; i < nItems; ++i)
    {
        auto pp = getPortScenePosition(i, portType, sceneTransform);

        QPointF p = pp - scenePoint;
        double distanceSqrt = QPointF::dotProduct(p, p);

        if (distanceSqrt < double(tolerance * tolerance))
        {
            result = PortIndex(i);
            break;
        }
    }

    return result;
}

int ANode::getCaptionHeight(void) const
{
    if (!mNodeDataModel->isCaptionVisible())
    {
        return 0;
    }

    QString name = mNodeDataModel->caption();

    return mBoldFontMetrics.boundingRect(name).height();
}

int ANode::getCaptionWidth(void) const
{
    if (!mNodeDataModel->isCaptionVisible())
    {
        return 0;
    }

    QString name = mNodeDataModel->caption();

    return mBoldFontMetrics.boundingRect(name).width();
}

int ANode::getValidationHeight(void) const
{
    return mBoldFontMetrics.boundingRect(mNodeDataModel->validationMessage()).height();
}

int ANode::getValidationWidth(void) const
{
    return mBoldFontMetrics.boundingRect(mNodeDataModel->validationMessage()).width();
}

int ANode::getEquivalentWidgetHeight(void) const
{
    if (mNodeDataModel->validationState() != ENodeValidationState::Valid)
    {
        return getHeight() - getCaptionHeight() + getValidationHeight();
    }

    return getHeight() - getCaptionHeight();
}

QPointF ANode::getWidgetPosition(void) const
{
    if (auto w = mNodeDataModel->getEmbeddedWidget())
    {
        if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag)
        {
            // If the widget wants to use as much vertical space as possible, place it immediately after the caption.
            return QPointF(mSpacing + getPortWidth(EPortType::Input), getCaptionHeight());
        }
        else
        {
            if (mNodeDataModel->validationState() != ENodeValidationState::Valid)
            {
                return QPointF(mSpacing + getPortWidth(EPortType::Input), (getCaptionHeight() + mHeight - getValidationHeight() - mSpacing - w->height()) / 2.0);
            }

            return QPointF(mSpacing + getPortWidth(EPortType::Input), (getCaptionHeight() + mHeight - w->height()) / 2.0);
        }
    }
    return QPointF();
}

QJsonObject ANode::save(void) const
{
    QJsonObject nodeJson;
    nodeJson["id"] = mUuid.toString();
    nodeJson["model"] = mNodeDataModel->save();

    QJsonObject obj;
    obj["x"] = mNodeGraphicsObject->pos().x();
    obj["y"] = mNodeGraphicsObject->pos().y();
    nodeJson["position"] = obj;

    return nodeJson;
}

void ANode::restore(const QJsonObject &json)
{
    mUuid = QUuid(json["id"].toString());

    QJsonObject positionJson = json["position"].toObject();
    QPointF point(positionJson["x"].toDouble(), positionJson["y"].toDouble());

    mNodeGraphicsObject->setPos(point);
    mNodeDataModel->restore(json["model"].toObject());
}

QVector<ConnectionPtrSet> const &ANode::getEntries(EPortType portType) const
{
    if (portType == EPortType::Input)
    {
        return mNodeState.InConnections;
    }
    else
    {
        return mNodeState.OutConnections;
    }
}

QVector<ConnectionPtrSet> &ANode::getEntries(EPortType portType)
{
    if (portType == EPortType::Input)
    {
        return mNodeState.InConnections;
    }
    else
    {
        return mNodeState.OutConnections;
    }
}

ConnectionPtrSet ANode::getConnections(EPortType portType, PortIndex portIndex) const
{
    return getEntries(portType)[int(portIndex)];
}

void ANode::setConnection(EPortType portType, PortIndex portIndex, AConnection &connection)
{
    auto &connections = getEntries(portType);
    connections[int(portIndex)].insert(std::make_pair(connection.getId(), &connection));
}

void ANode::eraseConnection(EPortType portType, PortIndex portIndex, QUuid id)
{
    getEntries(portType)[int(portIndex)].erase(id);
}

void ANode::reactToPossibleConnection(EPortType reactingPortType, const SNodeDataType &reactingDataType, const QPointF &scenePoint)
{
    QTransform const t = mNodeGraphicsObject->sceneTransform();

    QPointF p = t.inverted().map(scenePoint);

    setDraggingPosition(p);

    mNodeGraphicsObject->update();

    setReaction(SNodeState::REACTING, reactingPortType, reactingDataType);
}

void ANode::setReaction(SNodeState::EReactToConnectionState reaction, EPortType reactingPortType, SNodeDataType reactingDataType)
{
    mNodeState.Reaction = reaction;
    mNodeState.ReactingPortType = reactingPortType;
    mNodeState.ReactingDataType = std::move(reactingDataType);
}

QPointF ANode::getWidgetPosition()
{
    if (auto w = mNodeDataModel->getEmbeddedWidget())
    {
        if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag)
        {
            // If the widget wants to use as much vertical space as possible, place it immediately after the caption.
            return QPointF(mSpacing + getPortWidth(EPortType::Input), getCaptionHeight());
        }
        else
        {
            if (mNodeDataModel->validationState() != ENodeValidationState::Valid)
            {
                return QPointF(mSpacing + getPortWidth(EPortType::Input), (getCaptionHeight() + mHeight - getValidationHeight() - mSpacing - w->height()) / 2.0);
            }

            return QPointF(mSpacing + getPortWidth(EPortType::Input), (getCaptionHeight() + mHeight - w->height()) / 2.0);
        }
    }

    return QPointF();
}

void ANode::resetReactionToConnection()
{
    setReaction(SNodeState::NOT_REACTING);
    mNodeGraphicsObject->update();
}

void ANode::propagateData(INodeData *nodeData, PortIndex inPortIndex) const
{
    mNodeDataModel->setInputData(nodeData, inPortIndex);

    //Recalculate the nodes visuals. A data change can result in the node taking more space than before, so this forces a recalculate+repaint on the affected node
    mNodeGraphicsObject->setGeometryChanged();
    resize();
    mNodeGraphicsObject->update();
    mNodeGraphicsObject->moveConnections();
}

void ANode::onDataUpdated(PortIndex index)
{
    auto nodeData = mNodeDataModel->getOutputData(index);

    auto connections = getConnections(EPortType::Output, index);

    for (auto const &c : connections)
    {
        c.second->propagateData(nodeData);
    }
}

void ANode::onNodeSizeUpdated()
{
    if (mNodeDataModel->getEmbeddedWidget())
    {
        mNodeDataModel->getEmbeddedWidget()->adjustSize();
    }

    resize();

    for (EPortType type : {EPortType::Input, EPortType::Output})
    {
        for (auto &conn_set : getEntries(type))
        {
            for (auto &pair : conn_set)
            {
                AConnection *conn = pair.second;
                conn->getConnectionGraphicsObject()->move();
            }
        }
    }
}

APROCH_NAMESPACE_END
