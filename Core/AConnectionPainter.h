#ifndef APROCHCONNECTIONPAINTER_H
#define APROCHCONNECTIONPAINTER_H

#include "Aproch.h"

class QPainter;
class QPainterPath;

APROCH_NAMESPACE_BEGIN

class AConnection;

class AConnectionPainter
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

    static void Paint(QPainter *painter, const AConnection &connection, EConnectionShape shape = EConnectionShape::Cubic);
    static QPainterPath GetPainterStroke(const AConnection &connection);

private:
    static void DrawNormalLine(QPainter *painter, const AConnection &connection);
    static void DrawHoveredOrSelected(QPainter *painter, const AConnection &connection);
    static void DrawSketchLine(QPainter *painter, const AConnection &connection);

#ifdef APROCH_DEBUG_DRAWING
    static void DebugDrawing(QPainter *painter, const AConnection &connection);
#endif
};

APROCH_NAMESPACE_END

#endif // APROCHCONNECTIONPAINTER_H
