#ifndef THONGKEHOCSINH_H
#define THONGKEHOCSINH_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QSqlQueryModel;
QT_END_NAMESPACE

namespace Ui {
    class thongkehocsinh;
}

class thongkehocsinh : public QMainWindow
{
    Q_OBJECT

public:
    explicit thongkehocsinh(QWidget *parent = nullptr);
    ~thongkehocsinh();

private:
    Ui::thongkehocsinh *ui;

    QSqlQueryModel *subjectModel    = nullptr;
    QSqlQueryModel *schoolYearModel = nullptr;
    QSqlQueryModel *model           = nullptr;

    void displayPoints();
    void exportFile();
};

#endif // THONGKEHOCSINH_H
