#ifndef QUANLYDIEM_H
#define QUANLYDIEM_H

#include <QMainWindow>
#include "quanlygiaovien.h"
#include "quanlyhocsinh.h"
#include "quanlylophoc.h"
#include "quanlytaikhoan.h"
#include "thongkediem.h"
#include "thongkehocsinh.h"

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
    void login();

    void on_actionLog_out_triggered();
    void on_actiongiaovien_triggered();
    void on_actionhocsinh_triggered();
    void on_actionlop_triggered();
    void on_actionEdit_triggered();
    void on_actionscoreboard_triggered();
    void on_actionstudentlist_triggered();

    void on_BTsua_clicked();

    void on_BThuy_clicked();

    void on_BTluu_clicked();

private:
    Ui::quanlydiem *ui;

    QString username;
};

#endif // QUANLYDIEM_H
