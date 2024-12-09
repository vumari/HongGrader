#include "scoremodel.h"

#include "helper.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlError>

const int lockedCol     = 7;
const int scoreStartCol = 8;
const int scoreEndCol   = scoreStartCol + 5;
const int avgScoreCol   = scoreEndCol + 1;
const int passCol       = avgScoreCol + 1;

ScoreModel::ScoreModel(QObject *parent)
    : QSqlQueryModel(parent) {
    parentWidget = qobject_cast<QWidget *>(parent);

    if (!QSqlDatabase::contains("scoremodel")) {
        QSqlDatabase::cloneDatabase(QSqlDatabase::defaultConnection,
                                    "scoremodel");
    }

    db = QSqlDatabase::database("scoremodel");
    db.open();
}

ScoreModel::~ScoreModel() {
    db.close();
}

QSqlDatabase ScoreModel::database() const {
    return db;
}

void ScoreModel::select() {
    const static QLatin1StringView queryStr{
        R"(SELECT Diem.MaHS AS MaHS, HS.HoTen AS HoTen,
       Diem.MaMon AS MaMon, Mon.TenMon AS TenMon,
       Diem.MaHK AS MaHK, HK.TenHK AS TenHK,
       NH.TenNamHoc, NH.TrangThai,
       tx1, tx2, tx3, tx4, gk, ck, DiemTB, KetQua
FROM DiemTongHop AS Diem
INNER JOIN HocSinh AS HS
ON HS.MaHS = Diem.MaHS
INNER JOIN Mon
ON Mon.MaMon = Diem.MaMon
INNER JOIN HocKi AS HK
ON HK.MaHK = Diem.MaHK
INNER JOIN NamHoc AS NH
ON NH.TenNamHoc = Diem.TenNamHoc)" };

    QSqlQuery query{ db };

    if (!query.exec(queryStr)) {
        QMessageBox::critical(parentWidget, "Lỗi CSDL",
                              query.lastError().text());
    }

    setQuery(std::move(query));
    if (lastError().isValid()) {
        QMessageBox::critical(parentWidget, "Lỗi CSDL",
                              lastError().text());
    }

    changedCells.clear();
    applyHeaders();
}

void ScoreModel::appendRow(const int studentId,
                           const QString &subjectId,
                           const int &termId,
                           const QString &schoolYear) {
    const static QLatin1StringView query1Template{
        "IF EXISTS(SELECT * FROM DiemTongHop WHERE MaHS = ? AND MaMon = ? AND MaHK = ? AND TenNamHoc = ?) SELECT 1 ELSE SELECT 0" };
    const static QLatin1StringView query2Template{
        "INSERT INTO DiemTongHop(MaHS, MaMon, MaHK, TenNamHoc) VALUES(?, ?, ?, ?)" };

    QSqlQuery query{ db };

    query.prepare(query1Template);
    query.addBindValue(studentId);
    query.addBindValue(subjectId);
    query.addBindValue(termId);
    query.addBindValue(schoolYear);

    if (!query.exec() || !query.first()) {
        QMessageBox::critical(parentWidget, "Lỗi thêm hàng điểm",
                              query.lastError().text());
        select();
        return;
    }
    if (query.record().value(0).toBool()) {
        QMessageBox::critical(parentWidget,
                              "Lỗi thêm hàng điểm",
                              "Đã có hàng điểm trùng với các thuộc tính này rồi.");
        select();
        return;
    }

    query.prepare(query2Template);
    query.addBindValue(studentId);
    query.addBindValue(subjectId);
    query.addBindValue(termId);
    query.addBindValue(schoolYear);

    if (!query.exec()) {
        QMessageBox::critical(parentWidget, "Lỗi thêm hàng điểm",
                              query.lastError().text());
    }

    select();
}

bool ScoreModel::deleteRow(const int studentId, const QString &subjectId,
                           const int &termId, const QString &schoolYear,
                           bool refresh) {
    const static QLatin1StringView queryTemplate{
        "DELETE FROM DiemTongHop WHERE MaHS = ? AND MaMon = ? AND MaHK = ? AND TenNamHoc = ?" };

    QSqlQuery query{ db };

    query.prepare(queryTemplate);
    query.addBindValue(studentId);
    query.addBindValue(subjectId);
    query.addBindValue(termId);
    query.addBindValue(schoolYear);

    if (!query.exec()) {
        QMessageBox::critical(parentWidget, "Lỗi xoá hàng điểm",
                              query.lastError().text());
        select();
        return false;
    }

    if (refresh) {
        select();
    }
    return true;
}

bool ScoreModel::submitAll() {
    bool ok = true;

    for (auto it = changedCells.cbegin(); it != changedCells.cend(); ++it) {
        const int row = it.key().row();
        ok = updateItem(index(row, 0).data().toInt(),
                        index(row, 2).data().toString(),
                        index(row, 4).data().toInt(),
                        index(row, 6).data().toString(), it.key(),
                        it.value());
        if (!ok) {
            break;
        }
    }

    select();
    return ok;
}

void ScoreModel::applyHeaders() {
    Helper::setModelColHeaders(this, {
        "Mã HS", "Họ và tên", "Mã môn", "Môn học", "Mã học kì", "Học kì",
        "Năm học", "Bị khoá", "TX 1", "TX 2", "TX 3", "TX 4", "GK", "CK", "TB",
        "Kết quả" });
}

