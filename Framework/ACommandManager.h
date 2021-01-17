/****************************************************************************
 * @file    ACommandManager.h
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
#pragma once

namespace aproch
{
    namespace framework
    {
        /**
         * 命令管理器
         */
        class FRAMEWORK_API ACommandManager
        {
            APROCH_SINGLETON(ACommandManager);
        public:
            ~ACommandManager();

            /**
             * @brief 执行命令
             * @param commandId 要执行的命令ID 
             */
            void executeCommand(const CommandId& commandId);

            /**
             * 注册一个命令，如果命令存在则覆盖
             * @param command 命令
             * @note 命令注册成功会发送 commandRegisted 信号
             * @see commandRegisted
             */
            void registerCommand(ACommand* command);

            /**
             * 通过ID获取命令
             * @param commandId 命令ID
             * @return ACommand* 通过ID获取到的命令，如果不存在该命令返回nullptr
             */
            ACommand* getCommand(const CommandId& commandId);

            /**
             * 获取命令列表（引用）
             * @return QList<ACommand*> 命令列表引用
             */
            QList<ACommand*>& getCommandList()
            {
                return mCommandList;
            }

        private:
            ACommandManager();

        private:
            QList<ACommand*> mCommandList;
        };
    }
}
