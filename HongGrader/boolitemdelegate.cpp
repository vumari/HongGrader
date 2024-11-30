#include "boolitemdelegate.h"

BoolItemDelegate::BoolItemDelegate(QObject *parent,
                                   QString &&trueTxt,
                                   QString &&falseTxt)
    : QStyledItemDelegate{parent}, trueText{trueTxt}, falseText{falseTxt} {
}

QString BoolItemDelegate::displayText(const QVariant &value,
                                      const QLocale &locale) const {
    if (value.canConvert<bool>()) {
        return (value.toBool()) ? trueText : falseText;
    }
    return QStyledItemDelegate::displayText(value, locale);
}
