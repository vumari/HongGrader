#include "chuyenlop.h"
#include "ui_chuyenlop.h"

chuyenlop::chuyenlop(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::chuyenlop)
{
    ui->setupUi(this);
}

chuyenlop::~chuyenlop()
{
    delete ui;
}
