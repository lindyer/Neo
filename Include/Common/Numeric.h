﻿#pragma once

/**
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Github:        github/com/lindyer
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/


#include <QtCore/QObject>
#include <QtCore/qglobal.h>

namespace Neo {

    class Numeric : public QObject
    {
        Q_OBJECT
    public:
        explicit Numeric(QObject *parent = nullptr);

    signals:

    public slots:
    };

    /**
     * 获取整数的位数
     */
    template<typename T>
    int integerBase(T value) {
        static_assert(std::numeric_limits<T>::is_integer,"Type should be integer");
        int base = 0;
        while (value != 0) {
            value /= 10;
            base++;
        }
        return base;
    }
}
