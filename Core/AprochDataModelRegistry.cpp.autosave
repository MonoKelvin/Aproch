#include "AprochDataModelRegistry.h"

#include <QFile>
#include <QMessageBox>

APROCH_NAMESPACE_BEGIN

std::unique_ptr<INodeDataModel> AprochDataModelRegistry::create(const QString &modelName)
{
    auto it = mRegisteredItemCreators.find(modelName);

    if (it != mRegisteredItemCreators.end())
    {
        return it->second();
    }

    return nullptr;
}

TypeConverter AprochDataModelRegistry::getTypeConverter(const SNodeDataType &d1, const SNodeDataType &d2) const
{
    TypeConverterId converterId = std::make_pair(d1, d2);

    auto it = mRegisteredTypeConverters.find(converterId);

    if (it != mRegisteredTypeConverters.end())
    {
        return it->second;
    }

    return TypeConverter{};
}


APROCH_NAMESPACE_END
