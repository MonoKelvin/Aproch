#ifndef APROCHNODEDATAMODEL_H
#define APROCHNODEDATAMODEL_H

#include "APort.h"
#include "AStyle.h"
#include "ISerializable.h"

APROCH_NAMESPACE_BEGIN

class AConnection;
class INodeDataModel;
class ANode;

enum class ENodeValidationState
{
    Valid,
    Warning,
    Error
};

struct SNodeDataType
{
    QString ID;
    QString Name;

    friend inline bool operator<(const SNodeDataType &d1, const SNodeDataType &d2)
    {
        return d1.ID < d2.ID;
    }
};

/// Class represents data transferred between nodes.
/// @param type is used for comparing the types
/// The actual data is stored in subtypes
class APROCH_EXPORT INodeData : public QObject
{
public:
    INodeData(QObject *parent = nullptr)
        : QObject(parent)
    {}

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

    virtual EConnectionPolicy getPortOutConnectionPolicy(PortIndex) const
    {
        return EConnectionPolicy::Many;
    }

    inline void setNodeStyle(const SNodeStyle &style) { mNodeStyle = style; }
    inline SNodeStyle const &getNodeStyle(void) const { return mNodeStyle; }

public:
    virtual void setInputData(INodeData *nodeData, PortIndex port) = 0;

    virtual INodeData *getOutputData(PortIndex port) = 0;

    virtual QWidget *getEmbeddedWidget() = 0;

    virtual bool resizable() const { return false; }

    virtual ENodeValidationState validationState() const { return ENodeValidationState::Valid; }

    virtual QString validationMessage() const { return QString(""); }

    //    virtual ANodePainterDelegate* painterDelegate() const { return nullptr; }

public Q_SLOTS:

    virtual void inputConnectionCreated(const AConnection &)
    {
    }

    virtual void inputConnectionDeleted(const AConnection &)
    {
    }

    virtual void outputConnectionCreated(const AConnection &)
    {
    }

    virtual void outputConnectionDeleted(const AConnection &)
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
