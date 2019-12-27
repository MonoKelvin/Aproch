#ifndef APROCHDATAMODELREGISTRY_H
#define APROCHDATAMODELREGISTRY_H

#include "AprochNodeDataModel.h"
#include "Utilities.h"

#include <QSet>
#include <QString>
#include <unordered_map>

APROCH_NAMESPACE_BEGIN

// Class uses map for storing models (name, model)
class APROCH_EXPORT AprochDataModelRegistry
{
public:
    using RegistryItemPtr = std::unique_ptr<INodeDataModel>;
    using RegistryItemCreator = std::function<RegistryItemPtr()>;
    using RegisteredModelCreatorsMap = std::unordered_map<QString, RegistryItemCreator>;
    using RegisteredModelsCategoryMap = std::unordered_map<QString, QString>;
    using CategoriesSet = QSet<QString>;

    using RegisteredTypeConvertersMap = std::map<TypeConverterId, TypeConverter>;

    AprochDataModelRegistry()  = default;
    ~AprochDataModelRegistry() = default;

    AprochDataModelRegistry(const AprochDataModelRegistry &) = delete;
    AprochDataModelRegistry(AprochDataModelRegistry &&)      = default;

    AprochDataModelRegistry &operator=(const AprochDataModelRegistry &) = delete;
    AprochDataModelRegistry &operator=(AprochDataModelRegistry &&)      = default;

public:

    template<typename ModelType>
    void registerModel(RegistryItemCreator creator, const QString &category = QObject::tr("Others"))
    {
        registerModelImpl<ModelType>(std::move(creator), category);
    }

    template<typename ModelType>
    void registerModel(const QString &category = QObject::tr("Others"))
    {
        RegistryItemCreator creator = []() { return std::make_unique<ModelType>(); };
        registerModelImpl<ModelType>(std::move(creator), category);
    }

    template<typename ModelType>
    void registerModel(const QString &category, RegistryItemCreator creator)
    {
        registerModelImpl<ModelType>(std::move(creator), category);
    }

    void registerTypeConverter(const TypeConverterId &id, TypeConverter typeConverter)
    {
        mRegisteredTypeConverters[id] = std::move(typeConverter);
    }

    std::unique_ptr<INodeDataModel> create(const QString  &modelName);

    inline RegisteredModelCreatorsMap const &getRegisteredModelCreators() const
    {
        return mRegisteredItemCreators;
    }

    inline RegisteredModelsCategoryMap const &getRegisteredModelsCategoryAssociation() const
    {
        return mRegisteredModelsCategory;
    }

    inline CategoriesSet const &getCategories() const
    {
        return mCategories;
    }

    TypeConverter getTypeConverter(const SNodeDataType &d1, const SNodeDataType &d2) const;

private:

    RegisteredModelsCategoryMap mRegisteredModelsCategory;

    CategoriesSet mCategories;

    RegisteredModelCreatorsMap mRegisteredItemCreators;

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
    registerModelImpl(RegistryItemCreator creator, QString const &category)
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
    registerModelImpl(RegistryItemCreator creator, QString const &category)
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
