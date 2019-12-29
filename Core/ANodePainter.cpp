#include "ANodePainter.h"

#include "AFlowScene.h"
#include "AStyle.h"
#include "ANode.h"
#include "ADataModelRegistry.h"

APROCH_NAMESPACE_BEGIN

void ANodePainter::Paint(QPainter *painter, ANode &node, AFlowScene &scene)
{
    node.recalculateSize(painter->font());

    DrawNodeRect(painter, node);
    DrawModelName(painter, node);
    DrawPortLabel(painter, node);
    DrawPort(painter, node, scene);
    DrawConnectedPort(painter, node);
}

void ANodePainter::DrawNodeRect(QPainter *painter, ANode &node)
{
    SNodeStyle const &nodeStyle = AStyle::GetNodeStyle();

    auto color = node.getNodeGraphicsObject()->isSelected() ? nodeStyle.SelectedBoundaryColor : nodeStyle.NormalBoundaryColor;

    if (node.isHovered())
    {
        QPen p(color, qreal(nodeStyle.HoveredPenWidth));
        painter->setPen(p);
    }
    else
    {
        QPen p(color, qreal(nodeStyle.PenWidth));
        painter->setPen(p);
    }

    painter->setBrush(nodeStyle.BackgroundColor);

    double diam = double(nodeStyle.ConnectionPointDiameter);

    QRectF boundary(-diam, -diam, 2.0 * diam + node.getWidth(), 2.0 * diam + node.getHeight());

    painter->drawRoundedRect(boundary, NODE_BORDER_RADIUS, NODE_BORDER_RADIUS);
}

void ANodePainter::DrawModelName(QPainter *painter, ANode &node)
{
    auto const *model = node.getNodeDataModel();
    auto const &nodeStyle = model->getNodeStyle();

    if (!model->isCaptionVisible())
    {
        return;
    }

    QString const &name = model->caption();

    QFont f = painter->font();

    f.setBold(true);

    QFontMetrics metrics(f);

    auto rect = metrics.boundingRect(name);

    QPointF position((node.getWidth() - rect.width()) / 2.0, node.getSpacing() / 3.0);

    painter->setFont(f);
    painter->setPen(nodeStyle.FontColor);
    painter->drawText(position, name);

    f.setBold(false);
    painter->setFont(f);
}

void ANodePainter::DrawPortLabel(QPainter *painter, ANode &node)
{
    QFontMetrics const &metrics = painter->fontMetrics();

    for (EPortType portType : {EPortType::Output, EPortType::Input})
    {
        auto const *model = node.getNodeDataModel();
        auto const &nodeStyle = model->getNodeStyle();
        auto &entries = node.getEntries(portType);

        unsigned int n = unsigned(entries.size());
        for (unsigned int i = 0; i < n; ++i)
        {
            QPointF p = node.getPortScenePosition(i, portType);

            if (entries[int(i)].empty())
            {
                painter->setPen(nodeStyle.FontColorFaded);
            }
            else
            {
                painter->setPen(nodeStyle.FontColor);
            }

            QString s;

            if (model->isPortCaptionVisible(portType, i))
            {
                s = model->portCaption(portType, i);
            }
            else
            {
                s = model->dataType(portType, i).Name;
            }

            auto rect = metrics.boundingRect(s);

            p.setY(p.y() + rect.height() / 4.0);

            switch (portType)
            {
            case EPortType::Input:
                p.setX(5.0);
                break;

            case EPortType::Output:
                p.setX(node.getWidth() - 5.0 - rect.width());
                break;

            default:
                break;
            }

            painter->drawText(p, s);
        }
    }
}

void ANodePainter::DrawPort(QPainter *painter, ANode &node, AFlowScene &scene)
{
    auto const *model = node.getNodeDataModel();
    auto const &nodeStyle = model->getNodeStyle();

    float diameter = nodeStyle.ConnectionPointDiameter;
    double reducedDiameter = double(diameter) * 0.6;

    for (EPortType portType : {EPortType::Output, EPortType::Input})
    {
        unsigned int n = unsigned(node.getEntries(portType).size());

        for (unsigned int i = 0; i < n; ++i)
        {
            QPointF p = node.getPortScenePosition(i, portType);

            auto const &dataType = model->dataType(portType, i);

            bool canConnect = (node.getEntries(portType)[int(i)].empty() ||
                               (portType == EPortType::Output &&
                                model->portOutConnectionPolicy(i) == INodeDataModel::EConnectionPolicy::Many));

            double r = 1.0;
            if (node.isReacting() && canConnect && portType == node.getReactingPortType())
            {
                auto diff = node.getDraggingPos() - p;
                double distSqrt = QPointF::dotProduct(diff, diff);
                bool typeConvertable = false;

                {
                    if (portType == EPortType::Input)
                    {
                        typeConvertable = scene.getRegistry()->getTypeConverter(node.getReactingDataType(), dataType) != nullptr;
                    }
                    else
                    {
                        typeConvertable = scene.getRegistry()->getTypeConverter(dataType, node.getReactingDataType()) != nullptr;
                    }
                }

                if (node.getReactingDataType().ID == dataType.ID || typeConvertable)
                {
                    double const thres = 1600.0;
                    r = (distSqrt < thres) ? (2.0 - distSqrt / thres) : 1.0;
                }
                else
                {
                    double const thres = 80.0;
                    r = (distSqrt < thres) ? (distSqrt / thres) : 1.0;
                }
            }

            painter->setBrush(nodeStyle.ConnectionPointColor);
            painter->drawEllipse(p, reducedDiameter * r, reducedDiameter * r);
        }
    }
}

void ANodePainter::DrawConnectedPort(QPainter *painter, ANode &node)
{
    auto const *model = node.getNodeDataModel();
    auto const &nodeStyle = model->getNodeStyle();
    double diameter = double(nodeStyle.ConnectionPointDiameter);

    for (EPortType portType : {EPortType::Output, EPortType::Input})
    {
        int n = node.getEntries(portType).size();

        for (int i = 0; i < n; ++i)
        {
            QPointF p = node.getPortScenePosition(unsigned(i), portType);

            if (!node.getEntries(portType)[i].empty())
            {
                painter->setPen(nodeStyle.FilledConnectionPointColor);
                painter->setBrush(nodeStyle.FilledConnectionPointColor);
                painter->drawEllipse(p, diameter * 0.4, diameter * 0.4);
            }
        }
    }
}

APROCH_NAMESPACE_END
