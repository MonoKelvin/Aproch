#ifndef APROCHSTYLE_H
#define APROCHSTYLE_H

#include "IStyle.h"
#include <QColor>

APROCH_NAMESPACE_BEGIN

struct APROCH_EXPORT SFlowViewStyle : public IStyle
{
    QColor BackgroundColor;
    QColor FineGridColor;
    QColor CoarseGridColor;
    QColor CentralGridColor;

    void setStyle(const QJsonObject &obj);
};

struct APROCH_EXPORT SNodeStyle : public IStyle
{
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

    void setStyle(const QJsonObject &obj);
};

struct APROCH_EXPORT SConnectionStyle : public IStyle
{
    QColor ConstructionColor;
    QColor NormalColor;
    QColor SelectedColor;
    QColor SelectedHaloColor;
    QColor HoveredColor;

    float LineWidth;
    float ConstructionLineWidth;
    float PointDiameter;

    void setStyle(const QJsonObject &obj);
};

class APROCH_EXPORT AStyle
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
    AStyle();
    AStyle(AStyle const &) = delete;
    AStyle &operator=(AStyle const &) = delete;

    inline static AStyle &GetInstance(void) {
        static AStyle instance;
        return instance;
    }

private:
    SNodeStyle mNodeStyle;

    SConnectionStyle mConnectionStyle;

    SFlowViewStyle mFlowViewStyle;

};


APROCH_NAMESPACE_END

#endif // APROCHSTYLE_H
