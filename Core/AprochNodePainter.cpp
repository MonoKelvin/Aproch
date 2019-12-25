#include "AprochNodePainter.h"

#include "AprochStyle.h"
#include "AprochNode.h"

APROCH_NAMESPACE_BEGIN

AprochNodePainter::AprochNodePainter()
{

}

void AprochNodePainter::Paint(QPainter *painter, AprochNode *node, const AprochFlowScene &scene)
{
    DrawNodeRect(painter, node);
    DrawModelName(painter, node);
    DrawPortLabel(painter, node);
    DrawPort(painter, node, scene);
    DrawConnectedPort(painter, node);
}

void AprochNodePainter::DrawNodeRect(QPainter *painter, AprochNode *node)
{
    SNodeStyle const& nodeStyle = AprochStyle::GetNodeStyle();

    auto color = node->getNodeGraphicsObject().isSelected()
            ? nodeStyle.SelectedBoundaryColor
            : nodeStyle.NormalBoundaryColor;

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

}

void AprochNodePainter::DrawPortLabel(QPainter *painter, AprochNode *node)
{

}

void AprochNodePainter::DrawPort(QPainter *painter, AprochNode *node, const AprochFlowScene &scene)
{

}

void AprochNodePainter::DrawConnectedPort(QPainter *painter, AprochNode *node)
{

}

APROCH_NAMESPACE_END
