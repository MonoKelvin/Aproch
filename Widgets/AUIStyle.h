/****************************************************************************
 * @file    AUIStyle.h
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
#pragma once

// 默认UI风格
#define AprochDefaultUIStyle AStr("AprochDefaultUIStyle")

namespace aproch
{
    namespace widgets
    {
        using Boundary = QMargins;

        /**
         * 系统UI风格
         * @note AUIStyle中有一个默认的风格，名称为"AprochDefaultUIStyle"
         */
        class WIDGETS_API AUIStyle
        {
        public:
            /** 风格名称（保持唯一），用以区分不同的风格 */
            QString Name;

            //////////////////////////////////////////////////////////////////////////
            
            /** 水平间隔 */
            int HorizontalSpacing;

            /** 垂直间隔 */
            int VerticalSpacing;

            /** 外边距 */
            Boundary Margin;

            /** 内边距 */
            Boundary Spacing;

            /** 窗口右上角控制按钮的尺寸 */
            QSize WindowControllerSize;

            //////////////////////////////////////////////////////////////////////////

            // 主要的颜色
            QColor PrimaryColor;

            // 主要的悬停颜色
            QColor PrimaryHoverColor;

            // 主要的激活颜色
            QColor PrimaryActiveColor;

            // 主窗口背景色
            QColor MainWindowBackgrounColor;

            // 动态窗口背景色
            QColor DynamicWindowCackgroundColor;

            // 控件背景颜色
            QColor ControlBackgroundColor;

            // 鼠标悬停下的控件背景色
            QColor ControlHoverBackgroundColor;

            // 激活的控件背景色
            QColor ControlActiveBackgroundColor;

            // 禁用的控件背景色
            QColor ControlDisabledBackgroundColor;

            // 控件边框色
            QColor ControlBorderColor;

            // 主要的文本颜色
            QColor PrimaryTextColor;

            // 次要的文本颜色
            QColor SubTextColor;

            // 禁用的文本颜色
            QColor DisabledTextColor;

            // 菜单背景色
            QColor MenuBackgroundColor;

            //////////////////////////////////////////////////////////////////////////

            // 应用字体
            QFont AppFont;

        public:
            AUIStyle();
            AUIStyle(const AUIStyle& rhs);

            bool operator==(const AUIStyle& rhs) const;
            bool operator!=(const AUIStyle& rhs) const;

        public:
            /**
             * @brief 添加一个UI风格
             * @param name 
             */
            static void AddUIStyle(const AUIStyle& uiStyle);

            /**
             * @brief 通过名称获取UI风格
             * @param name 名称
             * @return AUIStyle UI风格引用
             */
            static AUIStyle& GetUIStyle(const QString& name);

        private:
            /** 风格集合 */
            static QList<AUIStyle> sStyleList;
        };

        // 全局默认样式
        extern AUIStyle AppUIStyle;
    }
}