/****************************************************************************
 * @file    AAprochWindow.cpp
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
#include "stdafx.h"
#include "AAprochWindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

#ifdef Q_OS_WIN
#include <windows.h>
#include <windowsx.h>
#endif

namespace aproch
{
    namespace widgets
    {
        AAprochWindow::AAprochWindow(QWidget* parent, Qt::WindowType type)
            : QWidget(parent, type)
            , mCentralWidget(nullptr)
            , mShadowBg(nullptr)
            , mMainLayout(nullptr)
            , mScaleBoundary(APROCH_RESIZE_BOUNDARY)
            , mIsResizable(true)
            , mIsMovable(true)
        {
            setMaximumSize(QGuiApplication::primaryScreen()->size());
            setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
            setAttribute(Qt::WA_TranslucentBackground, true);
            setMouseTracking(true);

            // 阴影背景
            mShadowBg = new QWidget(this);
            mShadowBg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            mShadowBg->setObjectName(AStr("aproch_window_shadow_bg"));

            // TODO: DEBUG
            mShadowBg->setStyleSheet("background:#222223;");

            // 主布局
            mMainLayout = new QBoxLayout(QBoxLayout::TopToBottom, mShadowBg);
            mMainLayout->setSpacing(0);
            mMainLayout->setMargin(0);

            // 调整显示层次
            mShadowBg->lower();

            setShadow({});
        }

        AAprochWindow::~AAprochWindow()
        {
        }

        void AAprochWindow::setShadow(const SShadowParam& shadowParam)
        {
            if (nullptr == mShadowBg->graphicsEffect())
            {
                QGraphicsDropShadowEffect* pEffect = new QGraphicsDropShadowEffect(mShadowBg);

                pEffect->setBlurRadius(shadowParam.radius);
                pEffect->setXOffset(shadowParam.xOffset);
                pEffect->setYOffset(shadowParam.yOffset);
                pEffect->setColor(shadowParam.color);

                mShadowBg->setGraphicsEffect(pEffect);
            }

            mShadowBg->graphicsEffect()->setEnabled(shadowParam.isEnabled);

            mShadowParam = shadowParam;
        }

        void AAprochWindow::setCentralWidget(QWidget* widget)
        {
            if (mCentralWidget == widget)
            {
                return;
            }

            if (nullptr != mCentralWidget)
            {
                delete mCentralWidget;
                mCentralWidget = nullptr;
            }

            if (nullptr != widget)
            {
                mCentralWidget = widget;

                if (mCentralWidget->parentWidget() != mShadowBg)
                {
                    mCentralWidget->setParent(mShadowBg);
                }

                mMainLayout->insertWidget(0, mCentralWidget);

                mCentralWidget->installEventFilter(this);
                mCentralWidget->raise();
            }
        }

        void AAprochWindow::resizeEvent(QResizeEvent* event)
        {
            if (mShadowParam.isEnabled)
            {
                const qreal r = mShadowParam.radius;
                mShadowBg->setGeometry(QRect(r - mShadowParam.xOffset, r - mShadowParam.yOffset,
                                       width() - 2 * r, height() - 2 * r));
            }
            else if (isMaximized() || isFullScreen())
            {
                mShadowBg->setGeometry(rect());
            }
            else
            {
                mShadowBg->setGeometry(APROCH_RESIZE_BOUNDARY, 
                                       APROCH_RESIZE_BOUNDARY,
                                       width() - 2 * APROCH_RESIZE_BOUNDARY, 
                                       height() - 2 * APROCH_RESIZE_BOUNDARY);
            }

            return QWidget::resizeEvent(event);
        }

        void AAprochWindow::changeEvent(QEvent* event)
        {
            switch (windowState())
            {
            case Qt::WindowMaximized:
            case Qt::WindowMinimized:
                mShadowBg->setGeometry(rect());
                mShadowBg->graphicsEffect()->setEnabled(false);
                break;
            case Qt::WindowNoState:
                if (mShadowParam.isEnabled && mShadowBg && mShadowBg->graphicsEffect())
                {
                    mShadowBg->graphicsEffect()->setEnabled(true);
                }
                break;
            default:
                break;
            }

            return QWidget::changeEvent(event);
        }

        bool AAprochWindow::eventFilter(QObject* watched, QEvent* event)
        {
            if (watched == mCentralWidget)
            {
                const auto& msEvt = static_cast<QMouseEvent*>(event);
                if (!(msEvt->buttons() & Qt::LeftButton))
                {
                    return false;
                }

                switch (event->type())
                {
                case QMouseEvent::MouseButtonDblClick:
                    if (isMaximized())
                    {
                        showNormal();
                    }
                    else
                    {
                        showMaximized();
                    }

                    return true;
                default:
                    break;
                }
            }

            return QWidget::eventFilter(watched, event);
        }
        
        bool AAprochWindow::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
        {
            // 全屏或者最大化不处理缩放窗口缩放事件
            const bool isMaximizedOrFullScreen = isMaximized() || isFullScreen();

#ifdef Q_OS_WIN
            const auto& msg = (MSG*)message;
            switch (msg->message)
            {
            case WM_NCLBUTTONDBLCLK:
                *result = HTCAPTION;
                return true;
            case WM_NCHITTEST:
                if (isMaximizedOrFullScreen)
                {
                    return false;
                }

                const int xPos = GET_X_LPARAM(msg->lParam) - frameGeometry().x();
                const int yPos = GET_Y_LPARAM(msg->lParam) - frameGeometry().y();

                const bool isLeft = xPos > 0 && xPos < mScaleBoundary;
                const bool isRight = xPos > (width() - mScaleBoundary) && xPos < width();
                const bool isTop = yPos > 0 && yPos < mScaleBoundary;
                const bool isBottom = yPos > (height() - mScaleBoundary) && yPos < height();

                // 如果有其他子控件并且不处于缩放状态，则处理该子控件的事件
                const auto& childWidget = childAt(xPos, yPos);
                if (!isLeft && !isRight && !isTop && !isBottom && nullptr != childWidget)
                {
                    //// 处理点击菜单栏
                    //if (childWidget == mCaptionBar)
                    //{
                    //    const QPoint winToMenuPos = QPoint(xPos, yPos) - mCaptionBar->pos();
                    //    if (nullptr == mCaptionBar->childAt(winToMenuPos))
                    //    {
                    //        *result = HTCAPTION;
                    //        return true;
                    //    }
                    //}
                    return false;
                }

                if (!mIsResizable)
                {
                    return false;
                }

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
    }
}