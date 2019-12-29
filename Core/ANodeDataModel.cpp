#include "ANodeDataModel.h"

#include "ANode.h"

APROCH_NAMESPACE_BEGIN

bool INodeData::isSameType(const INodeData &nodeData) const
{
    return (this->type().ID == nodeData.type().ID);
}

INodeDataModel::INodeDataModel()
    : mNodeStyle(AStyle::GetNodeStyle())
{

}

QJsonObject INodeDataModel::save() const
{
    QJsonObject modelJson;

    modelJson["name"] = name();

    return modelJson;
}

APROCH_NAMESPACE_END
