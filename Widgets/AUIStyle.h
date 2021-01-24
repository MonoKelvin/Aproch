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

// Ĭ��UI���
#define AprochDefaultUIStyle AStr("AprochDefaultUIStyle")

namespace aproch
{
    namespace widgets
    {
        using Boundary = QMargins;

        /**
         * ϵͳUI���
         * @note AUIStyle����һ��Ĭ�ϵķ������Ϊ"AprochDefaultUIStyle"
         */
        class WIDGETS_API AUIStyle
        {
        public:
            /** ������ƣ�����Ψһ�����������ֲ�ͬ�ķ�� */
            QString Name;

            //////////////////////////////////////////////////////////////////////////
            
            /** ˮƽ��� */
            int HorizontalSpacing;

            /** ��ֱ��� */
            int VerticalSpacing;

            /** ��߾� */
            Boundary Margin;

            /** �ڱ߾� */
            Boundary Spacing;

            /** �������Ͻǿ��ư�ť�ĳߴ� */
            QSize WindowControllerSize;

            //////////////////////////////////////////////////////////////////////////

            // ��Ҫ����ɫ
            QColor PrimaryColor;

            // ��Ҫ����ͣ��ɫ
            QColor PrimaryHoverColor;

            // ��Ҫ�ļ�����ɫ
            QColor PrimaryActiveColor;

            // �����ڱ���ɫ
            QColor MainWindowBackgrounColor;

            // ��̬���ڱ���ɫ
            QColor DynamicWindowCackgroundColor;

            // �ؼ�������ɫ
            QColor ControlBackgroundColor;

            // �����ͣ�µĿؼ�����ɫ
            QColor ControlHoverBackgroundColor;

            // ����Ŀؼ�����ɫ
            QColor ControlActiveBackgroundColor;

            // ���õĿؼ�����ɫ
            QColor ControlDisabledBackgroundColor;

            // �ؼ��߿�ɫ
            QColor ControlBorderColor;

            // ��Ҫ���ı���ɫ
            QColor PrimaryTextColor;

            // ��Ҫ���ı���ɫ
            QColor SubTextColor;

            // ���õ��ı���ɫ
            QColor DisabledTextColor;

            // �˵�����ɫ
            QColor MenuBackgroundColor;

            //////////////////////////////////////////////////////////////////////////

            // Ӧ������
            QFont AppFont;

        public:
            AUIStyle();
            AUIStyle(const AUIStyle& rhs);

            bool operator==(const AUIStyle& rhs) const;
            bool operator!=(const AUIStyle& rhs) const;

        public:
            /**
             * @brief ���һ��UI���
             * @param name 
             */
            static void AddUIStyle(const AUIStyle& uiStyle);

            /**
             * @brief ͨ�����ƻ�ȡUI���
             * @param name ����
             * @return AUIStyle UI�������
             */
            static AUIStyle& GetUIStyle(const QString& name);

        private:
            /** ��񼯺� */
            static QList<AUIStyle> sStyleList;
        };

        // ȫ��Ĭ����ʽ
        extern AUIStyle AppUIStyle;
    }
}