/****************************************************************************
 * @file    Macros.h
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

/**
 * @brief 常量字符串转换为编译器构造的QString，用以降低运行时构造QString的代价
 * @param _ConstString_ 常量字符串，如: "String"
 * @note 等价于使用 QStringLiteral(_ConstString_)
 * @see QStringLiteral
 */
#define AStr(_ConstString_) QStringLiteral(_ConstString_)

 /**
  * @brief 常量字符串转换为单字节字符串QLatin1String，用在所有可以支持QLatin1String参数的地方
  * @param _ConstString_ 常量字符串，如: "String"
  * @note 等价于使用 QLatin1String(_ConstString_)
  * @see QLatin1String
 */
#define ALatin1(_ConstString_) QLatin1String(_ConstString_)

/**
 * @brief 常量字符串转换为字节数组QByteArray，用以降低运行时构造QByteArray的代价
 * @param _ConstString_ 常量字符串，如: "Byte Array"
 * @note 等价于使用 QByteArrayLiteral(_ConstString_)
 * @see QByteArrayLiteral
 */
#define AByteArr(_ConstString_) QByteArrayLiteral(_ConstString_)
