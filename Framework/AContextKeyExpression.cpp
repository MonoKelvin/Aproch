#include "stdafx.h"
#include "AContextKeyExpression.h"

namespace aproch
{
    namespace framework
    {
        AContextKeyExpression::AContextKeyExpression()
        {
        }

        AContextKeyExpression::~AContextKeyExpression()
        {
        }
        
        QString AContextKeyExpression::serialize(void) const
        {
            return QString();
        }
        
        bool AContextKeyExpression::deserialize(const QString& stringValue)
        {
            return false;
        }
    }
}