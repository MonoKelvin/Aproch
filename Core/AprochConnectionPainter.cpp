#include "AprochConnectionPainter.h"

#include "AprochConnection.h"
#include "AprochConnectionGraphicsObject.h"
#include "AprochStyle.h"

#include <QPainter>
#include <QIcon>

APROCH_NAMESPACE_BEGIN

static QPainterPath ShapePath(const AprochConnection &connection)
{
    QPointF const &source = connection.source();
    QPointF const &sink = connection.sink();

    auto c1c2 = connection.pointsC1C2();

    // cubic spline
    QPainterPath cubic(source);

    cubic.cubicTo(c1c2.first, c1c2.second, sink);

    return cubic;
}


#ifdef APROCH_DEBUG_DRAWING
void AprochConnectionPainter::DebugDrawing(QPainter *painter, AprochConnection const &connection)
{
    Q_UNUSED(painter);
    {
        QPointF const &source = connection.source();
        QPointF const &sink = connection.sink();

        auto points = connection.pointsC1C2();

        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);

        painter->drawLine(QLineF(source, points.first));
        painter->drawLine(QLineF(points.first, points.second));
        painter->drawLine(QLineF(points.second, sink));
        painter->drawEllipse(points.first, 3, 3);
        painter->drawEllipse(points.second, 3, 3);

        painter->setBrush(Qt::NoBrush);
        painter->drawPath(cubicPath(connection));
    }

    {
        painter->setPen(Qt::yellow);
        painter->drawRect(connection.boundingRect());
    }
}
#endif

void AprochConnectionPainter::Paint(QPainter *painter, const AprochConnection &connection, EConnectionShape shape)
{
    DrawHoveredOrSelected(painter, connection);
    DrawSketchLine(painter, connection);
    DrawNormalLine(painter, connection);

#ifdef APROCH_DEBUG_DRAWING
    DebugDrawing(painter, connection);
#endif

    QPointF const &source = connection.source();
    QPointF const &sink = connection.sink();

    auto const &connectionStyle = AprochStyle::GetConnectionStyle();

    float const pointDiameter = connectionStyle.PointDiameter;

    painter->setPen(connectionStyle.ConstructionColor);
    painter->setBrush(connectionStyle.ConstructionColor);
    double const pointRadius = double(pointDiameter) / 2.0;
    painter->drawEllipse(source, pointRadius, pointRadius);
    painter->drawEllipse(sink, pointRadius, pointRadius);
}

QPainterPath AprochConnectionPainter::GetPainterStroke(const AprochConnection &connection)
{
    auto cubic = ShapePath(connection);

    QPointF const &source = connection.source();
    QPainterPath result(source);

    unsigned segments = 20;

    for (auto i = 0ul; i < segments; ++i)
    {
        double ratio = double(i + 1) / segments;
        result.lineTo(cubic.pointAtPercent(ratio));
    }

    QPainterPathStroker stroker; stroker.setWidth(10.0);

    return stroker.createStroke(result);
}


void AprochConnectionPainter::DrawSketchLine(QPainter *painter, const AprochConnection &connection)
{
    if (connection.isRequirePort())
    {
        auto const &connectionStyle = AprochStyle::GetConnectionStyle();

        QPen p;
        p.setWidthF(double(connectionStyle.ConstructionLineWidth));
        p.setColor(connectionStyle.ConstructionColor);
        p.setStyle(Qt::DashLine);

        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);

        auto cubic = ShapePath(connection);
        painter->drawPath(cubic);
    }
}

void AprochConnectionPainter::DrawHoveredOrSelected(QPainter *painter, const AprochConnection &connection)
{
    bool const hovered = connection.hovered();
    auto const &graphicsObject = connection.getConnectionGraphicsObject();
    bool const selected = graphicsObject.isSelected();

    // drawn as a fat background
    if (hovered || selected)
    {
        QPen p;

        auto const &connectionStyle = AprochStyle::GetConnectionStyle();
        float const lineWidth = connectionStyle.LineWidth;

        p.setWidthF(2.0 * double(lineWidth));
        p.setColor(selected ? connectionStyle.SelectedHaloColor : connectionStyle.HoveredColor);

        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);

        // cubic spline
        auto cubic = ShapePath(connection);
        painter->drawPath(cubic);
    }
}

void AprochConnectionPainter::DrawNormalLine(QPainter *painter, const AprochConnection &connection)
{
    if (connection.isRequirePort())
    {
        return;
    }

    auto const &connectionStyle = AprochStyle::GetConnectionStyle();

    QColor selectedColor = connectionStyle.SelectedColor;

    bool gradientColor = false;

    //    if (connectionStyle.useDataDefinedColors)
    //    {
    //        auto dataTypeOut = connection.setDataType(EPortType::Output);
    //        auto dataTypeIn = connection.setDataType(EPortType::Input);

    //        gradientColor = (dataTypeOut.id != dataTypeIn.id);

    //        normalColorOut = connectionStyle.normalColor(dataTypeOut.id);
    //        normalColorIn = connectionStyle.normalColor(dataTypeIn.id);
    //        selectedColor = connectionStyle.NormalColor.darker(200);
    //    }

    // draw normal line
    QPen p;

    p.setWidthF(double(connectionStyle.LineWidth));

    auto const &graphicsObject = connection.getConnectionGraphicsObject();
    bool const selected = graphicsObject.isSelected();

    auto cubic = ShapePath(connection);
    if (gradientColor)
    {
        painter->setBrush(Qt::NoBrush);

        QColor c = connectionStyle.NormalColor;
        if (selected)
        {
            c = c.darker(200);
        }
        p.setColor(c);
        painter->setPen(p);

        unsigned int const segments = 60;

        for (unsigned int i = 0u; i < segments; ++i)
        {
            double ratioPrev = double(i) / segments;
            double ratio = double(i + 1) / segments;

            if (i == segments / 2)
            {
                QColor c = connectionStyle.NormalColor;
                if (selected)
                    c = c.darker(200);

                p.setColor(c);
                painter->setPen(p);
            }
            painter->drawLine(cubic.pointAtPercent(ratioPrev), cubic.pointAtPercent(ratio));
        }

        {
            QPixmap pixmap = QIcon(":convert.png").pixmap(QSize(22, 22));
            painter->drawPixmap(cubic.pointAtPercent(0.50) - QPoint(pixmap.width() / 2, pixmap.height() / 2), pixmap);
        }
    }
    else
    {
        p.setColor(connectionStyle.NormalColor);

        if (selected)
        {
            p.setColor(selectedColor);
        }

        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);

        painter->drawPath(cubic);
    }
}


APROCH_NAMESPACE_END
