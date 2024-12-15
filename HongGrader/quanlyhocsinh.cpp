#include "quanlyhocsinh.h"
#include "ui_quanlyhocsinh.h"

#include "genderitemdelegate.h"
#include "helper.h"

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

        classDetailsModel = new QSqlQueryModel(this);

        ui->listlop->setModel(classDetailsModel);
        ui->listlop->horizontalHeader()->setSectionResizeMode(
            QHeaderView::ResizeToContents);

        QSqlQueryModel *schoolYearModel = new QSqlQueryModel(this);
        schoolYearModel->setQuery("SELECT * FROM NamHoc", db);
        ui->CBnamhoc->setModel(schoolYearModel);
        ui->CBlocnamhoc->setModel(schoolYearModel);

        connect(ui->CBnamhoc, &QComboBox::currentIndexChanged,
                this, &quanlyhocsinh::onSchoolYearChanged);
        connect(ui->CBlocnamhoc, &QComboBox::currentIndexChanged,
                this, &quanlyhocsinh::onSchoolYearFilterChanged);

        classModel = new QSqlTableModel(this, db);
        classModel->setTable("Lop");
        classModel->select();
        ui->CBlop->setModel(classModel);
        ui->CBlop->setModelColumn(1);

        classModel2 = new QSqlTableModel(this, db);
        classModel2->setTable("Lop");
        classModel2->select();
        ui->CBloclop->setModel(classModel2);
        ui->CBloclop->setModelColumn(1);

        onFilter();
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
    connect(ui->BTloc, &QPushButton::clicked,
            this, &quanlyhocsinh::onFilter);
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

    const static QLatin1StringView queryTemplate{
        "IF EXISTS(SELECT * FROM HocSinh WHERE HoTen = ? AND NgaySinh = ? AND GioiTinh = ? AND DanToc = ? AND NoiSinh = ?) SELECT 1 ELSE SELECT 0" };

    QSqlQuery query{ model->database() };

    query.prepare(queryTemplate);
    query.addBindValue(ui->LEten->text().trimmed());
    query.addBindValue(ui->DEngaysinh->date());
    query.addBindValue(ui->RBnu->isChecked());
    query.addBindValue(ui->LEdantoc->text().trimmed());
    query.addBindValue(ui->LEnoisinh->text().trimmed());

    if (!query.exec() || !query.first()) {
        QMessageBox::critical(this, "Lỗi CSDL",
                              query.lastError().text());
        return false;
    }
    if (query.record().value(0).toBool()) {
        QMessageBox::critical(this,
                              "Lỗi trùng học sinh",
                              "Đã có học sinh trùng với các thuộc tính này rồi.");

        return false;
    }

    return true;
}

void quanlyhocsinh::loadClassDetails() {
    const static QLatin1StringView queryTemplate{
        R"(SELECT CT.MaLop, Lop.TenLop, Lop.TenNamHoc FROM ChiTietHocSinh_Lop CT
INNER JOIN Lop
ON Lop.MaLop = CT.MaLop
WHERE MaHS = ?)"
    };

    auto *selectionModel = ui->tablehocsinh->selectionModel();

    if (selectionModel->hasSelection()) {
        const int &&currId = selectionModel->
                             currentIndex().siblingAtColumn(0).data().toInt();

        QSqlQuery query{ model->database() };
        query.prepare(queryTemplate);
        query.addBindValue(currId);
        if (!query.exec() || query.lastError().isValid()) {
            QMessageBox::critical(this, "Lỗi CSDL",
                                  query.lastError().text());
        }

        classDetailsModel->setQuery(std::move(query));
        ui->listlop->hideColumn(0);
    } else {
        classDetailsModel->clear();
    }
}

void quanlyhocsinh::onCurrRowChanged(const QModelIndex &current,
                                     const QModelIndex &previous) {
    mapper->setCurrentModelIndex(current);

    loadClassDetails();
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
    const auto *selectionModel = ui->tablehocsinh->selectionModel();

    if (selectionModel->hasSelection()) {
        if (Helper::ifValueExistsInTable(
                model->database(), "ChiTietHocSinh_Lop"_L1, "MaHS"_L1,
                selectionModel->currentIndex().siblingAtColumn(0).data(),
                this)) {
            QMessageBox::critical(this,
                                  "Lỗi xoá học sinh",
                                  "Không thể xoá học sinh do có lớp liên kết với học sinh này.");
            return;
        }
        if (Helper::ifValueExistsInTable(
                model->database(), "DiemTongHop"_L1, "MaHS"_L1,
                selectionModel->currentIndex().siblingAtColumn(0).data(),
                this)) {
            QMessageBox::critical(this,
                                  "Lỗi xoá học sinh",
                                  "Không thể xoá học sinh do có điểm liên kết với học sinh này.");
            return;
        }
        Helper::tryDeleteCurrentRow(model, ui->tablehocsinh);
    }
}

