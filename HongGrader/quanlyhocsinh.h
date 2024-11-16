#ifndef QUANLYHOCSINH_H
#define QUANLYHOCSINH_H

#include <QMainWindow>

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
};

#endif // QUANLYHOCSINH_H
