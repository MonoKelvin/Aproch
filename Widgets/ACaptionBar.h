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
         * 标题栏
         */
        class WIDGETS_API ACaptionBar : public QWidget
        {
            Q_OBJECT
        public:
            explicit ACaptionBar(QWidget* parent = nullptr);
            ~ACaptionBar(void);

            /**
             * @brief 获取菜单栏
             * @return 菜单栏
             */
            inline QMenuBar* getMenuBar() const noexcept
            {
                return mMenuBar;
            }

            /**
             * @brief 获取附件区域的水平布局容器
             * @note 附件区域的布局是标题栏预留的控件区域，开发者可以向里面添加自定义的各种控件
             * @return 附件区域的布局
             */
            inline QHBoxLayout* getAppendixLayout() const noexcept
            {
                return mAppendixLayout;
            }

            /**
             * @brief 获取最小化按钮
             * @return 最小化按钮
             */
            inline QPushButton* getMinimizeButton() const noexcept
            {
                return mBtnMinimize;
            }

            /**
             * @brief 获取最大化-还原按钮
             * @return 最大化-还原按钮
             */
            inline QPushButton* getMaxRestoreButton() const noexcept
            {
                return mBtnMaxRestore;
            }

            /**
             * @brief 获取关闭窗口按钮
             * @return 关闭窗口按钮
             */
            inline QPushButton* getCloseButton() const noexcept
            {
                return mBtnClose;
            }

        protected:
            virtual void paintEvent(QPaintEvent* event) override;

        protected:
            /** 菜单栏 */
            QMenuBar* mMenuBar;

            /** 附件区域的水平布局容器 */
            QHBoxLayout* mAppendixLayout;

            /** 最小化按钮 */
            QPushButton* mBtnMinimize;

            /** 最大化、还原按钮 */
            QPushButton* mBtnMaxRestore;

            /** 关闭按钮 */
            QPushButton* mBtnClose;
        };
    }
}