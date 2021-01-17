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
         * ���������
         */
        class FRAMEWORK_API ACommandManager
        {
            APROCH_SINGLETON(ACommandManager);
        public:
            ~ACommandManager();

            /**
             * @brief ִ������
             * @param commandId Ҫִ�е�����ID 
             */
            void executeCommand(const CommandId& commandId);

            /**
             * ע��һ����������������򸲸�
             * @param command ����
             * @note ����ע��ɹ��ᷢ�� commandRegisted �ź�
             * @see commandRegisted
             */
            void registerCommand(ACommand* command);

            /**
             * ͨ��ID��ȡ����
             * @param commandId ����ID
             * @return ACommand* ͨ��ID��ȡ���������������ڸ������nullptr
             */
            ACommand* getCommand(const CommandId& commandId);

            /**
             * ��ȡ�����б����ã�
             * @return QList<ACommand*> �����б�����
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
