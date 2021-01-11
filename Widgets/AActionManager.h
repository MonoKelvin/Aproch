/****************************************************************************
 * @file    AActionManager.h
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

#include "Framework/ASingleton.h"

namespace aproch
{
    namespace widgets
    {
        /**
         * 动作管理器，用于存储软件所有动作
         */
        class WIDGETS_API AActionManager : public aproch::framework::ASingleton<AActionManager>
        {
            APROCH_DECLARE_SINGLETON(AActionManager);
        public:

            /** 
             * 添加动作
             * @param action 动作
             */
            void addAction(AAction* action);

            /** 
             * 通过Id获取动作
             * @param actionId 动作Id
             * @return AAction* 获取到的动作，如果不存在该Id对应的动作返回nullptr
             */
            AAction* getActionById(const ActionId& actionId);

        private:
            AActionManager();
            ~AActionManager();

        private:
            QList<AAction*> mActions;
        };
    }
}
