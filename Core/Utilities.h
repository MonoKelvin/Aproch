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

#if (!defined(_MSC_VER) && (__cplusplus < 201300)) || ( defined(_MSC_VER) && (_MSC_VER < 1800))
//_MSC_VER == 1800 is Visual Studio 2013, which is already somewhat C++14 compilant,
// and it has make_unique in it's standard library implementation
template<typename T, typename... Args> std::unique_ptr<T> make_unique(Args &&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#else
template<typename T, typename... Args> std::unique_ptr<T> make_unique(Args &&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}
#endif

APROCH_NAMESPACE_END

#endif // UTILITIES_H
