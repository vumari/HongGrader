#ifndef QUANLYTAIKHOAN_H
#define QUANLYTAIKHOAN_H

#include <QMainWindow>

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
};

#endif // QUANLYTAIKHOAN_H
