#include "AStyle.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

APROCH_NAMESPACE_BEGIN

void SFlowViewStyle::setStyle(const QJsonObject &obj)
{
    READ_COLOR_FROM_JSON(obj, BackgroundColor);
    READ_COLOR_FROM_JSON(obj, FineGridColor);
    READ_COLOR_FROM_JSON(obj, CoarseGridColor);
    READ_COLOR_FROM_JSON(obj, CentralGridColor);
}

void SNodeStyle::setStyle(const QJsonObject &obj)
{
    READ_COLOR_FROM_JSON(obj, BackgroundColor);
    READ_COLOR_FROM_JSON(obj, NormalBoundaryColor);
    READ_COLOR_FROM_JSON(obj, SelectedBoundaryColor);
    READ_COLOR_FROM_JSON(obj, ShadowColor);
    READ_COLOR_FROM_JSON(obj, FontColor);
    READ_COLOR_FROM_JSON(obj, FontColorFaded);
    READ_COLOR_FROM_JSON(obj, ConnectionPointColor);
    READ_COLOR_FROM_JSON(obj, FilledConnectionPointColor);
    READ_COLOR_FROM_JSON(obj, WarningColor);
    READ_COLOR_FROM_JSON(obj, ErrorColor);
    READ_FLOAT_FROM_JSON(obj, PenWidth);
    READ_FLOAT_FROM_JSON(obj, HoveredPenWidth);
    READ_FLOAT_FROM_JSON(obj, ConnectionPointDiameter);
    READ_FLOAT_FROM_JSON(obj, Opacity);
    READ_VALUE_FROM_JSON(obj, UseCustomColors, Bool);
}

void SConnectionStyle::setStyle(const QJsonObject &obj)
{
    READ_COLOR_FROM_JSON(obj, ConstructionColor);
    READ_COLOR_FROM_JSON(obj, NormalColor);
    READ_COLOR_FROM_JSON(obj, SelectedColor);
    READ_COLOR_FROM_JSON(obj, SelectedHaloColor);
    READ_COLOR_FROM_JSON(obj, HoveredColor);
    READ_FLOAT_FROM_JSON(obj, LineWidth);
    READ_FLOAT_FROM_JSON(obj, ConstructionLineWidth);
    READ_FLOAT_FROM_JSON(obj, PointDiameter);
}

AStyle::AStyle()
{
    loadJsonFile(":/style/DefaultStyle.json");
}

void AStyle::loadJsonFile(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open file " << fileName;
        return;
    }

    loadJsonFromByteArray(file.readAll());
}

void AStyle::loadJsonFromByteArray(const QByteArray &byteArray)
{
    QJsonObject rootObj = QJsonDocument::fromJson(byteArray).object();
    mNodeStyle.setStyle(rootObj["NodeStyle"].toObject());
    mConnectionStyle.setStyle(rootObj["ConnectionStyle"].toObject());
    mFlowViewStyle.setStyle(rootObj["FlowViewStyle"].toObject());
}

APROCH_NAMESPACE_END
