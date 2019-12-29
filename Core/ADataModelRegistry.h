#ifndef APROCHDATAMODELREGISTRY_H
#define APROCHDATAMODELREGISTRY_H

#include "ANodeDataModel.h"
#include "Utilities.h"

#include <QString>
#include <unordered_map>
#include <set>

APROCH_NAMESPACE_BEGIN

using RegistryModelCreator = std::function<INodeDataModel*(void)>;
using RegisteredModelCreatorsMap = std::unordered_map<QString, RegistryModelCreator>;
using RegisteredModelsCategoryMap = std::unordered_map<QString, QString>;
using RegisteredTypeConvertersMap = std::map<TypeConverterId, TypeConverter>;
using CategoriesSet = std::set<QString>;

class APROCH_EXPORT ADataModelRegistry
{
public:
    ADataModelRegistry()  = default;
    ~ADataModelRegistry() = default;

    ADataModelRegistry(const ADataModelRegistry &) = delete;
    ADataModelRegistry(ADataModelRegistry &&)      = default;

    ADataModelRegistry &operator=(const ADataModelRegistry &) = delete;
    ADataModelRegistry &operator=(ADataModelRegistry &&)      = default;

public:
    template<typename ModelType>
    void registerModel(const QString &category = QObject::tr("Others"))
    {
        RegistryModelCreator creator = []() { return new ModelType(); };
        registerModelImpl<ModelType>(std::move(creator), category);
    }

    template<typename ModelType>
    void registerModel(const QString &category, RegistryModelCreator creator)
    {
        registerModelImpl<ModelType>(std::move(creator), category);
    }

    void registerTypeConverter(const TypeConverterId &id, TypeConverter typeConverter)
    {
        mRegisteredTypeConverters[id] = std::move(typeConverter);
    }

    INodeDataModel *create(const QString  &modelName);

public:
    inline RegisteredModelCreatorsMap const &getRegisteredModelCreators() const
    {
        return mRegisteredItemCreators;
    }

    inline RegisteredModelsCategoryMap const &getRegisteredModelsCategoryMap() const
    {
        return mRegisteredModelsCategory;
    }

    inline CategoriesSet const &getCategories() const
    {
        return mCategories;
    }

    TypeConverter getTypeConverter(const SNodeDataType &d1, const SNodeDataType &d2) const;

private:
    /// 数据模型类别映射
    RegisteredModelsCategoryMap mRegisteredModelsCategory;

    /// 所有的类别集合
    CategoriesSet mCategories;

    /// 数据模型的创建映射
    RegisteredModelCreatorsMap mRegisteredItemCreators;

    /// 数据模型类型转换映射
    RegisteredTypeConvertersMap mRegisteredTypeConverters;

private:

    // If the registered ModelType class has the static member method
    //
    //      static Qstring Name();
    //
    // use it. Otherwise use the non-static method:
    //
    //       virtual QString name() const;

    template <typename T, typename = void>
    struct HasStaticMethodName
        : std::false_type
    {};

    template <typename T> struct
    HasStaticMethodName<T, typename std::enable_if<std::is_same<decltype(T::Name()), QString>::value>::type> : std::true_type
    {};

    template<typename ModelType>
    typename std::enable_if< HasStaticMethodName<ModelType>::value>::type
    registerModelImpl(RegistryModelCreator creator, QString const &category)
    {
        const QString name = ModelType::Name();
        if (mRegisteredItemCreators.count(name) == 0)
        {
            mRegisteredItemCreators[name] = std::move(creator);
            mCategories.insert(category);
            mRegisteredModelsCategory[name] = category;
        }
    }

    template<typename ModelType>
    typename std::enable_if < !HasStaticMethodName<ModelType>::value >::type
    registerModelImpl(RegistryModelCreator creator, QString const &category)
    {
        const QString name = creator()->name();
        if (mRegisteredItemCreators.count(name) == 0)
        {
            mRegisteredItemCreators[name] = std::move(creator);
            mCategories.insert(category);
            mRegisteredModelsCategory[name] = category;
        }
    }

};

APROCH_NAMESPACE_END

#endif // APROCHDATAMODELREGISTRY_H
