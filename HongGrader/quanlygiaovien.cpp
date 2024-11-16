#include "quanlygiaovien.h"
#include "ui_quanlygiaovien.h"

quanlygiaovien::quanlygiaovien(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlygiaovien)
{
    ui->setupUi(this);
}

quanlygiaovien::~quanlygiaovien()
{
    delete ui;
}
