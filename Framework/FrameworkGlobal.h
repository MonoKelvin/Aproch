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

#include <mutex>

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(FRAMEWORK_LIB)
#  define FRAMEWORK_API Q_DECL_EXPORT
# else
#  define FRAMEWORK_API Q_DECL_IMPORT
# endif
#else
# define FRAMEWORK_API
#endif

/** ����ӿ����ͺ� */
#ifdef interface
#undef interface
#endif
#define interface struct

/** ����ʵ�ֽӿں� */
#ifdef implement
#undef implement
#endif
#define implement public

/** ������������͹ؼ��� */
#ifdef abstract
#undef abstract
#endif
#define abstract

/**
 * @brief �����ַ���ת��Ϊ�����������QString�����Խ�������ʱ����QString�Ĵ���
 * @param _ConstString_ �����ַ�������: "String"
 * @note �ȼ���ʹ�� QStringLiteral(_ConstString_)
 * @see QStringLiteral
 */
#define AStr(_ConstString_) QStringLiteral(_ConstString_)

 /**
  * @brief �����ַ���ת��Ϊ���ֽ��ַ���QLatin1String���������п���֧��QLatin1String�����ĵط�
  * @param _ConstString_ �����ַ�������: "String"
  * @note �ȼ���ʹ�� QLatin1String(_ConstString_)
  * @see QLatin1String
 */
#define ALatin1(_ConstString_) QLatin1String(_ConstString_)

/**
 * @brief �����ַ���ת��Ϊ�ֽ�����QByteArray�����Խ�������ʱ����QByteArray�Ĵ���
 * @param _ConstString_ �����ַ�������: "Byte Array"
 * @note �ȼ���ʹ�� QByteArrayLiteral(_ConstString_)
 * @see QByteArrayLiteral
 */
#define AByteArr(_ConstString_) QByteArrayLiteral(_ConstString_)

 /** ʹ�øú������������࣬ע�������ĵ����������������������Ϊpublic */
#define APROCH_SINGLETON(_ClassName_)                  \
public:                                                \
    static _ClassName_* getInstance(void) noexcept;    \
private:                                               \
    static QSharedPointer<_ClassName_> mInstance;     \
    static std::mutex mMutex

/** ʹ�øú�����ʼ�������ͻ����� */
#define APROCH_INIT_SINGLETON(_ClassName_)                              \
QSharedPointer<_ClassName_> _ClassName_::mInstance(nullptr);            \
std::mutex _ClassName_::mMutex;                                         \
_ClassName_* _ClassName_::getInstance(void) noexcept                    \
{                                                                       \
    std::lock_guard<std::mutex> guard1(mMutex);                         \
    if (nullptr == mInstance){                                          \
        std::lock_guard<std::mutex> guard2(mMutex);                     \
        if (nullptr == mInstance) {                                     \
            mInstance = QSharedPointer<_ClassName_>(new _ClassName_()); \
        }                                                               \
    }                                                                   \
    return mInstance.get();                                             \
}