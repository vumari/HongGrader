#include "chuyenlop.h"
#include "ui_chuyenlop.h"

#include "helper.h"

#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTimer>

chuyenlop::chuyenlop(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::chuyenlop) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSqlDatabase db = QSqlDatabase::database();
    if (db.open()) {
        schoolYearModel = new QSqlQueryModel(this);
        schoolYearModel->setQuery("SELECT * FROM NamHoc", db);
        ui->CBnamhoccu->setModel(schoolYearModel);
        ui->CBnamhocmoi->setModel(schoolYearModel);

        fromClassModel = new QSqlQueryModel(this);
        fromClassModel->setQuery("SELECT MaLop, TenLop FROM Lop", db);
        ui->CBlopcu->setModel(fromClassModel);
        ui->CBlopcu->setModelColumn(1);
        QTimer::singleShot(10, this, &chuyenlop::onOldSchoolYearChanged);

        toClassModel = new QSqlQueryModel(this);
        toClassModel->setQuery("SELECT MaLop, TenLop FROM Lop", db);
        ui->CBlopmoi->setModel(toClassModel);
        ui->CBlopmoi->setModelColumn(1);
        QTimer::singleShot(10, this, &chuyenlop::onNewSchoolYearChanged);

        fromModel = new QSqlQueryModel(this);
        Helper::setModelColHeaders(fromModel, {
            "Mã học sinh", "Họ và tên", "ĐTB kì I", "ĐTB kì II", "ĐTB cả năm"
        });
        ui->LVlopcu->setModel(fromModel);
        ui->LVlopcu->horizontalHeader()->setSectionResizeMode(
            QHeaderView::ResizeToContents);

        toModel = new QSqlQueryModel(this);
        Helper::setModelColHeaders(toModel, {
            "Mã học sinh", "Họ và tên", "ĐTB kì I", "ĐTB kì II", "ĐTB cả năm"
        });
        ui->LVlopmoi->setModel(toModel);
        ui->LVlopmoi->horizontalHeader()->setSectionResizeMode(
            QHeaderView::ResizeToContents);

        connect(ui->CBnamhoccu, &QComboBox::currentIndexChanged,
                this, &chuyenlop::onOldSchoolYearChanged);
        connect(ui->CBnamhocmoi, &QComboBox::currentIndexChanged,
                this, &chuyenlop::onNewSchoolYearChanged);
        connect(ui->CBlopcu, &QComboBox::currentIndexChanged,
                this, &chuyenlop::loadOldClassTable);
        connect(ui->CBlopmoi, &QComboBox::currentIndexChanged,
                this, &chuyenlop::loadNewClassTable);
        connect(ui->BTchuyen, &QPushButton::clicked,
                this, &chuyenlop::copySelectedStudents);
    }
}

chuyenlop::~chuyenlop() {
    delete ui;
}

void chuyenlop::onOldSchoolYearChanged() {
    onSchoolYearChanged(ui->CBnamhoccu, fromClassModel);
}

void chuyenlop::onNewSchoolYearChanged() {
    onSchoolYearChanged(ui->CBnamhocmoi, toClassModel);
}

void chuyenlop::onSchoolYearChanged(QComboBox *comboBox,
                                    QSqlQueryModel *model) {
    Q_ASSERT(comboBox != nullptr);
    Q_ASSERT(model != nullptr);
    if (comboBox->currentIndex() > -1) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery    query{ db };
        query.prepare(
            R"(SELECT Malop, TenLop FROM Lop WHERE TenNamHoc = ?)"_L1);
        query.addBindValue(comboBox->currentText());
        if (!query.exec()) {
            QMessageBox::critical(this, "Lỗi CSDL",
                                  query.lastError().text());
        } else {
            model->setQuery(std::move(query));
            return;
        }
    }

    model->clear();
}

void chuyenlop::loadOldClassTable() {
    loadClassTable(fromModel, ui->CBnamhoccu, ui->CBlopcu);
}

void chuyenlop::loadNewClassTable() {
    loadClassTable(toModel, ui->CBnamhocmoi, ui->CBlopmoi);
}

