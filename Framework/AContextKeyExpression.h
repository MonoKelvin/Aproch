/****************************************************************************
 * @file    AContextKeyExpression.h
 * @date    2021-1-10
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

#include "IStringSerializable.h"

namespace aproch
{
    namespace framework
    {
        /**
         * 目前所支持的上下文键值表达式类型
         */
        enum class EContextActionExprType
        {
            None,       // 该表达式无效
            False,      // 真
            True,       // 假
            Defined,    // 是否定义
            And,        // 与
            Or,         // 或
            Not,        // 非
            Xor,        // 异或
            Equals,     // 等于
            NotEquals,  // 不等于
            Regex,      // 等于正则表达式
            NotRegex,   // 不等于该正则表达式
        };

        class IContextActionExpr
        {
        public:
            virtual ~IContextActionExpr(void) = default;

            virtual int compare(const IContextActionExpr& rhs) = 0;

            virtual int equals(const IContextActionExpr& rhs) = 0;

            virtual QStringList keys(void) = 0;
        };

        /**
         * 上下文键值表达式，该表达式返回值只有true和false
         */
        class FRAMEWORK_API AContextKeyExpression : public IStringSerializable
        {
        public:
            AContextKeyExpression();
            explicit AContextKeyExpression(const QString& exprString);
            virtual ~AContextKeyExpression();

            virtual QString serialize(void) const override;
            virtual bool deserialize(const QString& stringValue) override;

        protected:
            bool parse();

        protected:
            QString mExprString;
        };
    }
}
