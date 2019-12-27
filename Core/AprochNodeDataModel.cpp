#include "AprochNodeDataModel.h"

APROCH_NAMESPACE_BEGIN

bool INodeData::isSameType(const INodeData& nodeData) const
{
    return (this->type().id == nodeData.type().id);
}

APROCH_NAMESPACE_END
