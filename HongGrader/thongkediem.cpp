#include "thongkediem.h"
#include "ui_thongkediem.h"

#include "helper.h"
#include "tableexporter.h"

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlError>
#include <QMessageBox>

thongkediem::thongkediem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::thongkediem) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    if (!QSqlDatabase::contains("thongkediem")) {
        QSqlDatabase::cloneDatabase(QSqlDatabase::defaultConnection,
                                    "thongkediem");
    }

    db = QSqlDatabase::database("thongkediem");

    if (db.open()) {
        ui->groupBox_HS->init(db);

        QSqlQueryModel *schoolYearModel = new QSqlQueryModel(this);
        schoolYearModel->setQuery("SELECT * FROM NamHoc", db);
        ui->CBnamhoc->setModel(schoolYearModel);

        model = new QSqlQueryModel(this);
        Helper::setModelColHeaders(model, {
            "Tên môn", "TX1", "TX2", "TX3", "TX4", "GK", "CK", "TB", "Kết quả"
        });
        ui->tabletkdiem->setModel(model);
        ui->tabletkdiem->horizontalHeader()->setSectionResizeMode(
            QHeaderView::ResizeToContents);

        connect(ui->BThienthi, &QPushButton::clicked,
                this, &thongkediem::displayPoints);
        connect(ui->BTxuatfile, &QPushButton::clicked,
                this, &thongkediem::exportFile);
    }
}

thongkediem::~thongkediem() {
    delete ui;
}

void thongkediem::displayPoints() {
    const int studentId = ui->SBmaHS->value();

    if (!Helper::ifStudentIdExists(db, studentId, this)) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Mã học sinh không tồn tại.");
        ui->SBmaHS->setFocus(Qt::OtherFocusReason);
    }

    const static QLatin1StringView queryTemplate{
        R"(SELECT Mon.TenMon, tx1, tx2, tx3, tx4, gk, ck, DiemTB, KetQua
FROM DiemTongHop AS Diem
RIGHT JOIN Mon
ON Mon.MaMon = Diem.MaMon
AND MaHS = ? AND MaHK = ? AND TenNamHoc = ?)" };

    QSqlQuery query{ db };

    query.prepare(queryTemplate);
    query.addBindValue(studentId);
    query.addBindValue(ui->RBHK1->isChecked() ? 1 : 2);
    query.addBindValue(ui->CBnamhoc->currentText());
    if (!query.exec()) {
        QMessageBox::critical(this, "Lỗi CSDL",
                              query.lastError().text());
    }

    model->setQuery(std::move(query));
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Lỗi CSDL",
                              model->lastError().text());
    }
    Helper::setModelColHeaders(model, {
        "Tên môn", "TX1", "TX2", "TX3", "TX4", "GK", "CK", "TB", "Kết quả"
    });
}

void thongkediem::exportFile() {
    if (TableExporter(model, this).startExport()) {
        QMessageBox::information(this, "Thành công", "Đã xuất tệp xong.");
    }
}
