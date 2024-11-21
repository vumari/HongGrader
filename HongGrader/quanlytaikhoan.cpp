#include "quanlytaikhoan.h"
#include "ui_quanlytaikhoan.h"

quanlytaikhoan::quanlytaikhoan(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlytaikhoan) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

quanlytaikhoan::~quanlytaikhoan() {
    delete ui;
}
