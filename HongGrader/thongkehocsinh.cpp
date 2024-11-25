#include "thongkehocsinh.h"
#include "ui_thongkehocsinh.h"

#include "helper.h"

#include <QSqlQueryModel>
#include <QSqlField>
#include <QSqlError>
#include <QMessageBox>
#include <QFileDialog>
#include <QSaveFile>
#include <QTextStream>

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
        connect(ui->BThienthi, &QPushButton::clicked,
                this, &thongkehocsinh::displayPoints);
        connect(ui->BTxuatfile, &QPushButton::clicked,
                this, &thongkehocsinh::exportFile);
    }
}

thongkehocsinh::~thongkehocsinh() {
    delete ui;
}

void thongkehocsinh::displayPoints() {
    const static QLatin1StringView selectClause{
        R"(SELECT HS.MaHS, HS.HoTen, Lop.TenLop, AVG(Diem.DiemTB)
FROM DiemTongHop Diem
INNER JOIN HocSinh HS
ON Diem.MaHS = HS.MaHS
INNER JOIN Lop
ON Lop.MaLop = Diem.MaLop)" };
    const static QLatin1StringView groupByClause{
        "GROUP BY HS.MaHS, HS.HoTen, Lop.TenLop" };

    QStringList whereFilters;

    if (ui->CBmonhoc->currentIndex() > 0) {
        whereFilters << "Diem.MaMon = '%1'"_L1.arg(
            Helper::getCurrIdFromComboBox(ui->CBmonhoc).toString());
    }
    if (ui->CBnamhoc->currentIndex() != -1) {
        whereFilters << "Lop.TenNamHoc = '%1'"_L1.arg(
            ui->CBnamhoc->currentText());
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

    QSqlDatabase db = QSqlDatabase::database();

    model->setQuery(queryParts.join(' '), db);
    Helper::setModelColHeaders(model, {
        "Mã học sinh", "Họ và tên", "Lớp", "Điểm trung bình", "Xếp loại"
    });
}

void thongkehocsinh::exportFile() {
    const static QLatin1StringView quotedString{ R"("%1")" };
    const static QString           filters =
        u"CSV ngăn cách bằng dấu phẩy (*.csv);;CSV tương thích với Excel (*.csv)"_s;

    QString selectedFilter =
        u"CSV tương thích với Excel (*.csv)"_s;
    const QString &&filePath = QFileDialog::getSaveFileName(
        this, "Chọn vị trí xuất tệp", QString(),
        filters, &selectedFilter);

    QString     output;
    QTextStream tout(&output, QTextStream::WriteOnly);

    tout.setRealNumberPrecision(2);
    bool forceUtf8 = false;

    while (model->canFetchMore())
        model->fetchMore();

    if (selectedFilter.startsWith("CSV"_L1)) {
        bool  excelCompat = selectedFilter.contains("Excel"_L1);
        QChar sep         = excelCompat ? ';' : ',';
        if (excelCompat) {
            forceUtf8 = true;
            tout << R"(Mã;"Họ và tên";Lớp;ĐTB)";
            tout.setLocale(QLocale(QLocale::Vietnamese));
        } else {
            tout << R"(Mã,"Họ và tên",Lớp,ĐTB)";
        }
        for (int i = 0; i < model->rowCount(); ++i) {
            tout << '\n';
            QSqlRecord &&record = model->record(i);
            for (int j = 0; j < record.count(); ++j) {
                if (j > 0) {
                    tout << sep;
                }
                QSqlField &&field = record.field(j);
                switch (field.metaType().id()) {
                    case QMetaType::Float: {
                        tout << field.value().toFloat();
                        break;
                    }
                    case QMetaType::Double: {
                        tout << field.value().toDouble();
                        break;
                    }
                    case QMetaType::Int:
                    case QMetaType::Long:
                    case QMetaType::LongLong:
                    case QMetaType::UInt:
                    case QMetaType::ULong:
                    case QMetaType::ULongLong: {
                        tout << field.value().toString();
                        break;
                    }
                    case QMetaType::QString: {
                        tout << quotedString.arg(
                            field.value().toString().replace('"', R"("")"));
                        break;
                    }
                    default: {
                        tout << field.value().toString();
                    }
                }
            }
        }
    }

    QSaveFile data(filePath);
    if (data.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        if (forceUtf8) {
            data.write("\uFEFF"); // Byte Order Mark (BOM)
        }
        data.write(output.toUtf8());
        if (!data.commit()) {
            QMessageBox::critical(this, "Error saving file",
                                  data.errorString());
        }
    } else {
        QMessageBox::critical(this, "Error saving file", data.errorString());
    }
}
