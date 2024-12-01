#include "scoremodel.h"

#include "helper.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlError>

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

void ScoreModel::select() {
    const static QLatin1StringView queryStr{
        R"(SELECT Diem.MaHS AS MaHS, HS.HoTen AS HoTen,
       Diem.MaMon AS MaMon, Mon.TenMon AS TenMon,
       Diem.MaHK AS MaHK, HK.TenHK AS TenHK,
       TenNamHoc,
       tx1, tx2, tx3, tx4, gk, ck, DiemTB, KetQua
FROM DiemTongHop AS Diem
INNER JOIN HocSinh AS HS
ON HS.MaHS = Diem.MaHS
INNER JOIN Mon
ON Mon.MaMon = Diem.MaMon
INNER JOIN HocKi AS HK
ON HK.MaHK = Diem.MaHK)" };

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
        "Mã học sinh", "Họ và tên", "Mã môn", "Môn học", "Mã học kì", "Học kì",
        "Năm học", "TX 1", "TX 2", "TX 3", "TX 4", "GK", "CK", "TB",
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
        changedCells.insert(QPersistentModelIndex(index), value);
        emit dataChanged(index, index, { role });
        return true;
    }
    return false;
}

Qt::ItemFlags ScoreModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QSqlQueryModel::flags(index);

    const int col = index.column();
    if (((col >= 7) && (col <= 7 + 5)) || (col == 14)) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

bool ScoreModel::removeRows(int row, int count, const QModelIndex &parent) {
    const int to = row + count;
    bool      ok = true;

    for (int i = to - 1; i >= row; --i) {
        QSqlRecord curRecord = record(row + i);
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
