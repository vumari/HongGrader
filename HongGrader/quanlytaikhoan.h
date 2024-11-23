#ifndef QUANLYTAIKHOAN_H
#define QUANLYTAIKHOAN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QSqlTableModel;
class QDataWidgetMapper;
QT_END_NAMESPACE

namespace Ui {
    class quanlytaikhoan;
}

class quanlytaikhoan : public QMainWindow
{
    Q_OBJECT

public:
    explicit quanlytaikhoan(QWidget *parent = nullptr);
    ~quanlytaikhoan();

private:
    Ui::quanlytaikhoan *ui;

    QSqlTableModel *model     = nullptr;
    QDataWidgetMapper *mapper = nullptr;

    bool checkValidInputs(const bool isAddingNew);
    void onAddRow();
    void onEditCurrentRow();
    void onDeleteCurrentRow();
};

#endif // QUANLYTAIKHOAN_H
