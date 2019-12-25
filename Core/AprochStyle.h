#ifndef APROCHSTYLE_H
#define APROCHSTYLE_H

#include "IStyle.h"
#include <QColor>

APROCH_NAMESPACE_BEGIN

class APROCH_EXPORT SFlowViewStyle : public IStyle
{
public:
    void setStyle(const QJsonObject &obj) override;

public:
    QColor BackgroundColor;
    QColor FineGridColor;
    QColor CoarseGridColor;
    QColor CentralGridColor;
};

class APROCH_EXPORT SNodeStyle : public IStyle
{
public:
    void setStyle(const QJsonObject &obj) override;

public:
    QColor BackgroundColor;
    QColor NormalBoundaryColor;
    QColor SelectedBoundaryColor;
    QColor ShadowColor;
    QColor FontColor;
    QColor FontColorFaded;

    QColor ConnectionPointColor;
    QColor FilledConnectionPointColor;

    QColor WarningColor;
    QColor ErrorColor;

    float PenWidth;
    float HoveredPenWidth;
    float ConnectionPointDiameter;
    float Opacity;

    bool UseCustomColors;
};

class APROCH_EXPORT SConnectionStyle : public IStyle
{
public:
    void setStyle(const QJsonObject &obj) override;

public:
    QColor ConstructionColor;
    QColor NormalColor;
    QColor SelectedColor;
    QColor SelectedHaloColor;
    QColor HoveredColor;

    float LineWidth;
    float ConstructionLineWidth;
    float PointDiameter;
};

class APROCH_EXPORT AprochStyle
{
public:
    inline static SNodeStyle const &GetNodeStyle(void) {
        return GetInstance().mNodeStyle;
    }
    inline static SConnectionStyle const &GetConnectionStyle(void){
        return GetInstance().mConnectionStyle;
    }
    inline static SFlowViewStyle const &GetFlowViewStyle(void){
        return GetInstance().mFlowViewStyle;
    }

    static void SetNodeStyle(const SNodeStyle& nodeStyle){
        GetInstance().mNodeStyle = nodeStyle;
    }
    static void SetConnectionStyle(const SConnectionStyle& connectionStyle){
        GetInstance().mConnectionStyle = connectionStyle;
    }
    static void SetFlowViewStyle(const SFlowViewStyle& flowViewStyle){
        GetInstance().mFlowViewStyle = flowViewStyle;
    }

    void loadJsonFile(QString fileName);
    void loadJsonFromByteArray(const QByteArray &byteArray);

private:
    AprochStyle();
    AprochStyle(AprochStyle const &) = delete;
    AprochStyle &operator=(AprochStyle const &) = delete;

    inline static AprochStyle &GetInstance(void) {
        static AprochStyle instance;
        return instance;
    }

private:
    SNodeStyle mNodeStyle;

    SConnectionStyle mConnectionStyle;

    SFlowViewStyle mFlowViewStyle;

};


APROCH_NAMESPACE_END

#endif // APROCHSTYLE_H
