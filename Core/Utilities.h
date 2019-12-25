#ifndef UTILITIES_H
#define UTILITIES_H

#include "Aproch.h"

#include <QSharedPointer>
#include <functional>

APROCH_NAMESPACE_BEGIN

class INodeData;
class SNodeDataType;

using SharedNodeData = QSharedPointer<INodeData>;

// a function taking in NodeData and returning NodeData
using TypeConverter = std::function<SharedNodeData(SharedNodeData)>;

// data-type-in, data-type-out
using TypeConverterId = QPair<SNodeDataType, SNodeDataType>;

APROCH_NAMESPACE_END

#endif // UTILITIES_H
