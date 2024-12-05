#ifndef QUANLYHOCSINH_H
#define QUANLYHOCSINH_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QSqlTableModel;
class QSqlQueryModel;
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

    QSqlTableModel *model             = nullptr;
    QSqlQueryModel *classDetailsModel = nullptr;
    QSqlTableModel *classModel        = nullptr;
    QSqlTableModel *classModel2       = nullptr;

    QDataWidgetMapper *mapper = nullptr;

    bool checkValidInputs();
    void loadClassDetails();
    void onCurrRowChanged(const QModelIndex &current,
                          const QModelIndex &previous);
    void onAddRow();
    void onEditCurrentRow();
    void onDeleteCurrentRow();
    void onAddClass();
    void onDeleteClass();
    void onSchoolYearChanged(const int &index);
    void onSchoolYearFilterChanged(const int &index);
    void onFilter();
};

#endif // QUANLYHOCSINH_H
