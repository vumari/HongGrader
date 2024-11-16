#include "quanlylophoc.h"
#include "ui_quanlylophoc.h"

quanlylophoc::quanlylophoc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlylophoc)
{
    ui->setupUi(this);
}

quanlylophoc::~quanlylophoc()
{
    delete ui;
}
