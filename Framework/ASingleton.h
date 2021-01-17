/****************************************************************************
 * @file    ASingleton.h
 * @date    2021-1-9
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
#ifndef _APROCH_ASINGLETON_H_
#define _APROCH_ASINGLETON_H_

#include <mutex>

/** 继承自ASingleton的单例类，需要将ASingleton声明为友元 */
//#define APROCH_DECLARE_SINGLETON(_ClassName_) private: friend class aproch::framework::ASingleton<_ClassName_>

/** 不继承ASingleton的单例类，可以使用该宏来声明 */
#define APROCH_SINGLETON(_ClassName_)                  \
public:                                                \
    static _ClassName_* getInstance(void) noexcept;    \
private:                                               \
    static std::shared_ptr<_ClassName_> mInstance;     \
    static std::mutex mMutex

/** 不继承ASingleton的单例类，可以使用该宏来初始化单例和互斥锁 */
#define APROCH_INIT_SINGLETON(_ClassName_)                      \
std::shared_ptr<_ClassName_> _ClassName_::mInstance = nullptr;  \
std::mutex _ClassName_::mMutex;                                 \
_ClassName_* _ClassName_::getInstance(void) noexcept            \
{                                                               \
    std::lock_guard<std::mutex> guard1(mMutex);                 \
    if (nullptr == mInstance){                                  \
        std::lock_guard<std::mutex> guard2(mMutex);             \
        if (nullptr == mInstance) {                             \
            mInstance = new _ClassName_();                      \
        }                                                       \
    }                                                           \
    return mInstance.get();                                     \
}

//namespace aproch
//{
//    namespace framework
//    {
//        /**
//         * 单例模板基类
//         */
//        template<class DerivedClass>
//        class ASingleton
//        {
//        public:
//            ASingleton(const ASingleton&) = delete;
//            ASingleton& operator =(const ASingleton&) = delete;
//            virtual ~ASingleton(void) noexcept = default;
//
//            /**
//             * @brief 获取单例对象
//             * @return 对象实例
//             */
//            static DerivedClass* getInstance(void) noexcept
//            {
//                std::lock_guard<std::mutex> guard1(mMutex);
//                if (nullptr == mInstance)
//                {
//                    std::lock_guard<std::mutex> guard2(mMutex);
//                    if (nullptr == mInstance)
//                    {
//                        mInstance = new DerivedClass;
//                    }
//                }
//
//                return mInstance;
//            }
//
//        protected:
//            ASingleton(void) noexcept = default;
//
//        protected:
//            /** 单例对象 */
//            static std::shared_ptr<DerivedClass> mInstance;
//
//            /** 互斥锁 */
//            static std::mutex mMutex;
//        };
//
//        template<class DerivedClass>
//        std::shared_ptr<DerivedClass> ASingleton<DerivedClass>::mInstance = nullptr;
//
//        template<class DerivedClass>
//        std::mutex ASingleton<DerivedClass>::mMutex;
//    }
//}

#endif // !_APROCH_ASINGLETON_H_