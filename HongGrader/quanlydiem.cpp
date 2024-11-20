#include "quanlydiem.h"
#include "dangnhap.h"
#include "ui_quanlydiem.h"

quanlydiem::quanlydiem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlydiem) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

quanlydiem::~quanlydiem() {
    delete ui;
}

void quanlydiem::on_actionLog_out_triggered() {
    hide();
    formlogin = new dangnhap(this);
    formlogin->show();
}


void quanlydiem::on_actiongiaovien_triggered() {
    formgiaovien = new quanlygiaovien(this);
    formgiaovien->show();
}


void quanlydiem::on_actionhocsinh_triggered() {
    formhocsinh = new quanlyhocsinh(this);
    formhocsinh->show();
}


void quanlydiem::on_actionlop_triggered() {
    formlophoc = new quanlylophoc(this);
    formlophoc->show();
}


void quanlydiem::on_actionEdit_triggered() {
    formtaikhoan = new quanlytaikhoan(this);
    formtaikhoan->show();
}


void quanlydiem::on_actionscoreboard_triggered() {
    formtkdiem = new thongkediem(this);
    formtkdiem->show();
}


void quanlydiem::on_actionstudentlist_triggered() {
    formtkhocsinh = new thongkehocsinh(this);
    formtkhocsinh->show();
}

