#pragma once

/***
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Github:        github/com/lindyer
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/

#include <QObject>

namespace Neo {

    class Common : public QObject
    {
        Q_OBJECT
    public:
        explicit Common(QObject *parent = nullptr);

    signals:

    public slots:
    };

}
