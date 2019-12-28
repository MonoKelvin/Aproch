#ifndef APROCHNODEPAINTER_H
#define APROCHNODEPAINTER_H

#include "Aproch.h"

#include <QPainter>

APROCH_NAMESPACE_BEGIN

class AprochNode;
class AprochFlowScene;

class AprochNodePainter
{
public:
    static void Paint(QPainter* painter, AprochNode &node, const AprochFlowScene &scene);

private:
    static void DrawNodeRect(QPainter* painter, AprochNode &node);
    static void DrawModelName(QPainter* painter, AprochNode &node);
    static void DrawPortLabel(QPainter* painter, AprochNode &node);
    static void DrawPort(QPainter* painter, AprochNode &node, const AprochFlowScene &scene);
    static void DrawConnectedPort(QPainter* painter, AprochNode &node);
};

APROCH_NAMESPACE_END

#endif // APROCHNODEPAINTER_H
