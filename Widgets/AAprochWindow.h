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
         * @brief Aproch自定义窗口组件
         */
        class WIDGETS_API AAprochWindow : public QWidget, public QAbstractNativeEventFilter
        {
            Q_OBJECT
        public:
            explicit AAprochWindow(QWidget* parent = nullptr, Qt::WindowType type = Qt::Window);
            virtual ~AAprochWindow();

            /**
             * @brief 设置是否可以缩放缩放窗口
             * @param resizable 是否可以缩放窗口
             */
            inline void setResizable(bool resizable) noexcept
            {
                mIsResizable = resizable;
            }

            /**
             * @brief 获得窗口是否可以缩放
             * @return 窗口是否可以缩放
             */
            inline bool isResizable(void) const noexcept
            {
                return mIsResizable;
            }

            /**
             * @brief 设置窗口移动属性
             * @param movable 是否可以移动窗口
             *  - true: 如果窗口没有标题栏则设置为true也无效
             *  - false: 让窗口无法移动
             */
            inline void setMovable(bool movable) noexcept
            {
                mIsMovable = movable;
            }

            /**
             * @brief 获取窗口是否可以移动
             * @return 窗口是否可以移动
             */
            inline bool isMovable(void) const noexcept
            {
                return mIsMovable;
            }

            /**
             * @brief 设置窗口阴影
             * @param shadowParam 阴影参数
             */
            void setShadow(const SShadowParam& shadowParam);

            /**
             * @brief 获得窗口阴影参数
             * @return 窗口阴影参数
             */
            inline const SShadowParam getShadow(void) const noexcept
            {
                return mShadowParam;
            }

            /**
             * @brief 设置中心控件
             * @param widget 中心控件
             */
            void setCentralWidget(QWidget* widget);

            /**
             * @brief 获得中心控件
             * @return 中心控件
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
             * 拦截系统原生事件，主要用于处理标题栏、窗口拉伸的事件
             */
            virtual bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;

        protected:
            // 阴影参数
            SShadowParam mShadowParam;

            // 中心控件
            QWidget* mCentralWidget;

            // 阴影背景控件
            QWidget* mShadowBg;

            // 主要布局
            QBoxLayout* mMainLayout;

            /** 控件缩放边界 */
            int mScaleBoundary;

            // 是否可以缩放尺寸
            bool mIsResizable;

            // 是否可以移动
            bool mIsMovable;
        };

    }
}
