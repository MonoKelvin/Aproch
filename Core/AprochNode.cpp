#include "AprochNode.h"

#include "AprochConnection.h"
#include "AprochNodeGraphicsObject.h"

#include <QWidget>

APROCH_NAMESPACE_BEGIN

AprochNode::AprochNode(std::unique_ptr<INodeDataModel> &&dataModel)
    : mWidth(100)
    , mHeight(150)
    , mPortWidth(30)
    , mSpacing(20)
    , mFontMetrics(QFont())
    , mBoldFontMetrics(QFont())
    , mHovered(false)
    , mDraggingPos(-1000, -1000)
    , mNodeDataModel(dataModel) {
    mNodeState.InConnections.resize(int(mNodeDataModel->nPorts(EPortType::Input)));
    mNodeState.OutConnections.resize(int(mNodeDataModel->nPorts(EPortType::Output)));
    mNodeState.Reaction = SNodeState::EReactToConnectionState::NOT_REACTING;
    mNodeState.ReactingPortType = EPortType::None;
    mNodeState.IsResizing = false;

    QFont f;
    f.setBold(true);

    mBoldFontMetrics = QFontMetrics(f);
}

QRectF AprochNode::boundingRect(void) const {
    double addon = double(AprochStyle::GetNodeStyle().ConnectionPointDiameter);

    return QRectF(-addon, -addon, mWidth + 2 * addon, mHeight + 2 * addon);
}

void AprochNode::resize(void) const {
    if (auto w = mNodeDataModel->embeddedWidget()) {
        mHeight = AMax(mHeight, w->height());
    }

    mHeight += captionHeight();

    mWidth = (mPortWidth +  mSpacing) * 2;

    if (auto w = mNodeDataModel->embeddedWidget()) {
        mWidth += w->width();
    }

    mWidth = AMax(mWidth, captionWidth());

    if (mNodeDataModel->validationState() != ENodeValidationState::Valid) {
//        mWidth = AMax(mWidth, validationWidth());
//        mHeight += validationHeight() + mSpacing;
    }
}

QPointF AprochNode::getPortScenePosition(PortIndex index, EPortType portType, const QTransform &transform) const {
    auto const &nodeStyle = AprochStyle::GetNodeStyle();

    QPointF result;

    double totalHeight = captionHeight();
    totalHeight += double(mSpacing) * index + mSpacing / 2.0;

    switch (portType) {
    case EPortType::Output: {
        float x = mWidth + nodeStyle.ConnectionPointDiameter;
        result = QPointF(double(x), totalHeight);
        break;
    }
    case EPortType::Input: {
        double x = double(-nodeStyle.ConnectionPointDiameter);
        result = QPointF(x, totalHeight);
        break;
    }
    default:
        break;
    }

    return transform.map(result);
}

int AprochNode::captionHeight(void) const {
    if (!mNodeDataModel->isCaptionVisible()) {
        return 0;
    }

    QString name = mNodeDataModel->caption();

    return mBoldFontMetrics.boundingRect(name).height();
}

int AprochNode::captionWidth(void) const {
    if (!mNodeDataModel->isCaptionVisible()) {
        return 0;
    }

    QString name = mNodeDataModel->caption();

    return mBoldFontMetrics.boundingRect(name).width();
}

QJsonObject AprochNode::save(void) const {
    QJsonObject nodeJson;
    nodeJson["id"] = mUuid.toString();
    nodeJson["model"] = mNodeDataModel->save();

    QJsonObject obj;
    obj["x"] = mNodeGraphicsObject->pos().x();
    obj["y"] = mNodeGraphicsObject->pos().y();
    nodeJson["position"] = obj;

    return nodeJson;
}

void AprochNode::restore(const QJsonObject &json) {
    mUuid = QUuid(json["id"].toString());

    QJsonObject positionJson = json["position"].toObject();
    QPointF point(positionJson["x"].toDouble(), positionJson["y"].toDouble());

    mNodeGraphicsObject->setPos(point);
    mNodeDataModel->restore(json["model"].toObject());
}

QVector<ConnectionPtrSet> const &AprochNode::getEntries(EPortType portType) const {
    if (portType == EPortType::Input) {
        return mNodeState.InConnections;
    } else {
        return mNodeState.OutConnections;
    }
}

QVector<ConnectionPtrSet> &AprochNode::getEntries(EPortType portType) {
    if (portType == EPortType::Input) {
        return mNodeState.InConnections;
    } else {
        return mNodeState.OutConnections;
    }
}

ConnectionPtrSet AprochNode::connections(EPortType portType, PortIndex portIndex) const {
    return getEntries(portType)[int(portIndex)];
}

void AprochNode::reactToPossibleConnection(EPortType reactingPortType, const SNodeDataType &reactingDataType, const QPointF &scenePoint) {
    QTransform const t = mNodeGraphicsObject->sceneTransform();

    QPointF p = t.inverted().map(scenePoint);

    setDraggingPosition(p);

    mNodeGraphicsObject->update();

    setReaction(SNodeState::REACTING, reactingPortType, reactingDataType);
}

void AprochNode::setReaction(SNodeState::EReactToConnectionState reaction,
                             EPortType reactingPortType,
                             SNodeDataType reactingDataType) {
    mNodeState.Reaction = reaction;
    mNodeState.ReactingPortType = reactingPortType;
    mNodeState.ReactingDataType = std::move(reactingDataType);
}



void AprochNode::resetReactionToConnection() {
    setReaction(SNodeState::NOT_REACTING);
    mNodeGraphicsObject->update();
}

void AprochNode::propagateData(std::shared_ptr<INodeData> nodeData, PortIndex inPortIndex) const {
    mNodeDataModel->setInputData(nodeData, inPortIndex);

    //Recalculate the nodes visuals. A data change can result in the node taking more space than before, so this forces a recalculate+repaint on the affected node
    mNodeGraphicsObject->setGeometryChanged();
    resize();
    mNodeGraphicsObject->update();
    mNodeGraphicsObject->moveConnections();
}


void AprochNode::onDataUpdated(PortIndex index) {
    auto nodeData = mNodeDataModel->getOutputData(index);

    auto connections = connections(EPortType::Output, index);

    for (auto const &c : connections) {
        c.second->propagateData(nodeData);
    }
}

void AprochNode::onNodeSizeUpdated() {
    if (mNodeDataModel->embeddedWidget()) {
        mNodeDataModel->embeddedWidget()->adjustSize();
    }

    resize();

    for (EPortType type : {EPortType::Input, EPortType::Output}) {
        for (auto &conn_set : getEntries(type)) {
            for (auto &pair : conn_set) {
                AprochConnection *conn = pair.second;
                conn->getConnectionGraphicsObject().move();
            }
        }
    }
}

APROCH_NAMESPACE_END
