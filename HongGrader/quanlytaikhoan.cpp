#include "quanlytaikhoan.h"
#include "ui_quanlytaikhoan.h"

#include "helper.h"

#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlField>
#include <QSqlError>
#include <QMessageBox>

quanlytaikhoan::quanlytaikhoan(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlytaikhoan) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSqlDatabase db = QSqlDatabase::database();
    if (db.open()) {
        model = new QSqlTableModel(this, db);
        model->setTable("TaiKhoan");
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();
        Helper::setModelColHeaders(model, {
            "Tên đăng nhập", "Mật khẩu"
        });

        ui->tabletaikhoan->setModel(model);
        ui->tabletaikhoan->horizontalHeader()->setSectionResizeMode(
            QHeaderView::ResizeToContents);

        mapper = new QDataWidgetMapper(this);
        mapper->setModel(model);
        mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        mapper->addMapping(ui->LEtentaikhoan, 0);
        mapper->addMapping(ui->LEmatkhau, 1);

        connect(ui->tabletaikhoan->selectionModel(),
                &QItemSelectionModel::currentRowChanged,
                mapper,
                &QDataWidgetMapper::setCurrentModelIndex);
    }

    connect(ui->BTthem, &QPushButton::clicked, this, &quanlytaikhoan::onAddRow);
    connect(ui->BTsua, &QPushButton::clicked,
            this, &quanlytaikhoan::onEditCurrentRow);
    connect(ui->BTxoa, &QPushButton::clicked,
            this, &quanlytaikhoan::onDeleteCurrentRow);
}

quanlytaikhoan::~quanlytaikhoan() {
    delete ui;
}

bool quanlytaikhoan::checkValidInputs(const bool isAddingNew) {
    const QString &&uname = ui->LEtentaikhoan->text().trimmed();

    if (uname.isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập tên tài khoản.");
        return false;
    }

    if (Helper::ifUsernameExists(model->database(), uname, this)) {
        const auto *selectionModel = ui->tabletaikhoan->selectionModel();

        bool changingUserName = true;
        if (!isAddingNew && selectionModel->hasSelection()) {
            const QString &&currUname =
                model->data(model->index(
                                selectionModel->currentIndex().row(),
                                0)).toString();

            if (currUname == uname) {
                changingUserName = false;
            }
        }

        if (changingUserName) {
            QMessageBox::critical(this, "Lỗi nhập liệu",
                                  "Tên người dùng đã tồn tại.");
            return false;
        }
    }

    if (ui->LEmatkhau->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập mật khẩu.");
        return false;
    }
    return true;
}

void quanlytaikhoan::onAddRow() {
    if (!checkValidInputs(true)) {
        return;
    }

    QSqlRecord newRecord = model->record();
    newRecord.setValue(0, ui->LEtentaikhoan->text().trimmed());
    newRecord.setValue(1, ui->LEmatkhau->text().trimmed());
    if (!model->insertRecord(-1, newRecord) || !model->submitAll()) {
        QMessageBox::critical(this, "Lỗi thêm dòng", model->lastError().text());
        model->revertAll();
    }
}

void quanlytaikhoan::onEditCurrentRow() {
    if (ui->tabletaikhoan->selectionModel()->hasSelection()) {
        if (!checkValidInputs(false)) {
            return;
        }

        if (!mapper->submit() || !model->submitAll()) {
            QMessageBox::critical(this, "Lỗi sửa dòng",
                                  model->lastError().text());
            model->revertAll();
        }
    }
}

void quanlytaikhoan::onDeleteCurrentRow() {
    Helper::tryDeleteCurrentRow(model, ui->tabletaikhoan);
}
