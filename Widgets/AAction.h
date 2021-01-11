/****************************************************************************
 * @file    AAction.h
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

#include <qaction.h>

#include "Framework/IJsonSerializable.h"

namespace aproch
{
    namespace widgets
    {
        /** 动作ID */
        using ActionId = QString;

        /**
         * UI动作
         */
        class WIDGETS_API AAction : public QAction, public aproch::framework::IJsonSerializable
        {
            Q_OBJECT

            Q_PROPERTY(ActionId mActionId READ getActionId WRITE setActionId NOTIFY actionIdChanged)
            Q_PROPERTY(aproch::framework::CommandId mCommandId READ getCommandId WRITE setCommandId NOTIFY commandIdChanged)
        public:
            explicit AAction(QObject* parent = nullptr);

            /**
             * 转化为json格式
             * @return QJsonObject* json对象指针
             */
            QJsonObject toJson(void) const override;

            /**
             * 从json格式解析
             * @return bool 是否解析成功
             */
            bool fromJson(const QJsonObject& jsonObject) override;

            /**
             * 获取动作ID
             * @return ActionId 动作Id
             */
            inline ActionId getActionId() const
            {
                return mActionId;
            }

            /** 
             * 设置动作Id
             * @param actionId 动作Id
             */
            void setActionId(const ActionId& actionId);

            /**
             * 获取命令ID
             * @return CommandId 命令Id
             */
            inline aproch::framework::CommandId getCommandId() const
            {
                return mActionId;
            }

            /**
             * 设置命令Id
             * @param commandId 命令Id
             */
            void setCommandId(const ActionId& commandId);

            /** 
             * 获取上下文使能表达式
             */
            inline aproch::framework::AContextKeyExpression getEnableExpression() const
            {
                return mEnableExpression;
            }

            /**
             * 设置上下文使能表达式
             * @param expr 上下文使能表达式
             */
            void setEnableExpression(const aproch::framework::AContextKeyExpression& expr);

        Q_SIGNALS:
            /**
             * 动作Id改变信号
             */
            void actionIdChanged();

            /**
             * 命令Id改变信号
             */
            void commandIdChanged();

            /**
             * 使能表达式改变信号
             */
            void enableExpressionChanged();

        protected:
            /** 动作ID */
            ActionId mActionId;

            /** 命令Id */
            aproch::framework::CommandId mCommandId;

            /** 上下文使能表达式 */
            aproch::framework::AContextKeyExpression mEnableExpression;
        };
    }
}