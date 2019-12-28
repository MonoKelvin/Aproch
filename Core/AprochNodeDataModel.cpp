#include "AprochNodeDataModel.h"

APROCH_NAMESPACE_BEGIN

bool INodeData::isSameType(const INodeData& nodeData) const
{
    return (this->type().id == nodeData.type().id);
}

INodeDataModel::INodeDataModel()
    :mNodeStyle(AprochStyle::GetNodeStyle())
{

}

QJsonObject INodeDataModel::save() const
{
    QJsonObject modelJson;

    modelJson["name"] = name();

    return modelJson;
}

APROCH_NAMESPACE_END
