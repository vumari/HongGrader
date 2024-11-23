#ifndef QUANLYHOCSINH_H
#define QUANLYHOCSINH_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QSqlTableModel;
class QSqlRelationalTableModel;
class QDataWidgetMapper;
QT_END_NAMESPACE

namespace Ui {
    class quanlyhocsinh;
}

class quanlyhocsinh : public QMainWindow
{
    Q_OBJECT

public:
    explicit quanlyhocsinh(QWidget *parent = nullptr);
    ~quanlyhocsinh();

private:
    Ui::quanlyhocsinh *ui;

    QSqlTableModel *model                       = nullptr;
    QSqlRelationalTableModel *classDetailsModel = nullptr;
    QSqlTableModel *classModel                  = nullptr;

    QDataWidgetMapper *mapper = nullptr;

    bool checkValidInputs();
    void onCurrRowChanged(const QModelIndex &current,
                          const QModelIndex &previous);
    void onAddRow();
    void onEditCurrentRow();
    void onDeleteCurrentRow();
    void onAddClass();
    void onDeleteClass();
    void onSchoolYearChanged(const int &index);
};

#endif // QUANLYHOCSINH_H
