/****************************************************************************
 * @file    AAprochWindow.h
 * @date    2021-1-24
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

#include <QWidget>
#include <QAbstractNativeEventFilter>

namespace aproch
{
    namespace widgets
    {
        /**
         * @brief Aproch�Զ��崰�����
         */
        class WIDGETS_API AAprochWindow : public QWidget, public QAbstractNativeEventFilter
        {
            Q_OBJECT
        public:
            explicit AAprochWindow(QWidget* parent = nullptr, Qt::WindowType type = Qt::Window);
            virtual ~AAprochWindow();

            /**
             * @brief �����Ƿ�����������Ŵ���
             * @param resizable �Ƿ�������Ŵ���
             */
            inline void setResizable(bool resizable) noexcept
            {
                mIsResizable = resizable;
            }

            /**
             * @brief ��ô����Ƿ��������
             * @return �����Ƿ��������
             */
            inline bool isResizable(void) const noexcept
            {
                return mIsResizable;
            }

            /**
             * @brief ���ô����ƶ�����
             * @param movable �Ƿ�����ƶ�����
             *  - true: �������û�б�����������ΪtrueҲ��Ч
             *  - false: �ô����޷��ƶ�
             */
            inline void setMovable(bool movable) noexcept
            {
                mIsMovable = movable;
            }

            /**
             * @brief ��ȡ�����Ƿ�����ƶ�
             * @return �����Ƿ�����ƶ�
             */
            inline bool isMovable(void) const noexcept
            {
                return mIsMovable;
            }

            /**
             * @brief ���ô�����Ӱ
             * @param shadowParam ��Ӱ����
             */
            void setShadow(const SShadowParam& shadowParam);

            /**
             * @brief ��ô�����Ӱ����
             * @return ������Ӱ����
             */
            inline const SShadowParam getShadow(void) const noexcept
            {
                return mShadowParam;
            }

            /**
             * @brief �������Ŀؼ�
             * @param widget ���Ŀؼ�
             */
            void setCentralWidget(QWidget* widget);

            /**
             * @brief ������Ŀؼ�
             * @return ���Ŀؼ�
             */
            inline QWidget* getCentralWidget(void) const noexcept
            {
                return mCentralWidget;
            }

        protected:
            virtual void resizeEvent(QResizeEvent* event) override;
            virtual void changeEvent(QEvent* event) override;
            virtual bool eventFilter(QObject* watched, QEvent* event) override;

            /**
             * ����ϵͳԭ���¼�����Ҫ���ڴ��������������������¼�
             */
            virtual bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;

        protected:
            // ��Ӱ����
            SShadowParam mShadowParam;

            // ���Ŀؼ�
            QWidget* mCentralWidget;

            // ��Ӱ�����ؼ�
            QWidget* mShadowBg;

            // ��Ҫ����
            QBoxLayout* mMainLayout;

            /** �ؼ����ű߽� */
            int mScaleBoundary;

            // �Ƿ�������ųߴ�
            bool mIsResizable;

            // �Ƿ�����ƶ�
            bool mIsMovable;
        };

    }
}
