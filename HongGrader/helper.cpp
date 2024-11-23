#include "helper.h"

#include <QSqlField>
#include <QAbstractItemModel>
#include <QTableView>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QComboBox>

namespace Helper {
    void initSqlRecord(QSqlRecord &record, const QString &tableName,
                       const bool isFirstColAuto) {
        for (int i = 0; i < record.count(); ++i) {
            QSqlField &&field = record.field(i);
            if (i == 0) {
                field.setAutoValue(isFirstColAuto);
            }
            field.setTableName("GiaoVien");
            record.replace(i, field);
        }
    }

    void setModelColHeaders(QAbstractItemModel *model,
                            std::initializer_list<QString> headers) {
        int i = 0;

        for (const QString &header: headers) {
            if (!header.isEmpty()) {
                model->setHeaderData(i, Qt::Horizontal, header);
            }
            ++i;
        }
    }

    void tryDeleteCurrentRow(QSqlTableModel *model, QAbstractItemView *view) {
        const auto *selectionModel = view->selectionModel();

        if (selectionModel->hasSelection()) {
            if (!model->removeRow(selectionModel->currentIndex().row())
                || !model->submitAll()) {
                QMessageBox::critical(view,
                                      "Lỗi xoá dòng",
                                      model->lastError().text());
                model->revertAll();
            }
        }
    }

    bool ifValueExistsInTable(const QSqlDatabase &db,
                              const QLatin1StringView table,
                              const QLatin1StringView column,
                              const QVariant &value, QWidget *msgParent) {
        const static QLatin1StringView queryTemplate{
            "IF EXISTS (SELECT 1 FROM %1 WHERE %2 = ?) SELECT 1 ELSE SELECT 0;" };

        QSqlQuery query{ db };

        query.prepare(queryTemplate.arg(table, column));
        query.addBindValue(value);

        if (query.exec() && query.next()) {
            return query.value(0).toBool();
        } else {
            QMessageBox::critical(msgParent, "Lỗi truy vấn CSDL",
                                  query.lastError().text());
        }
        return false;
    }

    bool ifUsernameExists(const QSqlDatabase &db, const QString &username,
                          QWidget *msgParent) {
        return ifValueExistsInTable(db, "TaiKhoan"_L1, "userName"_L1,
                                    username, msgParent);
    }

    QVariant getCurrIdFromComboBox(const QComboBox *combo, const int colIndex) {
        return combo->model()->index(combo->currentIndex(), 0).data();
    }
}
