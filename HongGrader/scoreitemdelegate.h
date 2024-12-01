#ifndef SCOREITEMDELEGATE_H
#define SCOREITEMDELEGATE_H

#include <QItemEditorCreatorBase>
#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
class QStyle;
QT_END_NAMESPACE

class ScoreSpinBoxEditorCreator : public QItemEditorCreatorBase {
public:
    ScoreSpinBoxEditorCreator();

    QWidget * createWidget(QWidget *parent) const;
    QByteArray valuePropertyName() const;

private:
    QStyle *s = nullptr;
};

class ScoreItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ScoreItemDelegate(QObject *parent = nullptr);

private:
};

#endif // SCOREITEMDELEGATE_H
