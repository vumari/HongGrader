#include "quanlygiaovien.h"
#include "ui_quanlygiaovien.h"

#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QDataWidgetMapper>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>

quanlygiaovien::quanlygiaovien(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::quanlygiaovien) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSqlDatabase db = QSqlDatabase::database();
    if (db.open()) {
        model = new QSqlRelationalTableModel(this, db);
        model->setTable("GiaoVien");
        model->setRelation(7, QSqlRelation("Mon", "MaMon", "TenMon"));
        model->setEditStrategy(QSqlTableModel::OnRowChange);
        model->select();
        model->setHeaderData(1, Qt::Horizontal, "Tên");
        model->setHeaderData(2, Qt::Horizontal, "Ngày sinh");
        model->setHeaderData(3, Qt::Horizontal, "Giới tính");
        model->setHeaderData(4, Qt::Horizontal, "Địa chỉ");
        model->setHeaderData(5, Qt::Horizontal, "Số điện thoại");
        model->setHeaderData(6, Qt::Horizontal, "E-mail");
        model->setHeaderData(7, Qt::Horizontal, "Môn dạy");

        ui->tablegiaovien->setModel(model);
        ui->tablegiaovien->setItemDelegate(
            new QSqlRelationalDelegate(ui->tablegiaovien));
        ui->tablegiaovien->hideColumn(0);

        mapper = new QDataWidgetMapper;
        mapper->setModel(model);
        mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        mapper->setItemDelegate(new QSqlRelationalDelegate(this));
        mapper->addMapping(ui->LEtenGV, 1);
        mapper->addMapping(ui->DEngaysinh, 2);
        mapper->addMapping(ui->LEdiachi, 4);
        mapper->addMapping(ui->LEdienthoai, 5);
        mapper->addMapping(ui->LEemail, 6);
        mapper->addMapping(ui->CBmon, 7);

        QSqlTableModel *relModel = model->relationModel(7);
        ui->CBmon->setModel(relModel);
        ui->CBmon->setModelColumn(relModel->fieldIndex("TenMon"));

        connect(ui->tablegiaovien->selectionModel(),
                &QItemSelectionModel::currentRowChanged,
                mapper,
                &QDataWidgetMapper::setCurrentModelIndex);
    }

    connect(ui->BTthem, &QPushButton::clicked, this, &quanlygiaovien::onAddRow);
    connect(ui->BTsua, &QPushButton::clicked,
            this, &quanlygiaovien::onEditCurrentRow);
    connect(ui->BTxoa, &QPushButton::clicked,
            this, &quanlygiaovien::onDeleteCurrentRow);
}

quanlygiaovien::~quanlygiaovien() {
    delete ui;
}

bool quanlygiaovien::checkValidInputs() {
    if (ui->LEtenGV->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập họ và tên.");
        return false;
    }
    if (ui->LEdiachi->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập địa chỉ.");
        return false;
    }
    if (ui->LEdienthoai->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập số điện thoại");
        return false;
    }
    if (ui->LEemail->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập địa chỉ e-mail.");
        return false;
    }
    return true;
}

void quanlygiaovien::onAddRow() {
    if (!checkValidInputs()) {
        return;
    }

    QSqlRecord newRecord = model->record();
    newRecord.setGenerated("MaGV", false);
    // newRecord.setGenerated("TenGV", false);
    // newRecord.setGenerated("NgaySinh", false);
    // newRecord.setGenerated("GioiTinh", false);
    // newRecord.setGenerated("DiaChi", false);
    // newRecord.setGenerated("SDT", false);
    // newRecord.setGenerated("Email", false);
    // newRecord.setGenerated("MaMon", false);
    newRecord.setValue(1, ui->LEtenGV->text().trimmed());
    newRecord.setValue(2, ui->DEngaysinh->date());
    QString gender = ui->RBnam->isChecked() ? "Nam" : "Nữ";
    newRecord.setValue(3, gender);
    newRecord.setValue(4, ui->LEdiachi->text().trimmed());
    newRecord.setValue(5, ui->LEdienthoai->text().trimmed());
    newRecord.setValue(6, ui->LEemail->text().trimmed());
    newRecord.setValue(7, ui->CBmon->currentIndex());
    if (!model->insertRecord(-1, newRecord)) {
        QMessageBox::critical(this, "Loi them dong", model->lastError().text());
    }
}

void quanlygiaovien::onEditCurrentRow() {
    if (ui->tablegiaovien->selectionModel()->hasSelection()) {
        if (!checkValidInputs()) {
            return;
        }

        mapper->submit();
    }
}

void quanlygiaovien::onDeleteCurrentRow() {
    const auto *selectionModel = ui->tablegiaovien->selectionModel();

    if (selectionModel->hasSelection()) {
        if (!model->removeRow(selectionModel->currentIndex().row())) {
            QMessageBox::critical(this,
                                  "Loi xofa dong",
                                  model->lastError().text());
        }
    }
}
