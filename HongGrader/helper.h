#ifndef HELPER_H
#define HELPER_H

#include <QSqlDatabase>
#include <QSqlRecord>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QSqlTableModel;
class QAbstractItemView;
class QComboBox;
QT_END_NAMESPACE

using namespace Qt::Literals::StringLiterals;

namespace Helper {
    void initSqlRecord(QSqlRecord &record, const QString &tableName,
                       const bool isFirstColAuto = true);
    void setModelColHeaders(QAbstractItemModel *model,
                            std::initializer_list<QString> headers);
    void tryDeleteCurrentRow(QSqlTableModel *model, QAbstractItemView *view);
    bool ifValueExistsInTable(const QSqlDatabase &db,
                              const QLatin1StringView table,
                              const QLatin1StringView column,
                              const QVariant &value, QWidget *msgParent);
    bool ifUsernameExists(const QSqlDatabase &db, const QString &username,
                          QWidget *msgParent);
    bool ifSchoolYearExists(const QSqlDatabase &db, const QString &schoolYear,
                            QWidget *msgParent);
    bool ifStudentIdExists(const QSqlDatabase &db, const int id,
                           QWidget *msgParent);
    QVariant getCurrIdFromComboBox(const QComboBox *combo,
                                   const int colIndex = 0);
}

#endif // HELPER_H
