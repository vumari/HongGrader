#include "thongkediem.h"
#include "ui_thongkediem.h"

#include <QSqlDatabase>

thongkediem::thongkediem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::thongkediem) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSqlDatabase db = QSqlDatabase::cloneDatabase(
        QSqlDatabase::defaultConnection, "thongkediem");

    ui->groupBox_HS->init(db);
}

thongkediem::~thongkediem() {
    delete ui;
}
