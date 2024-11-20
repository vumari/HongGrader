#ifndef THONGKEHOCSINH_H
#define THONGKEHOCSINH_H

#include <QMainWindow>

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
};

#endif // THONGKEHOCSINH_H
