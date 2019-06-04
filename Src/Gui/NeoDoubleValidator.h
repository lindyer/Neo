#pragma once

/**
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/

#include <QDoubleValidator>

namespace neo {

namespace Gui {

class NeoDoubleValidator : public QDoubleValidator {
    Q_OBJECT

public:
    NeoDoubleValidator(QObject* parent = nullptr);
    NeoDoubleValidator(double bottom, double top, int decimals, QObject* parent = nullptr);

    QValidator::State validate(QString& s, int& pos) const override;
};

}

}

