#ifndef HELPER_H
#define HELPER_H

#include <QSqlDatabase>
#include <QSqlRecord>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QSqlTableModel;
class QTableView;
QT_END_NAMESPACE

namespace Helper {
    void initSqlRecord(QSqlRecord &record, const QString &tableName,
                       const bool isFirstColAuto = true);
    void setModelColHeaders(QAbstractItemModel *model,
                            std::initializer_list<QString> headers);
    void tryDeleteCurrentRow(QSqlTableModel *model, QTableView *view);
    bool ifUsernameExists(const QSqlDatabase &db,
                          const QString &username,
                          QWidget *msgParent);
}

#endif // HELPER_H
