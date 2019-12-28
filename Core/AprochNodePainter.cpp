#include "AprochNodePainter.h"

#include "AprochFlowScene.h"
#include "AprochStyle.h"
#include "AprochNode.h"
#include "AprochDataModelRegistry.h"

APROCH_NAMESPACE_BEGIN

AprochNodePainter::AprochNodePainter()
{
}

void AprochNodePainter::Paint(QPainter *painter, AprochNode *node, const AprochFlowScene &scene)
{
    node->recalculateSize(painter->font());

    DrawNodeRect(painter, node);
    DrawModelName(painter, node);
    DrawPortLabel(painter, node);
    DrawPort(painter, node, scene);
    DrawConnectedPort(painter, node);
}

void AprochNodePainter::DrawNodeRect(QPainter *painter, AprochNode *node)
{
    SNodeStyle const &nodeStyle = AprochStyle::GetNodeStyle();

    auto color = node->getNodeGraphicsObject().isSelected() ? nodeStyle.SelectedBoundaryColor : nodeStyle.NormalBoundaryColor;

    if (node->isHovered())
    {
        QPen p(color, nodeStyle.HoveredPenWidth);
        painter->setPen(p);
    }
    else
    {
        QPen p(color, nodeStyle.PenWidth);
        painter->setPen(p);
    }

    painter->setBrush(nodeStyle.BackgroundColor);

    double diam = double(nodeStyle.ConnectionPointDiameter);

    QRectF boundary(-diam, -diam, 2.0 * diam + node->getWidth(), 2.0 * diam + node->getHeight());

    painter->drawRoundedRect(boundary, NODE_BORDER_RADIUS, NODE_BORDER_RADIUS);
}

void AprochNodePainter::DrawModelName(QPainter *painter, AprochNode *node)
{
    auto const &model = node->getNodeDataModel();
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

    QPointF position((node->getWidth() - rect.width()) / 2.0, node->getSpacing() / 3.0);

    painter->setFont(f);
    painter->setPen(nodeStyle.FontColor);
    painter->drawText(position, name);

    f.setBold(false);
    painter->setFont(f);
}

void AprochNodePainter::DrawPortLabel(QPainter *painter, AprochNode *node)
{
    QFontMetrics const &metrics = painter->fontMetrics();

    for (EPortType portType : {EPortType::Output, EPortType::Input})
    {
        auto const &model = node->getNodeDataModel();
        auto const &nodeStyle = model->getNodeStyle();
        auto &entries = node->getEntries(portType);

        int n = entries.size();
        for (int i = 0; i < n; ++i)
        {
            QPointF p = node->getPortScenePosition(i, portType);

            if (entries[i].empty())
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
                s = model->dataType(portType, i).name;
            }

            auto rect = metrics.boundingRect(s);

            p.setY(p.y() + rect.height() / 4.0);

            switch (portType)
            {
            case EPortType::Input:
                p.setX(5.0);
                break;

            case EPortType::Output:
                p.setX(node->getWidth() - 5.0 - rect.width());
                break;

            default:
                break;
            }

            painter->drawText(p, s);
        }
    }
}

void AprochNodePainter::DrawPort(QPainter *painter, AprochNode *node, const AprochFlowScene &scene)
{
    auto const &model = node->getNodeDataModel();
    auto const &nodeStyle = model->getNodeStyle();

    float diameter = nodeStyle.ConnectionPointDiameter;
    auto reducedDiameter = diameter * 0.6;

    for (EPortType portType : {EPortType::Output, EPortType::Input})
    {
        size_t n = node->getEntries(portType).size();

        for (unsigned int i = 0; i < n; ++i)
        {
            QPointF p = node->getPortScenePosition(i, portType);

            auto const &dataType = model->dataType(portType, i);

            bool canConnect = (node->getEntries(portType)[i].empty() || (portType == EPortType::Output &&
                                model->portOutConnectionPolicy(i) == INodeDataModel::EConnectionPolicy::Many));

            double r = 1.0;
            if (node->isReacting() && canConnect && portType == node->getReactingPortType())
            {
                auto diff = node->getDraggingPos() - p;
                double distSqrt = QPointF::dotProduct(diff, diff);
                bool typeConvertable = false;

                {
                    if (portType == EPortType::Input)
                    {
                        typeConvertable = scene.registry().getTypeConverter(node->getReactingDataType(), dataType) != nullptr;
                    }
                    else
                    {
                        typeConvertable = scene.registry().getTypeConverter(dataType, node->getReactingDataType()) != nullptr;
                    }
                }

                if (node->getReactingDataType().id == dataType.id || typeConvertable)
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

void AprochNodePainter::DrawConnectedPort(QPainter *painter, AprochNode *node)
{
    auto const &model = node->getNodeDataModel();
    auto const &nodeStyle = model->getNodeStyle();
    auto diameter = nodeStyle.ConnectionPointDiameter;

    for (EPortType portType : {EPortType::Output, EPortType::Input})
    {
        size_t n = node->getEntries(portType).size();

        for (size_t i = 0; i < n; ++i)
        {
            QPointF p = node->getPortScenePosition(i, portType);

            if (!node->getEntries(portType)[i].empty())
            {
                painter->setPen(nodeStyle.FilledConnectionPointColor);
                painter->setBrush(nodeStyle.FilledConnectionPointColor);
                painter->drawEllipse(p, diameter * 0.4, diameter * 0.4);
            }
        }
    }
}

APROCH_NAMESPACE_END
