#ifndef QUANLYDIEM_H
#define QUANLYDIEM_H

#include <QMainWindow>
#include "quanlygiaovien.h"
#include "quanlyhocsinh.h"
#include "quanlylophoc.h"

namespace Ui {
class quanlydiem;
}
class dangnhap;
class quanlydiem : public QMainWindow
{
    Q_OBJECT

public:
    explicit quanlydiem(QWidget *parent = nullptr);
    ~quanlydiem();

private slots:
    void on_actionLog_out_triggered();

    void on_actiongiaovien_triggered();

    void on_actionhocsinh_triggered();

    void on_actionlop_triggered();

private:
    Ui::quanlydiem *ui;
    dangnhap *formlogin;
    quanlygiaovien *formgiaovien;
    quanlyhocsinh *formhocsinh;
    quanlylophoc *formlophoc;
};

#endif // QUANLYDIEM_H
