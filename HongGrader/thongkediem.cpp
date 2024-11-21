#include "thongkediem.h"
#include "ui_thongkediem.h"

thongkediem::thongkediem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::thongkediem) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

thongkediem::~thongkediem() {
    delete ui;
}
