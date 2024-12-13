#include "thongkehocsinh.h"
#include "ui_thongkehocsinh.h"

#include "helper.h"
#include "tableexporter.h"

#include <QSqlQueryModel>
#include <QSqlField>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTimer>

thongkehocsinh::thongkehocsinh(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::thongkehocsinh) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSqlDatabase db = QSqlDatabase::database();
    if (db.open()) {
        subjectModel = new QSqlQueryModel(this);
        subjectModel->setQuery(
            "SELECT '' AS MaMon, N'Tất cả' AS TenMon UNION SELECT * FROM Mon",
            db);
        ui->CBmonhoc->setModel(subjectModel);
        ui->CBmonhoc->setModelColumn(1);

        schoolYearModel = new QSqlQueryModel(this);
        schoolYearModel->setQuery("SELECT * FROM NamHoc", db);
        ui->CBnamhoc->setModel(schoolYearModel);

        classModel = new QSqlQueryModel(this);
        classModel->setQuery("SELECT MaLop, TenLop FROM Lop", db);
        ui->CBlop->setModel(classModel);
        ui->CBlop->setModelColumn(1);
        QTimer::singleShot(10, this, &thongkehocsinh::onSchoolYearChanged);

        model = new QSqlQueryModel(this);
        Helper::setModelColHeaders(model, {
            "Mã học sinh", "Họ và tên", "Lớp", "Điểm trung bình", "Xếp loại"
        });
        ui->tabletkhocsinh->setModel(model);
        ui->tabletkhocsinh->horizontalHeader()->setSectionResizeMode(
            QHeaderView::ResizeToContents);

        // connect(ui->CBmonhoc, &QComboBox::currentIndexChanged,
        //         this, &thongkehocsinh::displayPoints);
        // connect(ui->CBnamhoc, &QComboBox::currentIndexChanged,
        //         this, &thongkehocsinh::displayPoints);
        // connect(ui->CBdiem, &QComboBox::currentIndexChanged,
        //         this, &thongkehocsinh::displayPoints);
        connect(ui->CBnamhoc, &QComboBox::currentIndexChanged,
                this, &thongkehocsinh::onSchoolYearChanged);
        connect(ui->BThienthi, &QPushButton::clicked,
                this, &thongkehocsinh::displayPoints);
        connect(ui->BTxuatfile, &QPushButton::clicked,
                this, &thongkehocsinh::exportFile);
    }
}

thongkehocsinh::~thongkehocsinh() {
    delete ui;
}

void thongkehocsinh::onSchoolYearChanged() {
    if (ui->CBnamhoc->currentIndex() > -1) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery    query{ db };
        query.prepare(
            R"(SELECT Malop, TenLop FROM Lop WHERE TenNamHoc = ?)"_L1);
        query.addBindValue(ui->CBnamhoc->currentText());
        if (!query.exec()) {
            QMessageBox::critical(this, "Lỗi CSDL",
                                  query.lastError().text());
        } else {
            classModel->setQuery(std::move(query));
            return;
        }
    }

    classModel->clear();
}

void thongkehocsinh::displayPoints() {
    const static QLatin1StringView selectClause{
        R"(SELECT HS.MaHS, HS.HoTen, AVG(Diem.DiemTB)
FROM DiemTongHop Diem
INNER JOIN HocSinh HS
ON Diem.MaHS = HS.MaHS
INNER JOIN Lop
ON Diem.TenNamHoc = Lop.TenNamHoc)" };
    const static QLatin1StringView groupByClause{
        "GROUP BY HS.MaHS, HS.HoTen" };

    if (ui->CBmonhoc->currentIndex() == -1) {
        QMessageBox::critical(this, "Lỗi nhập liệu", "Vui lòng chọn môn học");
        ui->CBmonhoc->setFocus(Qt::OtherFocusReason);
        return;
    }
    if (ui->CBnamhoc->currentIndex() == -1) {
        QMessageBox::critical(this, "Lỗi nhập liệu", "Vui lòng chọn năm học");
        ui->CBnamhoc->setFocus(Qt::OtherFocusReason);
        return;
    }

    QStringList whereFilters;

    if (ui->CBmonhoc->currentIndex() > 0) {
        whereFilters << "Diem.MaMon = '%1'"_L1.arg(
            Helper::getCurrIdFromComboBox(ui->CBmonhoc).toString());
    }
    if (ui->CBnamhoc->currentIndex() != -1) {
        whereFilters << "Lop.MaLop = %1"_L1.arg(
            Helper::getCurrIdFromComboBox(ui->CBlop).toString());
    }

    QStringList havingFilters;
    switch (ui->CBdiem->currentIndex()) {
        case 1: {
            havingFilters << "AVG(Diem.DiemTB) >= 8";
            break;
        }
        case 2: {
            havingFilters << "AVG(Diem.DiemTB) >= 5";
            break;
        }
        case 3: {
            havingFilters << "AVG(Diem.DiemTB) < 5";
            break;
        }
    }

    QStringList queryParts;
    queryParts << selectClause;
    if (!whereFilters.empty()) {
        queryParts << "WHERE" << whereFilters.join(" AND "_L1);
    }
    queryParts << groupByClause;
    if (!havingFilters.empty()) {
        queryParts << "HAVING" << havingFilters.join(" AND "_L1);
    }
    queryParts << "ORDER BY AVG(Diem.DiemTB) DESC";

    QSqlDatabase db = QSqlDatabase::database();
    // qDebug() << queryParts.join(' ');
    model->setQuery(queryParts.join(' '), db);
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Lỗi CSDL",
                              model->lastError().text());
    }
    Helper::setModelColHeaders(model, {
        "Mã học sinh", "Họ và tên", "Điểm trung bình", "Xếp loại"
    });
}

void thongkehocsinh::exportFile() {
    if (TableExporter(model, this).startExport()) {
        QMessageBox::information(this, "Thành công", "Đã xuất tệp xong.");
    }
}
