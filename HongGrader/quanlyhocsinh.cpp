#include "quanlyhocsinh.h"
#include "ui_quanlyhocsinh.h"

#include "genderitemdelegate.h"
#include "helper.h"

#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include <QSqlField>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QStringListModel>
#include <QCompleter>

static const QStringList ethnicities = {
    "Kinh",
    "Tày",
    "Thái",
    "Hoa",
    "Khơ-me",
    "Mường",
    "Nùng",
    "Hmông",
    "Dao",
    "Gia-rai",
    "Ngái",
    "Ê-đê",
    "Ba-na",
    "Xơ-đăng",
    "Sán Chay",
    "Cơ-ho",
    "Chăm",
    "Sán Dìu",
    "Hrê",
    "Mnông",
    "Ra-giai",
    "Xtiêng",
    "Bru – Vân Kiều",
    "Thổ",
    "Giáy",
    "Cơ-tu",
    "Gié - Triêng",
    "Mạ",
    "Khơ-mú",
    "Co",
    "Ta-ôi",
    "Chơ-ro",
    "Kháng",
    "Xinh-mun",
    "Hà Nhì",
    "Chu-ru",
    "Lào",
    "La Chí",
    "La Ha",
    "Phù Lá",
    "La Hủ",
    "Lự",
    "Lô Lô",
    "Chứt",
    "Mảng",
    "Pà Thẻn",
    "Cơ Lao",
    "Cống",
    "Bố Y",
    "Si La",
    "Pu Péo",
    "Brâu",
    "Ơ-đu",
    "Rơ-mă",
};

quanlyhocsinh::quanlyhocsinh(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlyhocsinh) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSqlDatabase db = QSqlDatabase::database();
    if (db.open()) {
        model = new QSqlTableModel(this, db);
        model->setTable("HocSinh");

        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();
        Helper::setModelColHeaders(model, {
            "Mã", "Họ và tên", "Ngày sinh", "Giới tính", "Dân tộc", "Nơi sinh",
        });

        ui->tablehocsinh->setModel(model);
        ui->tablehocsinh->setItemDelegate(
            new QSqlRelationalDelegate(this));
        ui->tablehocsinh->setItemDelegateForColumn(
            3, new GenderItemDelegate(this));
        ui->tablehocsinh->horizontalHeader()->setSectionResizeMode(
            QHeaderView::ResizeToContents);

        mapper = new QDataWidgetMapper(this);
        mapper->setModel(model);
        mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        mapper->setItemDelegate(
            new GenderItemDelegate(this, ui->RBnam, ui->RBnu));
        mapper->addMapping(ui->LEten, 1);
        mapper->addMapping(ui->DEngaysinh, 2);
        mapper->addMapping(ui->RBnu, 3);
        mapper->addMapping(ui->LEdantoc, 4);
        mapper->addMapping(ui->LEnoisinh, 5);

        connect(ui->tablehocsinh->selectionModel(),
                &QItemSelectionModel::currentRowChanged,
                this, &quanlyhocsinh::onCurrRowChanged);

        classDetailsModel = new QSqlRelationalTableModel(this, db);
        classDetailsModel->setTable("ChiTietHocSinh_Lop");

        classDetailsModel->setRelation(1,
                                       QSqlRelation("Lop", "MaLop", "TenLop"));
        classDetailsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        ui->listlop->setModel(classDetailsModel);
        ui->listlop->setModelColumn(1);
        ui->listlop->setItemDelegate(new QSqlRelationalDelegate(this));

        QSqlQueryModel *schoolYearModel = new QSqlQueryModel(this);
        schoolYearModel->setQuery("SELECT * FROM NamHoc", db);
        ui->CBnamhoc->setModel(schoolYearModel);

        connect(ui->CBnamhoc, &QComboBox::currentIndexChanged,
                this, &quanlyhocsinh::onSchoolYearChanged);

        classModel = new QSqlTableModel(this, db);
        classModel->setTable("Lop");
        classModel->select();
        ui->CBlop->setModel(classModel);
        ui->CBlop->setModelColumn(1);
        onSchoolYearChanged(0);
    }

    QCompleter *completer = new QCompleter(this);
    completer->setModel(new QStringListModel(ethnicities, this));
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->LEdantoc->setCompleter(completer);

    connect(ui->BTthem, &QPushButton::clicked, this, &quanlyhocsinh::onAddRow);
    connect(ui->BTsua, &QPushButton::clicked,
            this, &quanlyhocsinh::onEditCurrentRow);
    connect(ui->BTxoa, &QPushButton::clicked,
            this, &quanlyhocsinh::onDeleteCurrentRow);
    connect(ui->BTthemlop, &QPushButton::clicked,
            this, &quanlyhocsinh::onAddClass);
    connect(ui->BTxoalop, &QPushButton::clicked,
            this, &quanlyhocsinh::onDeleteClass);
}

