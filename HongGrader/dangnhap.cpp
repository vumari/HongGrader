#include "dangnhap.h"
#include "./ui_dangnhap.h"

dangnhap::dangnhap(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::dangnhap)
{
    ui->setupUi(this);
}

dangnhap::~dangnhap()
{
    delete ui;
}

void dangnhap::on_btdangnhap_clicked()
{
    QString username = ui->linetendangnhap->text();
    QString password = ui->linematkhau->text();
    if(username == "123" && password == "123"){
        hide();
        formdiem = new quanlydiem(this);
        formdiem->show();
    }
    else {
        ui->statusbar->showMessage("Tên người dùng và mật khẩu không đúng");
    }
}

