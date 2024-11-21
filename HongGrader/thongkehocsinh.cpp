#include "thongkehocsinh.h"
#include "ui_thongkehocsinh.h"

thongkehocsinh::thongkehocsinh(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::thongkehocsinh) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

thongkehocsinh::~thongkehocsinh() {
    delete ui;
}
