#include "scoreitemdelegate.h"

#include <QDoubleSpinBox>
#include <QStyleFactory>
#include <QItemEditorFactory>

ScoreSpinBoxEditorCreator::ScoreSpinBoxEditorCreator() {
    s = QStyleFactory::create("windowsvista");
}

QWidget * ScoreSpinBoxEditorCreator::createWidget(QWidget *parent) const {
    auto *spinBox = new QDoubleSpinBox(parent);

    spinBox->setStyle(s);
    spinBox->setStyleSheet("padding: 0px;");
    spinBox->setRange(0.0, 10.0);
    spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBox->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    spinBox->setFrame(false);

    return spinBox;
}

QByteArray ScoreSpinBoxEditorCreator::valuePropertyName() const {
    const static QByteArray prop{ "value" };

    return prop;
}


ScoreItemDelegate::ScoreItemDelegate(QObject *parent)
    : QStyledItemDelegate{parent} {
    auto *factory =
        new QItemEditorFactory(*QItemEditorFactory::defaultFactory());

    factory->registerEditor(QMetaType::Double, new ScoreSpinBoxEditorCreator());
    setItemEditorFactory(factory);
}
