/****************************************************************************
 * @file    ACaptionBar.cpp
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
#include "ACaptionBar.h"

#include <QMenuBar>
#include <QPushButton>

namespace aproch
{
    namespace widgets
    {  
        ACaptionBar::ACaptionBar(QWidget* parent /*= nullptr*/)
            : QWidget(parent)
        {
            QHBoxLayout* hBoxLayout = new QHBoxLayout(this);
            
            // 菜单栏
            mMenuBar = new QMenuBar(this);
            hBoxLayout->addWidget(mMenuBar, 1);

            // 空闲布局
            mAppendixLayout = new QHBoxLayout(this);
            mAppendixLayout->setMargin(0);
            mAppendixLayout->setSpacing(AppUIStyle.HorizontalSpacing);
            hBoxLayout->addLayout(mAppendixLayout);

            // 控制按钮
            mBtnMinimize = new QPushButton("C", this);
            mBtnMaxRestore = new QPushButton("R", this);
            mBtnClose = new QPushButton("M", this);

            mBtnMinimize->setFixedSize(AppUIStyle.WindowControllerSize);
            mBtnMaxRestore->setFixedSize(AppUIStyle.WindowControllerSize);
            mBtnClose->setFixedSize(AppUIStyle.WindowControllerSize);
            
            // 控制按钮的布局
            QHBoxLayout* windowControllerLayout = new QHBoxLayout();
            windowControllerLayout->setMargin(0);
            windowControllerLayout->setSpacing(0);
            windowControllerLayout->addWidget(mBtnMinimize);
            windowControllerLayout->addWidget(mBtnMaxRestore);
            windowControllerLayout->addWidget(mBtnClose);
            hBoxLayout->addLayout(windowControllerLayout);
        }

        ACaptionBar::~ACaptionBar(void)
        {

        }

        void ACaptionBar::paintEvent(QPaintEvent* event)
        {
            APROCH_USE_STYLE_SHEET();

            return __super::paintEvent(event);
        }
    }
}