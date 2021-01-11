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
        /** ����ID */
        using ActionId = QString;

        /**
         * UI����
         */
        class WIDGETS_API AAction : public QAction, public aproch::framework::IJsonSerializable
        {
            Q_OBJECT

            Q_PROPERTY(ActionId mActionId READ getActionId WRITE setActionId NOTIFY actionIdChanged)
            Q_PROPERTY(aproch::framework::CommandId mCommandId READ getCommandId WRITE setCommandId NOTIFY commandIdChanged)
        public:
            explicit AAction(QObject* parent = nullptr);

            /**
             * ת��Ϊjson��ʽ
             * @return QJsonObject* json����ָ��
             */
            QJsonObject toJson(void) const override;

            /**
             * ��json��ʽ����
             * @return bool �Ƿ�����ɹ�
             */
            bool fromJson(const QJsonObject& jsonObject) override;

            /**
             * ��ȡ����ID
             * @return ActionId ����Id
             */
            inline ActionId getActionId() const
            {
                return mActionId;
            }

            /** 
             * ���ö���Id
             * @param actionId ����Id
             */
            void setActionId(const ActionId& actionId);

            /**
             * ��ȡ����ID
             * @return CommandId ����Id
             */
            inline aproch::framework::CommandId getCommandId() const
            {
                return mActionId;
            }

            /**
             * ��������Id
             * @param commandId ����Id
             */
            void setCommandId(const ActionId& commandId);

            /** 
             * ��ȡ������ʹ�ܱ��ʽ
             */
            inline aproch::framework::AContextKeyExpression getEnableExpression() const
            {
                return mEnableExpression;
            }

            /**
             * ����������ʹ�ܱ��ʽ
             * @param expr ������ʹ�ܱ��ʽ
             */
            void setEnableExpression(const aproch::framework::AContextKeyExpression& expr);

        Q_SIGNALS:
            /**
             * ����Id�ı��ź�
             */
            void actionIdChanged();

            /**
             * ����Id�ı��ź�
             */
            void commandIdChanged();

            /**
             * ʹ�ܱ��ʽ�ı��ź�
             */
            void enableExpressionChanged();

        protected:
            /** ����ID */
            ActionId mActionId;

            /** ����Id */
            aproch::framework::CommandId mCommandId;

            /** ������ʹ�ܱ��ʽ */
            aproch::framework::AContextKeyExpression mEnableExpression;
        };
    }
}