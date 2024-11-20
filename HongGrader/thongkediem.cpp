#include "thongkediem.h"
#include "ui_thongkediem.h"

thongkediem::thongkediem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::thongkediem)
{
    ui->setupUi(this);
}

thongkediem::~thongkediem()
{
    delete ui;
}
