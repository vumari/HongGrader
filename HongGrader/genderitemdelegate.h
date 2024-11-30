#ifndef GENDERITEMDELEGATE_H
#define GENDERITEMDELEGATE_H

#include <QSqlRelationalDelegate>

QT_BEGIN_NAMESPACE
class QRadioButton;
QT_END_NAMESPACE

class GenderItemDelegate : public QSqlRelationalDelegate
{
    Q_OBJECT
public:
    GenderItemDelegate(QObject *parent = nullptr);
    GenderItemDelegate(QObject *parent,
                       QRadioButton *maleBtn,
                       QRadioButton *femaleBtn);

    virtual QString displayText(const QVariant &value,
                                const QLocale &locale) const final;

    virtual void setModelData(QWidget *editor,
                              QAbstractItemModel *model,
                              const QModelIndex &index) const final;

    virtual void setEditorData(QWidget *editor,
                               const QModelIndex &index) const final;

    QRadioButton *maleButton   = nullptr;
    QRadioButton *femaleButton = nullptr;
};

#endif // GENDERITEMDELEGATE_H