void quanlyhocsinh::onAddClass() {
    if (ui->CBlop->currentIndex() == -1) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng chọn lớp.");
        return;
    }

    QItemSelectionModel *selectionModel = ui->tablehocsinh->selectionModel();
    if (selectionModel->hasSelection()) {
        const int currId = ui->tablehocsinh->selectionModel()->
                           currentIndex().siblingAtColumn(0).data().toInt();
        const int classId = Helper::getCurrIdFromComboBox(ui->CBlop).toInt();

        QSqlQuery query{ model->database() };
        query.prepare(
            "IF EXISTS (SELECT 1 FROM ChiTietHocSinh_Lop WHERE MaHS = ? AND MaLop = ?) SELECT 1 ELSE SELECT 0;");
        query.addBindValue(currId);
        query.addBindValue(classId);
        if (query.exec() && query.next()) {
            if (query.value(0).toBool()) {
                QMessageBox::critical(this,
                                      "Lỗi thêm học sinh vào lớp",
                                      "Học sinh này đã được thêm vào lớp này rồi.");
                return;
            }
        } else {
            QMessageBox::critical(this, "Lỗi thêm học sinh vào lớp",
                                  query.lastError().text());
            return;
        }

        query.prepare(
            "INSERT INTO ChiTietHocSinh_Lop VALUES(?, ?)");
        query.addBindValue(currId);
        query.addBindValue(classId);
        if (!query.exec()) {
            QMessageBox::critical(this, "Lỗi thêm học sinh vào lớp",
                                  query.lastError().text());
        }

        onFilter();
        loadClassDetails();
    } else {
        QMessageBox::information(this, "Hãy chọn học sinh",
                                 "Xin vui lòng chọn 1 học sinh để thêm vào lớp");
    }
}

void quanlyhocsinh::onDeleteClass() {
    QItemSelectionModel *selectionModel = ui->listlop->selectionModel();

    if (selectionModel->hasSelection()) {
        const int studentId = ui->tablehocsinh->selectionModel()->
                              currentIndex().siblingAtColumn(0).data().toInt();

        int classId =
            selectionModel->currentIndex().siblingAtColumn(0).data().toInt();

        QSqlQuery query{ model->database() };
        query.prepare(
            "DELETE FROM ChiTietHocSinh_Lop WHERE MaHS = ? AND MaLop = ?");
        query.addBindValue(studentId);
        query.addBindValue(classId);
        if (!query.exec()) {
            QMessageBox::critical(this, "Lỗi xoá học sinh ra khỏi lớp",
                                  query.lastError().text());
        }
    } else {
        QMessageBox::information(this, "Hãy chọn học sinh",
                                 "Xin vui lòng chọn 1 học sinh để xoá trong lớp");
    }

    onFilter();
    loadClassDetails();
}

void quanlyhocsinh::onSchoolYearChanged(const int &index) {
    classModel->setFilter(u"TenNamHoc = '%1'"_s.arg(ui->CBnamhoc->currentText()));
}

void quanlyhocsinh::onSchoolYearFilterChanged(const int &index) {
    classModel2->setFilter(u"TenNamHoc = '%1'"_s.arg(ui->CBlocnamhoc->
                                                     currentText()));
}

void quanlyhocsinh::onFilter() {
    if (ui->RBtatca->isChecked()) {
        model->setFilter(QString());
    } else if (ui->RBchuacolop->isChecked()) {
        model->setFilter(
            u"NOT EXISTS(SELECT * FROM ChiTietHocSinh_Lop CT WHERE CT.MaHS = HocSinh.MaHS)"_s);
    } else {
        model->setFilter("EXISTS(SELECT * FROM ChiTietHocSinh_Lop AS CT WHERE CT.MaHS = HocSinh.MaHS AND CT.MaLop = %1)"_L1.arg(
                             Helper::getCurrIdFromComboBox(
                                 ui->CBloclop).toString()));
    }

    onSchoolYearChanged(0);
    onSchoolYearFilterChanged(0);
    ui->tablehocsinh->selectRow(0);
    loadClassDetails();
}
