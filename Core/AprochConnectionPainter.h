#ifndef APROCHCONNECTIONPAINTER_H
#define APROCHCONNECTIONPAINTER_H

#include "Aproch.h"

class QPainter;
class QPainterPath;

APROCH_NAMESPACE_BEGIN

class AprochConnection;

class AprochConnectionPainter
{
public:
    enum class EConnectionShape
    {
        /**
         * __
         *   )
         *  /
         * (__
         *
         */
        Cubic,

        /**
         * \
         *  \
         *   \
         *    \
         */
        StraightLine,

        /**
         * __
         *   \
         *    \
         *     \__
         */
        LeadLine
    };

    static void Paint(QPainter *painter, const AprochConnection &connection, EConnectionShape shape);
    static QPainterPath GetPainterStroke(const AprochConnection &connection);

private:
    static void DrawNormalLine(QPainter *painter, const AprochConnection &connection);
    static void DrawHoveredOrSelected(QPainter *painter, const AprochConnection &connection);
    static void DrawSketchLine(QPainter *painter, const AprochConnection &connection);

#ifdef APROCH_DEBUG_DRAWING
    static void DebugDrawing(QPainter *painter, const AprochConnection &connection);
#endif
};

APROCH_NAMESPACE_END

#endif // APROCHCONNECTIONPAINTER_H
