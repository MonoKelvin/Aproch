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

#define APROCH_MAX_SCALE_BOUNDARY 10

class AMainWindow : public QMainWindow, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit AMainWindow(QWidget *parent = nullptr);

    /**
     * ����ϵͳԭ���¼�����Ҫ���ڴ��������������������¼�
     */
    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;

    /**
     * ��ȡ�˵���
     * @return AMenuBar* �˵����ؼ�
     */
    inline aproch::widgets::AMenuBar* getMenuBar(void) const noexcept
    {
        return mMenuBar;
    }

    /**
     * ��ȡ�������ؼ�
     * @return AToolBar* �������ؼ�
     */
    inline aproch::widgets::AToolBar* getToolBar(void) const noexcept
    {
        return mToolBar; 
    }

protected:
    Ui::AprochClass ui;

    /** �ؼ����ű߽� */
    int mScaleBoundary;

    /** �˵��� */
    aproch::widgets::AMenuBar* mMenuBar;

    /** ������ */
    aproch::widgets::AToolBar* mToolBar;
};
