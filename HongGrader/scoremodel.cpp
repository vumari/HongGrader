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

void ScoreModel::setFilters(const int &classId,
                            const QString &subjectId,
                            const int &termId) {
    filterClassId   = classId;
    filterSubjectId = subjectId;
    filterTermId    = termId;
}

void ScoreModel::clearFilters() {
    filterClassId = -1;
    filterSubjectId.clear();
    filterTermId = -1;
}

void ScoreModel::select() {
    if (filterClassId == -1 || filterTermId == -1 ||
        filterSubjectId.isEmpty()) {
        selectAll();
    } else {
        selectWithFilter(filterClassId, filterSubjectId, filterTermId);
    }

    changedCells.clear();
    applyHeaders();
}

bool ScoreModel::appendRow(const int studentId, const QString &subjectId,
                           const int &termId, const QString &schoolYear,
                           const bool reload) {
    const static QLatin1StringView queryTemplate{
        "INSERT INTO DiemTongHop(MaHS, MaMon, MaHK, TenNamHoc) VALUES(?, ?, ?, ?)" };

    bool ok = false;

    if (rowExists(studentId, subjectId, termId, schoolYear) == NotExists) {
        QSqlQuery query{ db };

        query.prepare(queryTemplate);
        query.addBindValue(studentId);
        query.addBindValue(subjectId);
        query.addBindValue(termId);
        query.addBindValue(schoolYear);

        if (!query.exec()) {
            QMessageBox::critical(parentWidget, "Lỗi thêm hàng điểm",
                                  query.lastError().text());
        } else {
            ok = true;
        }
    }

    if (reload) {
        select();
    }
    return ok;
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

    for (const auto [row, cells]: changedCells.asKeyValueRange()) {
        const int      studentId  = index(row, 0).data().toInt();
        const QString &subjectId  = index(row, 2).data().toString();
        const int     &termId     = index(row, 4).data().toInt();
        const QString &schoolYear = index(row, 6).data().toString();

        auto exists = rowExists(studentId, subjectId, termId, schoolYear);
        if (exists == Error) {
            ok = false;
            break;
        } else if (exists == NotExists) {
            ok = appendRow(studentId, subjectId, termId, schoolYear, false);
        }
        if (!ok) {
            break;
        }

        for (const auto [index, value]: cells.asKeyValueRange()) {
            ok = updateItem(studentId, subjectId, termId, schoolYear,
                            index, value);
            if (!ok) {
                break;
            }
        }
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
        const int row = index.row();
        const int col = index.column();
        if (((col >= scoreStartCol) && (col <= scoreEndCol))
            || (col == passCol)) {
            if (index.siblingAtColumn(lockedCol).data().toBool()) {
                if (role == Qt::ForegroundRole) {
                    return QColor{ 90, 74, 74 }; // màu xám
                }
            }
        }

        if (changedCells.contains(row)) {
            if (changedCells.value(row).contains(pIndex)) {
                if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
                    return changedCells.value(row).value(pIndex);
                } else if (role == Qt::FontRole) {
                    QFont font;
                    font.setBold(true);
                    return font;
                }
            }
        }
    }
    return QSqlQueryModel::data(index, role);
}

bool ScoreModel::setData(const QModelIndex &index, const QVariant &value,
                         int role) {
    if (data(index, role) != value) {
        const int row = index.row();
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
        changedCells[row].insert(QPersistentModelIndex(index), value);
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

void ScoreModel::selectAll() {
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
}

void ScoreModel::selectWithFilter(const int &classId,
                                  const QString &subjectId,
                                  const int &termId) {
    const static QLatin1StringView queryTemplate{
        R"(SELECT T1.MaHS AS MaHS, HoTen, T1.MaMon AS MaMon,
       TenMon, T1.MaHK AS MaHK, TenHK,
       T1.TenNamHoc, T1.TrangThai, tx1, tx2, tx3, tx4,
       gk, ck, DiemTB, KetQua
FROM (SELECT HS.MaHS, HoTen, Mon.MaMon, TenMon,
       HK.MaHK, TenHK, NH.TenNamHoc, TrangThai,
       CT.MaLop, TenLop
    FROM HocSinh AS HS
    INNER JOIN ChiTietHocSinh_Lop AS CT
    ON CT.MaHS = HS.MaHS
    AND CT.MaLop = ?
    INNER JOIN Lop
    ON Lop.MaLop = CT.MaLop
    INNER JOIN NamHoc AS NH
    ON NH.TenNamHoc = Lop.TenNamHoc
    LEFT JOIN Mon
    ON Mon.MaMon = ?
    LEFT JOIN HocKi AS HK
    ON HK.MaHK = ?
) T1
LEFT JOIN DiemTongHop AS Diem
ON T1.MaHS = Diem.MaHS
AND T1.MaMon = Diem.MaMon
AND T1.MaHK = Diem.MaHK
AND T1.TenNamHoc = Diem.TenNamHoc;)" };

    QSqlQuery query{ db };

    query.prepare(queryTemplate);
    query.addBindValue(classId);
    query.addBindValue(subjectId);
    query.addBindValue(termId);

    if (!query.exec()) {
        QMessageBox::critical(parentWidget, "Lỗi CSDL",
                              query.lastError().text());
        return;
    }

    setQuery(std::move(query));
    if (lastError().isValid()) {
        QMessageBox::critical(parentWidget, "Lỗi CSDL",
                              lastError().text());
    }
}

ScoreModel::RowExistsResult ScoreModel::rowExists(const int studentId,
                                                  const QString &subjectId,
                                                  const int &termId,
                                                  const QString &schoolYear) {
    const static QLatin1StringView queryTemplate{
        "IF EXISTS(SELECT * FROM DiemTongHop WHERE MaHS = ? AND MaMon = ? AND MaHK = ? AND TenNamHoc = ?) SELECT 1 ELSE SELECT 0" };

    QSqlQuery query{ db };

    query.prepare(queryTemplate);
    query.addBindValue(studentId);
    query.addBindValue(subjectId);
    query.addBindValue(termId);
    query.addBindValue(schoolYear);

    if (!query.exec() || !query.first()) {
        QMessageBox::critical(parentWidget, "Lỗi CSDL",
                              query.lastError().text());
        return Error;
    }

    return (query.record().value(0).toBool()) ? Exists : NotExists;
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
