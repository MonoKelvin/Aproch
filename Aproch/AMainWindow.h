/****************************************************************************
 * @file    AMainWindow.h
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

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

namespace aproch
{
    /**
     * @brief 软件主窗口
     */
    class AMainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit AMainWindow(QWidget* parent = nullptr);

        /**
         * 获取标题栏
         * @return AMenuBar* 标题栏控件
         */
        inline aproch::widgets::ACaptionBar* getCaptionBar(void) const noexcept
        {
            return mCaptionBar;
        }

        /**
         * 获取工具栏控件
         * @return AToolBar* 工具栏控件
         */
        inline aproch::widgets::AToolBar* getToolBar(void) const noexcept
        {
            return mToolBar;
        }

    protected:
        Ui::AprochClass ui;

        /** 菜单栏 */
        aproch::widgets::ACaptionBar* mCaptionBar;

        /** 工具栏 */
        aproch::widgets::AToolBar* mToolBar;
    };
}