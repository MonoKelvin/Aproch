#ifndef APROCHNODEPAINTER_H
#define APROCHNODEPAINTER_H

#include "Aproch.h"

#include <QPainter>

APROCH_NAMESPACE_BEGIN

class ANode;
class AFlowScene;

class APROCH_EXPORT ANodePainter
{
public:
    static void Paint(QPainter *painter, ANode &node, AFlowScene &scene);

private:
    static void DrawNodeRect(QPainter *painter, ANode &node);
    static void DrawModelName(QPainter *painter, ANode &node);
    static void DrawPortLabel(QPainter *painter, ANode &node);
    static void DrawPort(QPainter *painter, ANode &node, AFlowScene &scene);
    static void DrawConnectedPort(QPainter *painter, ANode &node);
};

APROCH_NAMESPACE_END

#endif // APROCHNODEPAINTER_H
