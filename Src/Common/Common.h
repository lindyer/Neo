#pragma once

/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-22
 */

#include <QObject>
#include <QJsonDocument>

namespace Neo {

    class Common : public QObject
    {
        Q_OBJECT
    public:
        explicit Common(QObject *parent = nullptr);

    signals:

    public slots:
    };

} // Neo