void chuyenlop::loadClassTable(QSqlQueryModel *model,
                               QComboBox *schoolYearCombo,
                               QComboBox *classCombo) {
    const static QLatin1StringView queryTemplate{
        R"(SELECT HS.MaHS, HS.HoTen,
Lop.TenNamHoc,
AVG(DiemHK1) AS HK1, AVG(DiemHK2) AS HK2,
(AVG(DiemHK1) + AVG(DiemHK2) * 2) / 3 AS TBN
FROM HocSinh AS HS
INNER JOIN ChiTietHocSinh_Lop AS CT
ON CT.MaHS = HS.MaHS
INNER JOIN Lop
ON Lop.MaLop = CT.MaLop
AND Lop.MaLop = ?
INNER JOIN (
    SELECT MaHS, AVG(DiemTB) AS DiemHK1
    FROM DiemTongHop AS Diem
    WHERE MaHK = 1
    AND Diem.MaHS = MaHS
    AND Diem.TenNamHoc = TenNamHoc
    GROUP BY MaHS
) AS HK1
ON HK1.MaHS = HS.MaHS
INNER JOIN (
    SELECT MaHS, AVG(DiemTB) AS DiemHK2
    FROM DiemTongHop AS Diem
    WHERE MaHK = 2
    AND Diem.MaHS = MaHS
    AND Diem.TenNamHoc = TenNamHoc
    GROUP BY MaHS
) AS HK2
ON HK1.MaHS = HS.MaHS
GROUP BY HS.MaHS, HoTen, Lop.TenNamHoc)" };

    if (schoolYearCombo->currentIndex() == -1) {
        QMessageBox::critical(this, "Lỗi nhập liệu", "Vui lòng chọn năm học");
        schoolYearCombo->setFocus(Qt::OtherFocusReason);
        return;
    }
    if (classCombo->currentIndex() == -1) {
        // QMessageBox::critical(this, "Lỗi nhập liệu", "Vui lòng chọn lớp học");
        classCombo->setFocus(Qt::OtherFocusReason);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery    query{ db };

    query.prepare(queryTemplate);
    query.addBindValue(Helper::getCurrIdFromComboBox(classCombo));
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
        "Mã học sinh", "Họ và tên", "ĐTB kì I", "ĐTB kì II", "ĐTB cả năm"
    });
}

void chuyenlop::copySelectedStudents() {
    if (ui->CBlopmoi->currentIndex() == -1) {
        QMessageBox::critical(this, "Lỗi nhập liệu", "Vui lòng chọn lớp học");
        ui->CBlopmoi->setFocus(Qt::OtherFocusReason);
        return;
    }

    auto *selectionModel = ui->LVlopcu->selectionModel();
    if (selectionModel->hasSelection()) {
        const int classId =
            Helper::getCurrIdFromComboBox(ui->CBlopmoi).toInt();
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery    query{ db };

        for (const QModelIndex &row: selectionModel->selectedRows()) {
            const int studentId = row.data().toInt();

            query.prepare(
                "IF EXISTS (SELECT 1 FROM ChiTietHocSinh_Lop WHERE MaHS = ? AND MaLop = ?) SELECT 1 ELSE SELECT 0;");
            query.addBindValue(studentId);
            query.addBindValue(classId);
            if (query.exec() && query.next()) {
                if (!query.value(0).toBool()) {
                    query.prepare(
                        "INSERT INTO ChiTietHocSinh_Lop VALUES(?, ?)");
                    query.addBindValue(studentId);
                    query.addBindValue(classId);
                    if (!query.exec()) {
                        QMessageBox::critical(this, "Lỗi thêm học sinh vào lớp",
                                              query.lastError().text());
                        return;
                    }
                }
            } else {
                QMessageBox::critical(this, "Lỗi thêm học sinh vào lớp",
                                      query.lastError().text());
                return;
            }
        }

        loadOldClassTable();
        loadNewClassTable();
    } else {
        QMessageBox::information(this,
                                 "Hãy chọn học sinh",
                                 "Xin vui lòng chọn ít nhất 1 học sinh để chuyển lớp");
    }
}
