#include "quanlynamhoc.h"
#include "ui_quanlynamhoc.h"

#include "helper.h"
#include "boolitemdelegate.h"

#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlField>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

quanlynamhoc::quanlynamhoc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlynamhoc) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSqlDatabase db = QSqlDatabase::database();
    if (db.open()) {
        model = new QSqlTableModel(this, db);
        model->setTable("NamHoc");
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();
        Helper::setModelColHeaders(model, { "Tên năm học", "Trạng thái" });

        ui->tablenamhoc->setModel(model);
        ui->tablenamhoc->horizontalHeader()->setSectionResizeMode(
            QHeaderView::ResizeToContents);
        ui->tablenamhoc->setItemDelegateForColumn(
            1, new BoolItemDelegate(this, u"Đã hoàn thành"_s,
                                    u"Chưa hoàn thành"_s));

        mapper = new QDataWidgetMapper(this);
        mapper->setModel(model);
        mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        mapper->addMapping(ui->LEnamhocmoi, 0);
        mapper->addMapping(ui->checkBoxvohieu, 1);

        connect(ui->tablenamhoc->selectionModel(),
                &QItemSelectionModel::currentRowChanged,
                mapper,
                &QDataWidgetMapper::setCurrentModelIndex);
    }

    connect(ui->BTthemnamhoc, &QPushButton::clicked,
            this, &quanlynamhoc::onAddSchoolYear);
    connect(ui->BTsua, &QPushButton::clicked,
            this, &quanlynamhoc::onEditCurrentRow);
    connect(ui->BTxoanamhoc, &QPushButton::clicked,
            this, &quanlynamhoc::onDeleteSchoolYear);
}

quanlynamhoc::~quanlynamhoc() {
    delete ui;
}

bool quanlynamhoc::checkValidInputs() {
    if (ui->LEnamhocmoi->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập năm học.");
        ui->LEnamhocmoi->setFocus(Qt::OtherFocusReason);
        return false;
    }
    return true;
}

void quanlynamhoc::onEditCurrentRow() {
    if (ui->tablenamhoc->selectionModel()->hasSelection()) {
        if (!checkValidInputs()) {
            return;
        }

        const QString &&schoolYear =
            ui->tablenamhoc->selectionModel()->currentIndex().siblingAtColumn(0)
            .data().toString();
        const QString &&newSchoolYear = ui->LEnamhocmoi->text().trimmed();

        if (schoolYear != newSchoolYear) {
            if (Helper::ifSchoolYearExists(model->database(), newSchoolYear,
                                           this)) {
                QMessageBox::critical(this,
                                      "Lỗi sửa năm học",
                                      "Năm học này đã tồn tại, nên không thể sửa được.");
                ui->LEnamhocmoi->setFocus(Qt::OtherFocusReason);
                return;
            }
        }

        if (!mapper->submit() || !model->submitAll()) {
            QMessageBox::critical(this,
                                  "Lỗi sửa dòng",
                                  model->lastError().text());
            model->revertAll();
        }
    }
}

void quanlynamhoc::onAddSchoolYear() {
    if (!checkValidInputs()) {
        return;
    }

    const QString &&schoolYear = ui->LEnamhocmoi->text().trimmed();
    if (Helper::ifSchoolYearExists(model->database(), schoolYear, this)) {
        QMessageBox::critical(this,
                              "Lỗi thêm năm học",
                              "Năm học này đã tồn tại, nên không thể thêm được.");
        ui->LEnamhocmoi->setFocus(Qt::OtherFocusReason);
        return;
    }

    QSqlRecord newRecord = model->record();
    newRecord.setValue(0, ui->LEnamhocmoi->text().trimmed());
    newRecord.setValue(1, ui->checkBoxvohieu->isChecked());
    if (!model->insertRecord(-1, newRecord) || !model->submitAll()) {
        QMessageBox::critical(this,
                              "Lỗi thêm năm học",
                              model->lastError().text());
        model->revertAll();
    }
}

void quanlynamhoc::onDeleteSchoolYear() {
    const auto *selectionModel = ui->tablenamhoc->selectionModel();

    if (selectionModel->hasSelection()) {
        if (Helper::ifValueExistsInTable(
                model->database(), "Lop"_L1, "TenNamHoc"_L1,
                selectionModel->currentIndex().siblingAtColumn(0).data(),
                this)) {
            QMessageBox::critical(this,
                                  "Lỗi xoá năm học",
                                  "Không thể xoá năm học do có lớp liên kết với năm học này.");
            return;
        }
        if (Helper::ifValueExistsInTable(
                model->database(), "DiemTongHop"_L1, "TenNamHoc"_L1,
                selectionModel->currentIndex().siblingAtColumn(0).data(),
                this)) {
            QMessageBox::critical(this,
                                  "Lỗi xoá năm học",
                                  "Không thể xoá năm học do có điểm liên kết với năm học này.");
            return;
        }
        Helper::tryDeleteCurrentRow(model, ui->tablenamhoc);
    }
}
