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
             * �ļ���ع����� 
             */
            class FRAMEWORK_API AFileToolkit
            {
            public:
                /**
                 * @brief �����ļ���
                 * @param path �ļ��У�·����ȫ��
                 * @param recursion �Ƿ�ʹ�õݹ鴴��·��
                 *  - false: ���뱣֤��һ��·�����ڣ�
                 *  - true: ����·�������в����ڵ�·���ļ��С�
                 * @return bool �ļ��У�·���������ɹ�����true�����򷵻�false��
                 * @see CreateFile
                 */
                static bool CreateDiretory(const QString& path, bool recursion = true);

                /**
                 * @brief �����ļ�
                 * @param fileName �ļ���ȫ�ƣ�����·���ͺ�׺��
                 * @param recursion �Ƿ�ʹ�õݹ鴴���ļ���
                 *  - false: ���뱣֤�ļ����ڵ��ļ��д��ڣ�
                 *  - true: �ᴴ��·�������в����ڵ�·���ļ��С�
                 * @return bool �ļ������ɹ�����true�����򷵻�false��
                 * @see CreateDiretory
                 */
                static bool CreateFile(const QString& fileName, bool recursion = true);

                /**
                 * @brief ����һ���ļ�
                 * @param sourceFile Դ�ļ���ȫ�ƣ�����·������
                 * @param newFile ���ļ���ȫ�ƣ��ļ�������ʱ��ݹ鴴���ļ�·��
                 * @param isOverWrite �Ƿ񸲸��Ѿ����ڵ��ļ�
                 * @return ���Ƴɹ�����true�����򷵻�false
                 */
                static bool CopyFile(const QString& sourceFile, const QString& newFile, bool isOverWrite = true);

                /**
                 * @brief ����ַ����Ƿ����ļ��Ƿ��ַ�
                 * @param str ���ڼ����ַ���
                 * @param checkEmpty �Ƿ���ַ������ռ��
                 *  - true: �ַ���Ϊ����ֱ�ӷ���false�����ټ��Ƿ��ַ�
                 *  - false: ֻ������Ƿ�����Ƿ��ַ������strΪ����᷵��true
                 * @return ������ڣ�\ / \ " * ? < > | ��9���Ƿ��ַ��򷵻�true�����򷵻�false
                 */
                static bool HasIllegalChar(const QString& str, bool checkEmpty = true);

                /**
                 * @brief ָ��·�����ļ�����
                 * @param filePath �ļ�·��ȫ�̣�������׺��
                 * @return bool �ļ��Ƿ����
                 */
                static bool IsFileExists(const QString& filePath);

                /**
                 * @brief �Ƿ�ָ��·������
                 * @param path ·����
                 * @return bool ·���Ƿ����
                 */
                static bool IsPathExists(const QString& path);
            };
        }
    }
}