int ScoreModel::rowCount(const QModelIndex &parent) const {
    return QSqlQueryModel::rowCount(parent);
}

int ScoreModel::columnCount(const QModelIndex &parent) const {
    return QSqlQueryModel::columnCount(parent);
}

QVariant ScoreModel::data(const QModelIndex &index, int role) const {
    QPersistentModelIndex &&pIndex = index;

    if (pIndex.isValid()) {
        const int col = index.column();
        if (((col >= scoreStartCol) && (col <= scoreEndCol))
            || (col == passCol)) {
            if (index.siblingAtColumn(lockedCol).data().toBool()) {
                if (role == Qt::ForegroundRole) {
                    return QColor{ 90, 74, 74 }; // màu xám
                }
            }
        }

        if (changedCells.contains(pIndex)) {
            if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
                return changedCells.value(pIndex);
            } else if (role == Qt::FontRole) {
                QFont font;
                font.setBold(true);
                return font;
            }
        }
    }
    return QSqlQueryModel::data(index, role);
}

bool ScoreModel::setData(const QModelIndex &index, const QVariant &value,
                         int role) {
    if (data(index, role) != value) {
        const int col = index.column();
        if (index.siblingAtColumn(lockedCol).data().toBool()) {
            return false;
        }
        if (col == passCol) {
            const QString &strValue = value.toString();
            if ((strValue != "Dat") && (strValue != "Khong Dat")) {
                return false;
            }
        }
        changedCells.insert(QPersistentModelIndex(index), value);
        emit dataChanged(index, index, { role });
        if ((col >= scoreStartCol) && (col <= scoreEndCol)) {
            updateAvgScore(index);
        }
        return true;
    }
    return false;
}

Qt::ItemFlags ScoreModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QSqlQueryModel::flags(index);

    const int col = index.column();
    if (((col >= scoreStartCol) && (col <= scoreEndCol)) || (col == passCol)) {
        if (!index.siblingAtColumn(lockedCol).data().toBool()) {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}

bool ScoreModel::removeRows(int row, int count, const QModelIndex &parent) {
    const int to = row + count;
    bool      ok = true;

    for (int i = to - 1; i >= row; --i) {
        QSqlRecord curRecord = record(row);
        ok = deleteRow(curRecord.value(0).toInt(),
                       curRecord.value(2).toString(),
                       curRecord.value(4).toInt(),
                       curRecord.value(6).toString(), false);
        if (!ok) {
            break;
        }
    }
    select();
    return ok;
}

bool ScoreModel::updateItem(const int studentId, const QString &subjectId,
                            const int &termId, const QString &schoolYear,
                            const QPersistentModelIndex &index,
                            const QVariant &value) {
    const static QLatin1StringView queryTemplate{
        "UPDATE DiemTongHop SET %1 = ? WHERE MaHS = ? AND MaMon = ? AND MaHK = ? AND TenNamHoc = ?" };

    QSqlQuery       query{ db };
    const QString &&colName = record().fieldName(index.column());

    query.prepare(queryTemplate.arg(colName));
    query.addBindValue(value);
    query.addBindValue(studentId);
    query.addBindValue(subjectId);
    query.addBindValue(termId);
    query.addBindValue(schoolYear);

    if (!query.exec()) {
        QMessageBox::critical(parentWidget, "Lỗi sửa điểm",
                              query.lastError().text());
        return false;
    }
    return true;
}

void ScoreModel::updateAvgScore(const QModelIndex &index) {
    double      sum      = 0;
    int         factor   = 0;
    int         col      = scoreStartCol;
    QModelIndex curIndex = index.siblingAtColumn(col);

    if (curIndex.isValid() && !curIndex.data().isNull()) {
        sum += curIndex.data().toDouble();
        ++factor; // Hệ số 1
    }
    ++col;
    curIndex = index.siblingAtColumn(col);
    if (curIndex.isValid() && !curIndex.data().isNull()) {
        sum += curIndex.data().toDouble();
        ++factor; // Hệ số 1
    }
    ++col;
    curIndex = index.siblingAtColumn(col);
    if (curIndex.isValid() && !curIndex.data().isNull()) {
        sum += curIndex.data().toDouble();
        ++factor; // Hệ số 1
    }
    ++col;
    curIndex = index.siblingAtColumn(col);
    if (curIndex.isValid() && !curIndex.data().isNull()) {
        sum += curIndex.data().toDouble();
        ++factor; // Hệ số 1
    }
    ++col;
    curIndex = index.siblingAtColumn(col);
    if (curIndex.isValid() && !curIndex.data().isNull()) {
        sum    += curIndex.data().toDouble() * 2;
        factor += 2; // Hệ số 2
    }
    ++col;
    curIndex = index.siblingAtColumn(col);
    if (curIndex.isValid() && !curIndex.data().isNull()) {
        sum    += curIndex.data().toDouble() * 3;
        factor += 3; // Hệ số 3
    }

    ++col;
    curIndex = index.siblingAtColumn(col);
    if (factor > 0) {
        const double avg = std::round(sum * 100 / (double)factor) / 100.0;
        setData(curIndex, avg);
        QString &&passStr = (avg >= 5) ? "Dat"_L1 : "Khong Dat"_L1;
        setData(curIndex.siblingAtColumn(passCol), std::move(passStr));
    } else {
        setData(curIndex, QVariant(QMetaType::fromType<float>())); // NULL
        setData(curIndex.siblingAtColumn(passCol), "Khong Dat"_L1);
    }
}