quanlyhocsinh::~quanlyhocsinh() {
    delete ui;
}

bool quanlyhocsinh::checkValidInputs() {
    if (ui->LEten->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập họ và tên.");
        ui->LEten->setFocus(Qt::OtherFocusReason);
        return false;
    }
    if (ui->LEnoisinh->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập nơi sinh.");
        ui->LEnoisinh->setFocus(Qt::OtherFocusReason);
        return false;
    }
    if (ui->LEdantoc->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập dân tộc");
        ui->LEdantoc->setFocus(Qt::OtherFocusReason);
        return false;
    }
    return true;
}

void quanlyhocsinh::onCurrRowChanged(const QModelIndex &current,
                                     const QModelIndex &previous) {
    mapper->setCurrentModelIndex(current);

    const int &&currId = current.siblingAtColumn(0).data().toInt();

    classDetailsModel->setFilter(u"MaHS = %1"_s.arg(currId));
    classDetailsModel->select();
}

void quanlyhocsinh::onAddRow() {
    if (!checkValidInputs()) {
        return;
    }

    QSqlRecord newRecord = model->record();
    newRecord.setGenerated("MaHS", false);
    newRecord.setValue(1, ui->LEten->text().trimmed());
    newRecord.setValue(2, ui->DEngaysinh->date());
    newRecord.setValue(3, ui->RBnu->isChecked());
    newRecord.setValue(4, ui->LEdantoc->text().trimmed());
    newRecord.setValue(5, ui->LEnoisinh->text().trimmed());
    if (!model->insertRecord(-1, newRecord) || !model->submitAll()) {
        QMessageBox::critical(this, "Lỗi thêm dòng", model->lastError().text());
        model->revertAll();
    }
}

void quanlyhocsinh::onEditCurrentRow() {
    if (ui->tablehocsinh->selectionModel()->hasSelection()) {
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

void quanlyhocsinh::onDeleteCurrentRow() {
    Helper::tryDeleteCurrentRow(model, ui->tablehocsinh);
}

void quanlyhocsinh::onAddClass() {
    if (ui->CBlop->currentIndex() == -1) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng chọn lớp.");
        return;
    }

    QItemSelectionModel *selectionModel = ui->tablehocsinh->selectionModel();
    if (selectionModel->hasSelection()) {
        const int &&currId = ui->tablehocsinh->selectionModel()->
                             currentIndex().siblingAtColumn(0).data().toInt();

        QSqlRecord newRecord = classDetailsModel->record();
        newRecord.setValue(0, currId);
        newRecord.setValue(1, Helper::getCurrIdFromComboBox(ui->CBlop));
        if (!classDetailsModel->insertRecord(-1, newRecord)
            || !classDetailsModel->submitAll()) {
            QMessageBox::critical(this,
                                  "Lỗi thêm học sinh vào lớp",
                                  classDetailsModel->lastError().text());
            classDetailsModel->revertAll();
        }
    }
}

void quanlyhocsinh::onDeleteClass() {
    QItemSelectionModel *selectionModel = ui->listlop->selectionModel();
    QSqlTableModel      *relModel       = classDetailsModel->relationModel(1);

    if (selectionModel->hasSelection()) {
        const int studentId = ui->tablehocsinh->selectionModel()->
                              currentIndex().siblingAtColumn(0).data().toInt();

        const auto &&matches = relModel->match(
            relModel->index(0, 1), Qt::EditRole,
            selectionModel->currentIndex().siblingAtColumn(1).data(),
            1, Qt::MatchExactly);
        int classId = -1;
        if (!matches.empty()) {
            classId = matches[0].siblingAtColumn(0).data().toInt();
        }

        QSqlQuery query{ model->database() };
        query.prepare(
            "DELETE FROM ChiTietHocSinh_Lop WHERE MaHS = ? AND MaLop = ?");
        query.addBindValue(studentId);
        query.addBindValue(classId);
        if (!query.exec()) {
            QMessageBox::critical(this, "Lỗi xoá học sinh ra khỏi lớp",
                                  query.lastError().text());
        }

        classDetailsModel->setFilter(u"MaHS = %1"_s.arg(studentId));
    }
}

void quanlyhocsinh::onSchoolYearChanged(const int &index) {
    classModel->setFilter(u"TenNamHoc = '%1'"_s.arg(ui->CBnamhoc->currentText()));
}
