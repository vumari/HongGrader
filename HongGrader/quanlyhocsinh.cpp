#include "quanlyhocsinh.h"
#include "ui_quanlyhocsinh.h"

quanlyhocsinh::quanlyhocsinh(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlyhocsinh)
{
    ui->setupUi(this);
}

quanlyhocsinh::~quanlyhocsinh()
{
    delete ui;
}
