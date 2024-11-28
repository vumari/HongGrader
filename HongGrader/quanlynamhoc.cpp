#include "quanlynamhoc.h"
#include "ui_quanlynamhoc.h"

quanlynamhoc::quanlynamhoc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlynamhoc)
{
    ui->setupUi(this);
}

quanlynamhoc::~quanlynamhoc()
{
    delete ui;
}
