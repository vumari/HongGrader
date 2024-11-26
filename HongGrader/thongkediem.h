#ifndef THONGKEDIEM_H
#define THONGKEDIEM_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
class QSqlQueryModel;
QT_END_NAMESPACE

namespace Ui {
    class thongkediem;
}

class thongkediem : public QMainWindow
{
    Q_OBJECT

public:
    explicit thongkediem(QWidget *parent = nullptr);
    ~thongkediem();

private:
    Ui::thongkediem *ui;

    QSqlDatabase db;
    QSqlQueryModel *model = nullptr;

    void displayPoints();
    void exportFile();
};

#endif // THONGKEDIEM_H
