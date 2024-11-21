#include "quanlydiem.h"
#include "ui_quanlydiem.h"

#include "dangnhap.h"

#include <QTimer>

quanlydiem::quanlydiem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlydiem) {
    ui->setupUi(this);
    // setAttribute(Qt::WA_DeleteOnClose, true);

    QTimer::singleShot(0, this, &quanlydiem::login);
}

quanlydiem::~quanlydiem() {
    delete ui;
}

void quanlydiem::login() {
    dangnhap formDangNhap(this);
    int      result = formDangNhap.exec();

    if (result == QDialog::Accepted) {
        username = formDangNhap.username;
    } else {
        qApp->quit();
    }
}

void quanlydiem::on_actionLog_out_triggered() {
    login();
}


void quanlydiem::on_actiongiaovien_triggered() {
    (new quanlygiaovien(this))->show();
}


void quanlydiem::on_actionhocsinh_triggered() {
    (new quanlyhocsinh(this))->show();
}


void quanlydiem::on_actionlop_triggered() {
    (new quanlylophoc(this))->show();
}


void quanlydiem::on_actionEdit_triggered() {
    (new quanlytaikhoan(this))->show();
}


void quanlydiem::on_actionscoreboard_triggered() {
    (new thongkediem(this))->show();
}


void quanlydiem::on_actionstudentlist_triggered() {
    (new thongkehocsinh(this))->show();
}

