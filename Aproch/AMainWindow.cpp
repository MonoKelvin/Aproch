/****************************************************************************
 * @file    AMainWindow.cpp
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
#include "stdafx.h"
#include "AMainWindow.h"

#ifdef Q_OS_WIN
#include <windowsx.h>
#endif

#define APROCH_MAX_SCALE_BOUNDARY 10

AMainWindow::AMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mScaleBoundary(8)
{
    ui.setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    setMinimumSize(200, 200);
    setStyleSheet("background:white");

    // 设置菜单栏
    mMenuBar = new aproch::widgets::AMenuBar(this);
    setMenuBar(mMenuBar);

    // 设置工具栏
    mToolBar = new aproch::widgets::AToolBar(this);
    addToolBar(Qt::TopToolBarArea, mToolBar);

    QAction* a = new QAction("Test1", this);
    QAction* a2 = new QAction("Test2", this);
    addAction(a);
    addAction(a2);
}

bool AMainWindow::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
{
    // 全屏或者最大化不处理缩放窗口缩放事件
    const bool isMaximizedOrFullScreen = isMaximized() || isFullScreen();

#ifdef Q_OS_WIN
    const auto& msg = (MSG*)message;
    switch (msg->message)
    {
    case WM_NCHITTEST:
        if (isMaximizedOrFullScreen)
        {
            return false;
        }

        const int xPos = GET_X_LPARAM(msg->lParam) - frameGeometry().x();
        const int yPos = GET_Y_LPARAM(msg->lParam) - frameGeometry().y();

        // 如果有其他子控件则优先处理该子控件
        if (nullptr != childAt(xPos, yPos))
        {
            return false;
        }

        *result = HTCAPTION;

        const bool isLeft = xPos > 0 && xPos < mScaleBoundary;
        const bool isRight = xPos > (width() - mScaleBoundary) && xPos < width();
        const bool isTop = yPos > 0 && yPos < mScaleBoundary;
        const bool isBottom = yPos > (height() - mScaleBoundary) && yPos < height();

        // 优先处理四个角落的事件
        if (isBottom && isRight)
        {
            *result = HTBOTTOMRIGHT;
        }
        else if (isTop && isRight)
        {
            *result = HTTOPRIGHT;
        }
        else if (isTop && isLeft)
        {
            *result = HTTOPLEFT;
        }
        else if (isBottom && isLeft)
        {
            *result = HTBOTTOMLEFT;
        }
        // 再处理上下左右缩放事件
        else if (isRight)
        {
            *result = HTRIGHT;
        }
        else if (isBottom)
        {
            *result = HTBOTTOM;
        }
        else if (isTop)
        {
            *result = HTTOP;
        }
        else if (isLeft)
        {
            *result = HTLEFT;
        }

        return true;
    }
#endif

    return false;
}
