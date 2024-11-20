#include "thongkehocsinh.h"
#include "ui_thongkehocsinh.h"

thongkehocsinh::thongkehocsinh(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::thongkehocsinh)
{
    ui->setupUi(this);
}

thongkehocsinh::~thongkehocsinh()
{
    delete ui;
}
