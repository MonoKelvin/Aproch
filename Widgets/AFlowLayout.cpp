/****************************************************************************
 * @file    AFlowLayout.cpp
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
#include "AFlowLayout.h"

namespace aproch
{
    namespace widgets
    {
        AFlowLayout::AFlowLayout(QWidget* parent)
            : QLayout(parent)
            , mHorizontalSpacing(AppUIStyle.HorizontalSpacing)
            , mVerticalSpacing(AppUIStyle.VerticalSpacing)
        {
            setContentsMargins(AppUIStyle.Margin);
        }

        AFlowLayout::~AFlowLayout()
        {
            QLayoutItem* item;
            while ((item = takeAt(0)))
            {
                delete item; 
                item = nullptr;
            }
        }

        void AFlowLayout::addItem(QLayoutItem* item)
        {
            mItemList.append(item);
        }

        int AFlowLayout::horizontalSpacing() const
        {
            if (mHorizontalSpacing >= 0)
            {
                return mHorizontalSpacing;
            }
            else
            {
                return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
            }
        }

        int AFlowLayout::verticalSpacing() const
        {
            if (mVerticalSpacing >= 0)
            {
                return mVerticalSpacing;
            }
            else
            {
                return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
            }
        }

        void AFlowLayout::setHorizontalSpacing(int value)
        {
            mHorizontalSpacing = value;
        }

        void AFlowLayout::setVerticalSpacing(int value)
        {
            mVerticalSpacing = value;
        }

        Qt::Orientations AFlowLayout::expandingDirections() const
        {
            return Qt::Orientations();
        }

        bool AFlowLayout::hasHeightForWidth() const
        {
            return true;
        }

        int AFlowLayout::heightForWidth(int width) const
        {
            return doLayout(QRect(0, 0, width, 0), true);
        }

        int AFlowLayout::count() const
        {
            return mItemList.size();
        }

        QLayoutItem* AFlowLayout::itemAt(int index) const
        {
            return mItemList.value(index);
        }

        QSize AFlowLayout::minimumSize() const
        {
            QSize size;
            for (const QLayoutItem* item : qAsConst(mItemList))
            {
                size = size.expandedTo(item->minimumSize());
            }

            const QMargins margins = contentsMargins();
            size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());

            return size;
        }

        void AFlowLayout::setGeometry(const QRect& rect)
        {
            QLayout::setGeometry(rect);
            doLayout(rect, false);
        }

        QSize AFlowLayout::sizeHint() const
        {
            return minimumSize();
        }

        QLayoutItem* AFlowLayout::takeAt(int index)
        {
            if (index >= 0 && index < mItemList.size())
            {
                return mItemList.takeAt(index);
            }

            return nullptr;
        }

        int AFlowLayout::doLayout(const QRect& rect, bool testOnly) const
        {
            int left, top, right, bottom;
            getContentsMargins(&left, &top, &right, &bottom);

            const QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);

            int x = effectiveRect.x();
            int y = effectiveRect.y();
            int lineHeight = 0;

            for (QLayoutItem* item : qAsConst(mItemList))
            {
                const QWidget* wid = item->widget();
                int spaceX = horizontalSpacing();
                if (spaceX == -1)
                {
                    spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
                }

                int spaceY = verticalSpacing();
                if (spaceY == -1)
                {
                    spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
                }

                int nextX = x + item->sizeHint().width() + spaceX;
                if (nextX - spaceX > effectiveRect.right() && lineHeight > 0)
                {
                    x = effectiveRect.x();
                    y = y + lineHeight + spaceY;
                    nextX = x + item->sizeHint().width() + spaceX;
                    lineHeight = 0;
                }

                if (!testOnly)
                {
                    item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
                }

                x = nextX;
                lineHeight = qMax(lineHeight, item->sizeHint().height());
            }

            return y + lineHeight - rect.y() + bottom;
        }

        int AFlowLayout::smartSpacing(QStyle::PixelMetric pm) const
        {
            const auto& parent = this->parent();
            if (nullptr == parent)
            {
                return -1;
            }
            else if (parent->isWidgetType())
            {
                const auto& pw = static_cast<QWidget*>(parent);
                return pw->style()->pixelMetric(pm, nullptr, pw);
            }
            else
            {
                return static_cast<QLayout*>(parent)->spacing();
            }
        }
    }
}