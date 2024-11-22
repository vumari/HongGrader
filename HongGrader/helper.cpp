#include "helper.h"

#include <QSqlField>
#include <QAbstractItemModel>
#include <QTableView>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

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

    void tryDeleteCurrentRow(QSqlTableModel *model, QTableView *view) {
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

    bool ifUsernameExists(const QSqlDatabase &db, const QString &username,
                          QWidget *msgParent) {
        QSqlQuery query{ db };

        query.prepare(
            "IF EXISTS (SELECT 1 FROM TaiKhoan WHERE userName = ?) SELECT 1 ELSE SELECT 0;");
        query.addBindValue(username);

        if (query.exec() && query.next()) {
            return query.value(0).toBool();
        } else {
            QMessageBox::critical(msgParent, "Lỗi truy vấn CSDL",
                                  query.lastError().text());
        }
        return false;
    }
}
