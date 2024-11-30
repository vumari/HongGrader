#ifndef BOOLITEMDELEGATE_H
#define BOOLITEMDELEGATE_H

#include <QStyledItemDelegate>

class BoolItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit BoolItemDelegate(QObject *parent,
                              QString &&trueTxt,
                              QString &&falseTxt);

    virtual QString displayText(const QVariant &value,
                                const QLocale &locale) const final;

private:
    QString trueText  = "Đúng";
    QString falseText = "Sai";
};

#endif // BOOLITEMDELEGATE_H
