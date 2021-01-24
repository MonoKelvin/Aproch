/****************************************************************************
 * @file    WidgetsGlobal.h
 * @date    2021-1-12
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

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(WIDGETS_LIB)
#  define WIDGETS_API Q_DECL_EXPORT
# else
#  define WIDGETS_API Q_DECL_IMPORT
# endif
#else
# define WIDGETS_API
#endif

#include <QStyleOption>
#include <QStylePainter>

 /**
  * @brief �̳���QWidget���������ʵ���Լ���paintEvent��������ô������޷�ʹ����ʽ��
  * �ı���ʽ��ͨ���ú��������ʽ���иı䵱ǰ�ؼ�����ʽ
  * @note �ú�������`paintEvent`�����С�
  */
#define APROCH_USE_STYLE_SHEET()                       \
{                                                      \
    QStylePainter _Painter_(this);                     \
    QStyleOption _Opt_;                                \
    _Opt_.initFrom(this);                              \
    _Opt_.rect = rect();                               \
    _Painter_.drawPrimitive(QStyle::PE_Widget, _Opt_); \
}

// ����������ֵ
#define APROCH_RESIZE_BOUNDARY 6

/** @brief ��Ӱ���� */
struct SShadowParam
{
    /** @brief ��Ӱ��ɫ */
    QColor color;

    /** @brief ģ���뾶 */
    qreal radius;

    /** @brief X����ƫ�� */
    qreal xOffset;

    /** @brief Y����ƫ�� */
    qreal yOffset;

    /** @brief �Ƿ����� */
    bool isEnabled;

    SShadowParam(void) noexcept
        : color("#292929")
        , radius(20.0)
        , xOffset(0.0)
        , yOffset(0.0)
        , isEnabled(true)
    {
    }
};