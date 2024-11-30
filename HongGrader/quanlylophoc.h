#ifndef QUANLYLOPHOC_H
#define QUANLYLOPHOC_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QSqlRelationalTableModel;
class QDataWidgetMapper;
QT_END_NAMESPACE

namespace Ui {
    class quanlylophoc;
}

class quanlylophoc : public QMainWindow
{
    Q_OBJECT

public:
    explicit quanlylophoc(QWidget *parent = nullptr);
    ~quanlylophoc();

private:
    Ui::quanlylophoc *ui;

    QSqlRelationalTableModel *model = nullptr;
    QDataWidgetMapper *mapper       = nullptr;

    bool checkValidInputs();
    void onAddRow();
    void onEditCurrentRow();
    void onDeleteCurrentRow();
};

#endif // QUANLYLOPHOC_H
