#include "quanlylophoc.h"
#include "ui_quanlylophoc.h"

#include "helper.h"

#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QDataWidgetMapper>
#include <QSqlField>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

quanlylophoc::quanlylophoc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlylophoc) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSqlDatabase db = QSqlDatabase::database();
    if (db.open()) {
        model = new QSqlRelationalTableModel(this, db);
        model->setTable("Lop");

        model->setRelation(2, QSqlRelation("GiaoVien", "MaGV", "TenGV"));
        model->setRelation(3, QSqlRelation("NamHoc", "TenNamHoc", "TenNamHoc"));
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();
        Helper::setModelColHeaders(model, {
            "Mã", "Tên lớp", "GVCN", "Năm học",
        });

        ui->tablelophoc->setModel(model);
        ui->tablelophoc->setItemDelegate(
            new QSqlRelationalDelegate(this));
        ui->tablelophoc->horizontalHeader()->setSectionResizeMode(
            QHeaderView::ResizeToContents);

        mapper = new QDataWidgetMapper(this);
        mapper->setModel(model);
        mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        mapper->setItemDelegate(
            new QSqlRelationalDelegate(this));
        mapper->addMapping(ui->LEten, 1);
        mapper->addMapping(ui->CBgvcn, 2);
        mapper->addMapping(ui->CBnamhoc, 3);

        QSqlTableModel *relModel = model->relationModel(2);
        ui->CBgvcn->setModel(relModel);
        ui->CBgvcn->setModelColumn(relModel->fieldIndex("TenGV"));
        relModel = model->relationModel(3);
        ui->CBnamhoc->setModel(relModel);

        connect(ui->tablelophoc->selectionModel(),
                &QItemSelectionModel::currentRowChanged,
                mapper,
                &QDataWidgetMapper::setCurrentModelIndex);
    }

    connect(ui->BTthem, &QPushButton::clicked, this, &quanlylophoc::onAddRow);
    connect(ui->BTsua, &QPushButton::clicked,
            this, &quanlylophoc::onEditCurrentRow);
    connect(ui->BTxoa, &QPushButton::clicked,
            this, &quanlylophoc::onDeleteCurrentRow);
    connect(ui->BTthemnamhoc, &QPushButton::clicked,
            this, &quanlylophoc::onAddSchoolYear);
    connect(ui->BTxoanamhoc, &QPushButton::clicked,
            this, &quanlylophoc::onDeleteSchoolYear);
}

quanlylophoc::~quanlylophoc() {
    delete ui;
}

bool quanlylophoc::checkValidInputs() {
    if (ui->LEten->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập tên.");
        return false;
    }
    if (ui->CBgvcn->currentIndex() == -1) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng chọn giáo viên chủ nhiệm.");
        return false;
    }
    if (ui->CBnamhoc->currentIndex() == -1) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng chọn năm học.");
        return false;
    }
    return true;
}

void quanlylophoc::onAddRow() {
    if (!checkValidInputs()) {
        return;
    }

    QSqlRecord newRecord = model->record();
    newRecord.setGenerated("MaLop", false);
    newRecord.setValue(1, ui->LEten->text().trimmed());
    newRecord.setValue(2, Helper::getCurrIdFromComboBox(ui->CBgvcn));
    newRecord.setValue(3, Helper::getCurrIdFromComboBox(ui->CBnamhoc));
    if (!model->insertRecord(-1, newRecord) || !model->submitAll()) {
        QMessageBox::critical(this, "Lỗi thêm dòng", model->lastError().text());
        model->revertAll();
    }
}

void quanlylophoc::onEditCurrentRow() {
    if (ui->tablelophoc->selectionModel()->hasSelection()) {
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

void quanlylophoc::onDeleteCurrentRow() {
    Helper::tryDeleteCurrentRow(model, ui->tablelophoc);
}

void quanlylophoc::onAddSchoolYear() {
    const QString &&schoolYear = ui->LEnamhocmoi->text().trimmed();

    if (schoolYear.isEmpty()) {
        QMessageBox::critical(this, "Lỗi thêm năm học",
                              "Vui lòng nhập năm học.");
        return;
    }
    if (Helper::ifValueExistsInTable(
            model->database(), "NamHoc"_L1, "TenNamHoc"_L1, schoolYear, this)) {
        QMessageBox::critical(this,
                              "Lỗi thêm năm học",
                              "Năm học này đã tồn tại, nên không thể thêm được.");
        return;
    }

    QSqlTableModel *relModel = model->relationModel(3);

    QSqlRecord newRecord = relModel->record();
    newRecord.setValue(0, ui->LEnamhocmoi->text().trimmed());
    if (!relModel->insertRecord(-1, newRecord) || !relModel->submitAll()) {
        QMessageBox::critical(this,
                              "Lỗi thêm năm học",
                              relModel->lastError().text());
        relModel->revertAll();
    }
}

void quanlylophoc::onDeleteSchoolYear() {
    const QString &&schoolYear = ui->LEnamhocmoi->text().trimmed();

    if (schoolYear.isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập tên.");
        return;
    }

    if (!Helper::ifValueExistsInTable(
            model->database(), "NamHoc"_L1, "TenNamHoc"_L1, schoolYear, this)) {
        QMessageBox::critical(this,
                              "Lỗi xoá năm học",
                              "Năm học này không tồn tại, nên không thể xoá được.");
    } else {
        QSqlTableModel *relModel = model->relationModel(3);

        const auto &&matches = relModel->match(
            relModel->index(0, 0), Qt::EditRole, schoolYear, 1,
            Qt::MatchExactly);
        if (!matches.empty()) {
            if (!relModel->removeRow(matches[0].row())
                || !relModel->submitAll()) {
                QMessageBox::critical(this,
                                      "Lỗi xoá dòng",
                                      relModel->lastError().text());
                relModel->revertAll();
            } else {
                relModel->select();
            }
        }
    }
}
