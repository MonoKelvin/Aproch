/****************************************************************************
 * @file    AFileToolkit.h
 * @date    2021-1-17
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

namespace aproch
{
    namespace framework
    {
        namespace toolkit
        {
            /**
             * 文件相关工具类 
             */
            class FRAMEWORK_API AFileToolkit
            {
            public:
                /**
                 * @brief 创建文件夹
                 * @param path 文件夹（路径）全称
                 * @param recursion 是否使用递归创建路径
                 *  - false: 必须保证上一级路径存在；
                 *  - true: 创建路径中所有不存在的路径文件夹。
                 * @return bool 文件夹（路径）创建成功返回true，否则返回false。
                 * @see CreateFile
                 */
                static bool CreateDiretory(const QString& path, bool recursion = true);

                /**
                 * @brief 创建文件
                 * @param fileName 文件名全称，包含路径和后缀名
                 * @param recursion 是否使用递归创建文件。
                 *  - false: 必须保证文件所在的文件夹存在；
                 *  - true: 会创建路径中所有不存在的路径文件夹。
                 * @return bool 文件创建成功返回true，否则返回false。
                 * @see CreateDiretory
                 */
                static bool CreateFile(const QString& fileName, bool recursion = true);

                /**
                 * @brief 复制一个文件
                 * @param sourceFile 源文件名全称，包括路径名称
                 * @param newFile 新文件名全称，文件不存在时会递归创建文件路径
                 * @param isOverWrite 是否覆盖已经存在的文件
                 * @return 复制成功返回true，否则返回false
                 */
                static bool CopyFile(const QString& sourceFile, const QString& newFile, bool isOverWrite = true);

                /**
                 * @brief 检测字符串是否含有文件非法字符
                 * @param str 用于检测的字符串
                 * @param checkEmpty 是否对字符串做空检查
                 *  - true: 字符串为空则直接返回false，不再检查非法字符
                 *  - false: 只检查是是否包含非法字符，如果str为空则会返回true
                 * @return 如果存在：\ / \ " * ? < > | 这9个非法字符则返回true，否则返回false
                 */
                static bool HasIllegalChar(const QString& str, bool checkEmpty = true);

                /**
                 * @brief 指定路径的文件存在
                 * @param filePath 文件路径全程，包括后缀名
                 * @return bool 文件是否存在
                 */
                static bool IsFileExists(const QString& filePath);

                /**
                 * @brief 是否指定路径存在
                 * @param path 路径名
                 * @return bool 路径是否存在
                 */
                static bool IsPathExists(const QString& path);
            };
        }
    }
}
