#ifndef TABLEEXPORTER_H
#define TABLEEXPORTER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QAbstractTableModel;
QT_END_NAMESPACE

class TableExporter
{
public:
    TableExporter(QAbstractTableModel *newTable, QWidget *w = nullptr);

    bool startExport();

private:
    QAbstractTableModel *model = nullptr;
    QWidget *widget            = nullptr;

    bool exportToCsv(const QString &filePath, const bool excelCompat);

    static QString quoteString(const QVariant &data);
};

#endif // TABLEEXPORTER_H
