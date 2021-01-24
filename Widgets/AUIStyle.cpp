/****************************************************************************
 * @file    AUIStyle.cpp
 * @date    2021-1-23
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
#include "AUIStyle.h"

namespace aproch
{
    namespace widgets
    {
        AUIStyle AppUIStyle;

        QList<AUIStyle> AUIStyle::sStyleList;

        AUIStyle::AUIStyle()
            : Name(AprochDefaultUIStyle)
            , HorizontalSpacing(4)
            , VerticalSpacing(4)
            , Margin(QMargins(0, 0, 0, 0))
            , Spacing(QMargins(0, 0, 0, 0))
            , WindowControllerSize(46, 32)
            , PrimaryColor(ALatin1("#007acc"))
            , PrimaryHoverColor(ALatin1("#3395d6"))
            , PrimaryActiveColor(ALatin1("#3395d6"))
            , MainWindowBackgrounColor(ALatin1("#2d2d30"))
            , DynamicWindowCackgroundColor(ALatin1("#252526"))
            , ControlBackgroundColor(ALatin1("#333337"))
            , ControlHoverBackgroundColor(ALatin1("#3f3f41"))
            , ControlActiveBackgroundColor(ALatin1())
            , ControlDisabledBackgroundColor(ALatin1("#535355"))
            , ControlBorderColor(ALatin1("#3f3f46"))
            , PrimaryTextColor(ALatin1("#f1f1f1"))
            , SubTextColor(ALatin1())
            , DisabledTextColor(ALatin1())
            , MenuBackgroundColor(ALatin1("#1b1b1c"))
            , AppFont(AStr("Microsoft Yahei"))
        {
        }

        AUIStyle::AUIStyle(const AUIStyle& rhs)
            : Name(rhs.Name)
            , HorizontalSpacing(rhs.HorizontalSpacing)
            , VerticalSpacing(rhs.VerticalSpacing)
            , Margin(rhs.Margin)
            , Spacing(rhs.Spacing)
            , WindowControllerSize(rhs.WindowControllerSize)
            , PrimaryColor(rhs.PrimaryColor)
            , PrimaryHoverColor(rhs.PrimaryHoverColor)
            , PrimaryActiveColor(rhs.PrimaryActiveColor)
            , MainWindowBackgrounColor(rhs.MainWindowBackgrounColor)
            , DynamicWindowCackgroundColor(rhs.DynamicWindowCackgroundColor)
            , ControlBackgroundColor(rhs.ControlBackgroundColor)
            , ControlHoverBackgroundColor(rhs.ControlHoverBackgroundColor)
            , ControlActiveBackgroundColor(rhs.ControlActiveBackgroundColor)
            , ControlDisabledBackgroundColor(rhs.ControlDisabledBackgroundColor)
            , ControlBorderColor(rhs.ControlBorderColor)
            , PrimaryTextColor(rhs.PrimaryTextColor)
            , SubTextColor(rhs.SubTextColor)
            , DisabledTextColor(rhs.DisabledTextColor)
            , MenuBackgroundColor(rhs.MenuBackgroundColor)
            , AppFont(rhs.AppFont)
        {
        }

        bool AUIStyle::operator==(const AUIStyle& rhs) const
        {
            return Name == rhs.Name;
        }

        bool AUIStyle::operator!=(const AUIStyle& rhs) const
        {
            return Name != rhs.Name;
        }

        void AUIStyle::AddUIStyle(const AUIStyle& uiStyle)
        {
            if (!sStyleList.contains(uiStyle))
            {
                sStyleList.push_front(uiStyle);
            }
        }

        AUIStyle& AUIStyle::GetUIStyle(const QString& name)
        {
            for (auto iter = sStyleList.begin(); iter != sStyleList.end(); ++iter)
            {
                if (iter->Name == name)
                {
                    return *iter;
                }
            }
        }

    }
}
