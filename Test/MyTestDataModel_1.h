#ifndef MYTESTDATAMODEL_1_H
#define MYTESTDATAMODEL_1_H

#include "Core/ANodeDataModel.h"

//#include <QObject>
//#include <QLabel>
#include <memory>

using Aproch::INodeData;
using Aproch::SNodeDataType;
using Aproch::INodeDataModel;
using Aproch::EPortType;
using Aproch::PortIndex;

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph
class MyNodeData : public INodeData
{
public:
    SNodeDataType type() const override
    {
        return SNodeDataType{"MyNodeData", "My Node Data"};
    }
};

//------------------------------------------------------------------------------

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class MyDataModel : public INodeDataModel
{
    Q_OBJECT

public:
    virtual ~MyDataModel() {}

public:
    QString caption() const override
    {
        return QString("My Data Model");
    }

    QString name() const override
    {
        return QString("MyDataModel");
    }

public:
    QJsonObject save() const override
    {
        QJsonObject modelJson;

        modelJson["name"] = name();

        return modelJson;
    }

public:
    unsigned int nPorts(EPortType) const override
    {
        return 3;
    }

    SNodeDataType dataType(EPortType, PortIndex) const override
    {
        return MyNodeData().type();
    }

    std::shared_ptr<INodeData> getOutputData(PortIndex) override
    {
        return std::make_shared<MyNodeData>();
    }

    void setInputData(std::shared_ptr<INodeData>, PortIndex) override
    {
        //
    }

    QWidget * getEmbeddedWidget() override { return nullptr; }
};

#endif // MYTESTDATAMODEL_1_H
