#include "stdafx.h"
#include "ACommand.h"

namespace aproch
{
    namespace framework
    {
        ACommand::ACommand(QObject* parent)
            : QObject(parent)
        {
        }

        ACommand::~ACommand()
        {
        }
    }
}