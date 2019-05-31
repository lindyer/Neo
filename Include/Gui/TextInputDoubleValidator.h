#pragma once

/**
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Github:        github/com/lindyer
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/

#include <QDoubleValidator>

namespace Neo {

namespace Gui {

class TextInputDoubleValidator : public QDoubleValidator {
    Q_OBJECT

public:
    TextInputDoubleValidator(QObject* parent = nullptr);
    TextInputDoubleValidator(double bottom, double top, int decimals, QObject* parent = nullptr);

    QValidator::State validate(QString& s, int& pos) const override;
};

}

}

