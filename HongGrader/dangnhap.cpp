#include "dangnhap.h"
#include "./ui_dangnhap.h"

#include <QMessageBox>

dangnhap::dangnhap(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dangnhap) {
    ui->setupUi(this);
}

dangnhap::~dangnhap() {
    delete ui;
}

void dangnhap::on_btdangnhap_clicked() {
    QString &&uname = ui->linetendangnhap->text();
    QString &&pass  = ui->linematkhau->text();

    if (uname == "123" && pass == "123") {
        username = uname;
        accept();
    } else {
        QMessageBox::critical(this,
                              "Lỗi đăng nhập",
                              "Tên người dùng và mật khẩu không đúng");
    }
}

