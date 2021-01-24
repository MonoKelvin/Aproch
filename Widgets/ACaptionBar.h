/****************************************************************************
 * @file    ACaptionBar.h
 * @date    2021-1-9
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

#include <QFrame>

class QPushButton;

namespace aproch
{
    namespace widgets
    {
        class AFlowLayout;

        /**
         * ������
         */
        class WIDGETS_API ACaptionBar : public QWidget
        {
            Q_OBJECT
        public:
            explicit ACaptionBar(QWidget* parent = nullptr);
            ~ACaptionBar(void);

            /**
             * @brief ��ȡ�˵���
             * @return �˵���
             */
            inline QMenuBar* getMenuBar() const noexcept
            {
                return mMenuBar;
            }

            /**
             * @brief ��ȡ���������ˮƽ��������
             * @note ��������Ĳ����Ǳ�����Ԥ���Ŀؼ����򣬿����߿�������������Զ���ĸ��ֿؼ�
             * @return ��������Ĳ���
             */
            inline QHBoxLayout* getAppendixLayout() const noexcept
            {
                return mAppendixLayout;
            }

            /**
             * @brief ��ȡ��С����ť
             * @return ��С����ť
             */
            inline QPushButton* getMinimizeButton() const noexcept
            {
                return mBtnMinimize;
            }

            /**
             * @brief ��ȡ���-��ԭ��ť
             * @return ���-��ԭ��ť
             */
            inline QPushButton* getMaxRestoreButton() const noexcept
            {
                return mBtnMaxRestore;
            }

            /**
             * @brief ��ȡ�رմ��ڰ�ť
             * @return �رմ��ڰ�ť
             */
            inline QPushButton* getCloseButton() const noexcept
            {
                return mBtnClose;
            }

        protected:
            virtual void paintEvent(QPaintEvent* event) override;

        protected:
            /** �˵��� */
            QMenuBar* mMenuBar;

            /** ���������ˮƽ�������� */
            QHBoxLayout* mAppendixLayout;

            /** ��С����ť */
            QPushButton* mBtnMinimize;

            /** ��󻯡���ԭ��ť */
            QPushButton* mBtnMaxRestore;

            /** �رհ�ť */
            QPushButton* mBtnClose;
        };
    }
}