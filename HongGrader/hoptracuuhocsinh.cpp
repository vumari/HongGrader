#include "hoptracuuhocsinh.h"
#include "ui_hoptracuuhocsinh.h"

#include "helper.h"

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlField>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QCompleter>

hoptracuuhocsinh::hoptracuuhocsinh(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::hoptracuuhocsinh) {
    ui->setupUi(this);
}

hoptracuuhocsinh::~hoptracuuhocsinh() {
    delete ui;
}

void hoptracuuhocsinh::init(QSqlDatabase db) {
    this->db = db;
    if (db.open()) {
        schoolYearModel = new QSqlQueryModel(this);
        schoolYearModel->setQuery("SELECT * FROM NamHoc", db);
        ui->CBnamhoc->setModel(schoolYearModel);

        connect(ui->CBnamhoc, &QComboBox::currentIndexChanged,
                this, &hoptracuuhocsinh::onSchoolYearChanged);

        classModel = new QSqlTableModel(this, db);
        classModel->setTable("Lop");
        classModel->select();
        ui->CBlop->setModel(classModel);
        ui->CBlop->setModelColumn(1);

        connect(ui->CBlop, &QComboBox::currentIndexChanged,
                this, &hoptracuuhocsinh::onClassChanged);

        studentModel = new QSqlQueryModel(this);
        QCompleter *completer = new QCompleter(studentModel, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setCompletionColumn(1);
        ui->LETenHS->setCompleter(completer);

        connect(ui->BTtracuu, &QPushButton::clicked,
                this, &hoptracuuhocsinh::find);
    }
}

void hoptracuuhocsinh::changeEvent(QEvent *e) {
    QGroupBox::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void hoptracuuhocsinh::onSchoolYearChanged(const int &index) {
    classModel->setFilter(u"TenNamHoc = '%1'"_s.arg(ui->CBnamhoc->currentText()));
    if (classModel->lastError().isValid()) {
        QMessageBox::critical(this, "Lỗi",
                              classModel->lastError().text());
    }
}

void hoptracuuhocsinh::onClassChanged(const int &index) {
    QSqlQuery query{ db };

    if (!query.prepare(
            R"(SELECT HoTen FROM HocSinh
INNER JOIN ChiTietHocSinh_Lop CT
ON CT.MaHS = HocSinh.MaHS
WHERE CT.MaLop = ?)")
        ) {
        if (query.lastError().isValid()) {
            QMessageBox::critical(this, "Lỗi CSDL",
                                  query.lastError().text());
        }
    }
    query.addBindValue(Helper::getCurrIdFromComboBox(ui->CBlop));
    studentModel->setQuery(std::move(query));
}

bool hoptracuuhocsinh::checkValidInputs() {
    if (ui->LETenHS->text().trimmed().isEmpty()) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng nhập họ và tên.");
        return false;
    }
    if (ui->CBnamhoc->currentIndex() == -1) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng chọn năm học.");
        return false;
    }
    if (ui->CBlop->currentIndex() == -1) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Vui lòng chọn lớp học.");
        return false;
    }

    return true;
}

void hoptracuuhocsinh::find() {
    if (!checkValidInputs()) {
        return;
    }

    QSqlQuery query{ db };

    if (!query.prepare(
            R"(SELECT HS.MaHS FROM HocSinh HS
INNER JOIN ChiTietHocSinh_Lop CT
ON CT.MaHS = HS.MaHS
WHERE CT.MaLop = ? AND HS.HoTen = ?)")
        ) {
        QMessageBox::critical(this, "Lỗi CSDL",
                              query.lastError().text());
    }
    query.addBindValue(Helper::getCurrIdFromComboBox(ui->CBlop));
    query.addBindValue(ui->LETenHS->text().trimmed());

    if (query.exec()) {
        if (query.next()) {
            const int id = query.record().value(0).toInt();
            ui->LEketqua->setText(QString::number(id));
            emit lookupPerformed(id);
        } else {
            QMessageBox::information(this, "Không thấy",
                                     "Không tìm thấy học sinh nào cả.");
            ui->LEketqua->clear();
        }
    } else {
        QMessageBox::critical(this, "Lỗi CSDL",
                              query.lastError().text());
    }
}
