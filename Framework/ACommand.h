/****************************************************************************
 * @file    ACommand.h
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

#include <qobject.h>

namespace aproch
{
    namespace framework
    {
        /** 命令参数 */
        using CommandArgs = QVariantMap;

        /** 命令ID */
        using CommandId = QString;

        /**
         * 简单命令抽象类，不支持撤销还原
         */
        class FRAMEWORK_API ACommand : public QObject
        {
            Q_OBJECT
        public:
            explicit ACommand(QObject* parent = nullptr);
            virtual ~ACommand();

            /**
             * 处理命令的函数
             */
            virtual void handle(const CommandArgs& args) = 0;

            /**
             * 获取命令ID（常量）
             * @return const QString& 命令ID（常量）
             */
            inline const CommandId& getCommandId()
            {
                return mCommandId;
            }

            /**
             * 获取命令ID
             * @return const QString& 命令ID
             */
            inline CommandId getCommandId() const
            {
                return mCommandId;
            }

        Q_SIGNALS:
            /**
             * 信号 - 当命令执行成功
             */
            void resolved();

            /**
             * 信号 - 当命令执行失败
             */
            void rejected();

        protected:
            /** 命令ID */
            CommandId mCommandId;
        };
    }
}