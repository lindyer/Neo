#include "NeoDoubleValidator.h"
#include "../Common/Numeric.h"

namespace neo {

NeoDoubleValidator::NeoDoubleValidator(QObject *parent)
    : QDoubleValidator(parent) {
}


NeoDoubleValidator::NeoDoubleValidator(double bottom, double top, int decimals, QObject* parent)
    : QDoubleValidator(bottom, top, decimals, parent) {
}


QValidator::State NeoDoubleValidator::validate(QString& s, int& pos) const {
    Q_UNUSED(pos)

    if (s.isEmpty() || (s.startsWith("-") && s.length() == 1)) {
        // allow empty field or standalone minus sign
        return QValidator::Intermediate;
    }
    // check length of decimal places
    QChar point = locale().decimalPoint();
    int decimalPointIndex = s.indexOf(point);
    if (decimalPointIndex != -1) {
        int lengthDecimals = s.length() - s.indexOf(point) - 1;
        if (lengthDecimals > decimals()) {
            return QValidator::Invalid;
        }
    }
    // check range of value
    bool isNumber;
    qreal value = locale().toDouble(s, &isNumber);
    if (isNumber) {
        int times = pow(10, decimals());
        qint64 multipleValue = static_cast<qint64>(value * times);
        qint64 multipleBottom = static_cast<qint64>(bottom() * times);
        qint64 multipleTop = static_cast<qint64>(top() * times);
        qint64 valueInteger = static_cast<qint64>(value);
        qint64 bottomInteger = static_cast<qint64>(bottom());
        //qint64 topInteger = static_cast<qint64>(top());
        int valueBase = integerBase(valueInteger);
        int bottomBase = integerBase(bottomInteger);
        if (valueBase < bottomBase) {
            return QValidator::Intermediate;
        }/* else if (valueInteger < bottomInteger || valueInteger > topInteger) {
            return QValidator::Invalid;
        } */else if ((valueInteger == bottomInteger && (decimalPointIndex == s.length() - 1 || decimalPointIndex == -1) || (multipleBottom <= multipleValue && multipleValue <= multipleTop)) ) {
            return QValidator::Acceptable;
        }
    }
    return QValidator::Invalid;
}

} // Neo
