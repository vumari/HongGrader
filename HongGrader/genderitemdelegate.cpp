#include "genderitemdelegate.h"

#include <QRadioButton>

GenderItemDelegate::GenderItemDelegate(QObject *parent)
    : QSqlRelationalDelegate{parent} {
}

GenderItemDelegate::GenderItemDelegate(QObject *parent,
                                       QRadioButton *maleBtn,
                                       QRadioButton *femaleBtn)
    : QSqlRelationalDelegate{parent}, maleButton{maleBtn},
    femaleButton{femaleBtn} {
}

QString GenderItemDelegate::displayText(const QVariant &value,
                                        const QLocale &locale) const {
    if (value.typeId() == QMetaType::UInt) {
        return (value.toBool()) ? "Nữ" : "Nam";
    }
    return QSqlRelationalDelegate::displayText(value, locale);
}

void GenderItemDelegate::setModelData(QWidget *editor,
                                      QAbstractItemModel *model,
                                      const QModelIndex &index) const {
    if (auto *radioButton = qobject_cast<QRadioButton *>(editor)) {
        if ((radioButton == maleButton) || (radioButton == femaleButton)) {
            model->setData(index, femaleButton->isChecked());
        }
    } else {
        QSqlRelationalDelegate::setModelData(editor, model, index);
    }
}

void GenderItemDelegate::setEditorData(QWidget *editor,
                                       const QModelIndex &index) const {
    if (qobject_cast<QRadioButton *>(editor)) {
        if (index.data().toBool()) {
            femaleButton->setChecked(true);
        } else {
            maleButton->setChecked(true);
        }
    } else {
        QSqlRelationalDelegate::setEditorData(editor, index);
    }
}
