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
    const QString &&schoolYear = ui->LEnamhocmoi->text().trimmed();

    if (schoolYear.isEmpty()) {
        QMessageBox::critical(this, "Lỗi thêm năm học",
                              "Vui lòng nhập năm học.");
        return;
    }
    if (Helper::ifSchoolYearExists(model->database(), schoolYear, this)) {
        QMessageBox::critical(this,
                              "Lỗi thêm năm học",
                              "Năm học này đã tồn tại, nên không thể thêm được.");
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
    Helper::tryDeleteCurrentRow(model, ui->tablenamhoc);
}
