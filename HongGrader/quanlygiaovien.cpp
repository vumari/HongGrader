#include "quanlygiaovien.h"
#include "ui_quanlygiaovien.h"

#include "genderitemdelegate.h"
#include "helper.h"

#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QDataWidgetMapper>
#include <QSqlField>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpressionValidator>

quanlygiaovien::quanlygiaovien(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::quanlygiaovien) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSqlDatabase db = QSqlDatabase::database();
    if (db.open()) {
        model = new QSqlRelationalTableModel(this, db);
        model->setTable("GiaoVien");

        // recordToInsert = model->record();
        // Helper::initSqlRecord(recordToInsert, "GiaoVien");

        model->setRelation(7, QSqlRelation("Mon", "MaMon", "TenMon"));
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();
        Helper::setModelColHeaders(model, {
            QString(),
            "Họ và tên", "Ngày sinh", "Giới tính",
            "Địa chỉ", "Số điện thoại", "E-mail", "Môn dạy",
        });

        ui->tablegiaovien->setModel(model);
        ui->tablegiaovien->setItemDelegate(
            new QSqlRelationalDelegate(this));
        ui->tablegiaovien->setItemDelegateForColumn(
            3, new GenderItemDelegate(this));
        ui->tablegiaovien->hideColumn(0);
        ui->tablegiaovien->horizontalHeader()->setSectionResizeMode(
            QHeaderView::ResizeToContents);

        mapper = new QDataWidgetMapper(this);
        mapper->setModel(model);
        mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        mapper->setItemDelegate(
            new GenderItemDelegate(this, ui->RBnam, ui->RBnu));
        mapper->addMapping(ui->LEtenGV, 1);
        mapper->addMapping(ui->DEngaysinh, 2);
        mapper->addMapping(ui->RBnu, 3);
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

    QRegularExpression regex;
    regex.setPattern(
        R"((?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\]))");
    ui->LEemail->setValidator(new QRegularExpressionValidator(regex, this));
}

quanlygiaovien::~quanlygiaovien() {
    delete ui;
}

bool quanlygiaovien::checkValidInputs() {
    if (ui->LEtenGV->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập họ và tên.");
        ui->LEtenGV->setFocus(Qt::OtherFocusReason);
        return false;
    }
    if (ui->LEdiachi->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập địa chỉ.");
        ui->LEdiachi->setFocus(Qt::OtherFocusReason);
        return false;
    }
    if (ui->LEdienthoai->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập số điện thoại");
        ui->LEdienthoai->setFocus(Qt::OtherFocusReason);
        return false;
    } else if (!ui->LEdienthoai->hasAcceptableInput()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Số điện thoại không hợp lệ");
        ui->LEdienthoai->setFocus(Qt::OtherFocusReason);
        return false;
    }
    if (ui->LEemail->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập địa chỉ e-mail.");
        ui->LEemail->setFocus(Qt::OtherFocusReason);
        return false;
    } else if (!ui->LEemail->hasAcceptableInput()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Địa chỉ e-mail không hợp lệ.");
        ui->LEemail->setFocus(Qt::OtherFocusReason);
        return false;
    }
    if (ui->CBmon->currentIndex() == -1) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng chọn môn dạy.");
        ui->CBmon->setFocus(Qt::OtherFocusReason);
    }
    return true;
}

void quanlygiaovien::onAddRow() {
    if (!checkValidInputs()) {
        return;
    }

    QSqlRecord newRecord = model->record();
    newRecord.setGenerated("MaGV", false);
    newRecord.setValue(1, ui->LEtenGV->text().trimmed());
    newRecord.setValue(2, ui->DEngaysinh->date());
    newRecord.setValue(3, ui->RBnu->isChecked());
    newRecord.setValue(4, ui->LEdiachi->text().trimmed());
    newRecord.setValue(5, ui->LEdienthoai->text().trimmed());
    newRecord.setValue(6, ui->LEemail->text().trimmed());
    newRecord.setValue(7, Helper::getCurrIdFromComboBox(ui->CBmon));
    if (!model->insertRecord(-1, newRecord) || !model->submitAll()) {
        QMessageBox::critical(this, "Lỗi thêm dòng", model->lastError().text());
        model->revertAll();
    }
}

void quanlygiaovien::onEditCurrentRow() {
    if (ui->tablegiaovien->selectionModel()->hasSelection()) {
        if (!checkValidInputs()) {
            return;
        }

        if (!mapper->submit() || !model->submitAll()) {
            QMessageBox::critical(this,
                                  "Lỗi sửa dòng",
                                  model->lastError().text());
            model->revertAll();
        }
    }
}

void quanlygiaovien::onDeleteCurrentRow() {
    const auto *selectionModel = ui->tablegiaovien->selectionModel();

    if (selectionModel->hasSelection()) {
        if (Helper::ifValueExistsInTable(
                model->database(), "Lop"_L1, "MaGVCN"_L1,
                selectionModel->currentIndex().siblingAtColumn(0).data(),
                this)) {
            QMessageBox::critical(this,
                                  "Lỗi xoá giáo viên",
                                  "Không thể xoá giáo viên do giáo viên này làm chủ nhiệm của ít nhất một lớp.");
            return;
        }
        Helper::tryDeleteCurrentRow(model, ui->tablegiaovien);
    }
}
