#include "quanlydiem.h"
#include "ui_quanlydiem.h"

#include "dangnhap.h"

#include <QTimer>
#include <QSqlDatabase>

quanlydiem::quanlydiem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlydiem) {
    ui->setupUi(this);
    ui->BThuy->hide();
    ui->BTluu->hide();

    if (!QSqlDatabase::contains("main")) {
        QSqlDatabase::cloneDatabase(QSqlDatabase::defaultConnection, "main");
    }

    QSqlDatabase db = QSqlDatabase::database("main");
    ui->groupBox_HS->init(db);

    QTimer::singleShot(1, this, &quanlydiem::login);
}

quanlydiem::~quanlydiem() {
    delete ui;
}

void quanlydiem::login() {
    dangnhap formDangNhap(this);
    int      result = formDangNhap.exec();

    if (result == QDialog::Accepted) {
        username = formDangNhap.username;
    } else {
        qApp->quit();
    }
}

void quanlydiem::on_actionLog_out_triggered() {
    login();
}


void quanlydiem::on_actiongiaovien_triggered() {
    (new quanlygiaovien(this))->show();
}


void quanlydiem::on_actionhocsinh_triggered() {
    (new quanlyhocsinh(this))->show();
}


void quanlydiem::on_actionlop_triggered() {
    (new quanlylophoc(this))->show();
}


void quanlydiem::on_actionEdit_triggered() {
    (new quanlytaikhoan(this))->show();
}


void quanlydiem::on_actionscoreboard_triggered() {
    (new thongkediem(this))->show();
}


void quanlydiem::on_actionstudentlist_triggered() {
    (new thongkehocsinh(this))->show();
}


void quanlydiem::on_BTsua_clicked() {
    ui->BTsua->hide();
    ui->BThuy->show();
    ui->BTluu->show();
    ui->BTthem->setDisabled(true);
    ui->Btxoa->setDisabled(true);
}


void quanlydiem::on_BThuy_clicked() {
    ui->BThuy->hide();
    ui->BTluu->hide();
    ui->BTsua->show();
    ui->BTthem->setDisabled(false);
    ui->Btxoa->setDisabled(false);
}


void quanlydiem::on_BTluu_clicked() {
    ui->BTluu->hide();
    ui->BThuy->hide();
    ui->BTsua->show();
    ui->BTthem->setDisabled(false);
    ui->Btxoa->setDisabled(false);
}

