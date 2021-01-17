/****************************************************************************
 * @file    ACommandManager.cpp
 * @date    2021-1-10
 * @author  MonoKelvin
 * @email   15007083506@qq.com
 * @github  https://github.com/MonoKelvin
 * @brief
 *
 * This source file is part of Aproch.
 * Copyright (C) 2020 by MonoKelvin. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
#include "stdafx.h"
#include "ACommandManager.h"

#include <QDebug>

namespace aproch
{
    namespace framework
    {
        APROCH_INIT_SINGLETON(ACommandManager);

        ACommandManager::ACommandManager()
        {

        }

        ACommandManager::~ACommandManager()
        {

        }

        void ACommandManager::executeCommand(const CommandId& commandId)
        {
            for (const auto& cmd : mCommandList)
            {
                if (nullptr != cmd && cmd->getCommandId() == commandId)
                {
                    cmd->handle();
                }
            }
        }

        void ACommandManager::registerCommand(ACommand* command)
        {
            if (nullptr == command || command->getCommandId().isEmpty())
            {
                // TODO: ÃÌº”»’÷æ
                qDebug() << "Invalid Command";
                return;
            }

            if (!mCommandList.contains(command))
            {
                mCommandList.push_back(command);
            }
            else
            {
                qDebug() << "Command Exists";
            }
        }
        
        ACommand* ACommandManager::getCommand(const CommandId& commandId)
        {
            for (const auto& command : mCommandList)
            {
                if (command->getCommandId() == commandId)
                {
                    return command;
                }
            }

            return nullptr;
        }

    }
}