#include "quanlylophoc.h"
#include "ui_quanlylophoc.h"

quanlylophoc::quanlylophoc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlylophoc) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

quanlylophoc::~quanlylophoc() {
    delete ui;
}
