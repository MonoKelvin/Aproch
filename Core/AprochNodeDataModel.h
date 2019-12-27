#ifndef APROCHNODEDATAMODEL_H
#define APROCHNODEDATAMODEL_H

#include "AprochPort.h"
#include "AprochStyle.h"
#include "ISerializable.h"

#include <QObject>
#include <memory>

APROCH_NAMESPACE_BEGIN

enum class ENodeValidationState
{
    Valid,
    Warning,
    Error
};

struct SNodeDataType
{
    QString id;
    QString name;

    friend inline bool operator<(const SNodeDataType &d1, const SNodeDataType &d2)
    {
        return d1.id < d2.id;
    }
};

class AprochConnection;

// Class represents data transferred between nodes.
// @param type is used for comparing the types
// The actual data is stored in subtypes
class APROCH_EXPORT INodeData
{
public:

    virtual ~INodeData() = default;

    virtual bool isSameType(const INodeData &nodeData) const;

    // Type for inner use
    virtual SNodeDataType type() const = 0;
};

class APROCH_EXPORT INodeDataModel : public QObject, public ISerializable
{
    Q_OBJECT
public:

    INodeDataModel();

    virtual ~INodeDataModel() override = default;

    // Caption is used in GUI
    virtual QString caption() const = 0;

    // It is possible to hide caption in GUI
    virtual bool isCaptionVisible() const { return true; }

    // Port caption is used in GUI to label individual ports
    virtual QString portCaption(EPortType, PortIndex) const { return QString(); }

    // It is possible to hide port caption in GUI
    virtual bool isPortCaptionVisible(EPortType, PortIndex) const { return false; }

    // Name makes this model unique
    virtual QString name() const = 0;

public:
    QJsonObject save() const override;

public:
    virtual PortIndex nPorts(EPortType portType) const = 0;

    virtual SNodeDataType dataType(EPortType portType, PortIndex portIndex) const = 0;

public:

    enum class EConnectionPolicy
    {
        One,
        Many,
    };

    virtual EConnectionPolicy portOutConnectionPolicy(PortIndex) const
    {
        return EConnectionPolicy::Many;
    }

    inline void setNodeStyle(const SNodeStyle &style) { mNodeStyle = style; }
    inline SNodeStyle const &getNodeStyle(void) const { return mNodeStyle; }

public:

    // Triggers the algorithm
    virtual void setInputData(std::shared_ptr<INodeData> nodeData, PortIndex port) = 0;

    virtual std::shared_ptr<INodeData> getOutputData(PortIndex port) = 0;

    virtual QWidget *embeddedWidget() = 0;

    virtual bool resizable() const { return false; }

    virtual ENodeValidationState validationState() const { return ENodeValidationState::Valid; }

    virtual QString validationMessage() const { return QString(""); }

//    virtual AprochNodePainterDelegate* painterDelegate() const { return nullptr; }

public Q_SLOTS:

    virtual void inputConnectionCreated(const AprochConnection &)
    {
    }

    virtual void inputConnectionDeleted(const AprochConnection &)
    {
    }

    virtual void outputConnectionCreated(const AprochConnection &)
    {
    }

    virtual void outputConnectionDeleted(const AprochConnection &)
    {
    }

Q_SIGNALS:
    void dataUpdated(PortIndex index);
    void dataInvalidated(PortIndex index);
    void computingStarted();
    void computingFinished();
    void embeddedWidgetSizeUpdated();

private:

    SNodeStyle mNodeStyle;
};

APROCH_NAMESPACE_END

#endif // APROCHNODEDATAMODEL_H
