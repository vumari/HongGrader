#include "tableexporter.h"

#include <QModelIndex>
#include <QMessageBox>
#include <QFileDialog>
#include <QSaveFile>
#include <QTextStream>

using namespace Qt::Literals::StringLiterals;

const static QLatin1StringView _quotedString{ R"("%1")" };
const static QString           filters =
    u"CSV ngăn cách bằng dấu phẩy (*.csv);;CSV tương thích với Excel (*.csv)"_s;


TableExporter::TableExporter(QAbstractTableModel *newTable, QWidget *p)
    : model{newTable}, widget{p} {
}

bool TableExporter::startExport() {
    if (model->rowCount() == 0) {
        QMessageBox::critical(widget, "Lỗi xuất tệp",
                              "Bảng này phải có ít nhất 1 hàng.");
        return false;
    }
    if (model->columnCount() == 0) {
        QMessageBox::critical(widget, "Lỗi xuất tệp",
                              "Bảng này phải có ít nhất 1 cột.");
        return false;
    }

    QString selectedFilter =
        u"CSV tương thích với Excel (*.csv)"_s;
    const QString &&filePath = QFileDialog::getSaveFileName(
        widget, "Chọn vị trí xuất tệp", QString(),
        filters, &selectedFilter);

    if (filePath.isEmpty()) {
        return false;
    }
    if (selectedFilter.startsWith("CSV"_L1)) {
        bool excelCompat = selectedFilter.contains("Excel"_L1);
        return exportToCsv(filePath, excelCompat);
    }
    return false;
}

bool TableExporter::exportToCsv(const QString &filePath,
                                const bool excelCompat) {
    QString     output;
    QTextStream tout(&output, QTextStream::WriteOnly);

    tout.setRealNumberPrecision(2);
    bool forceUtf8 = false;

    const auto &&rootIndex = QModelIndex();
    if (model->canFetchMore(rootIndex)) {
        model->fetchMore(rootIndex);
    }

    const int rows = model->rowCount();
    const int cols = model->columnCount();

    QChar sep = excelCompat ? ';' : ',';
    for (int i = 0; i < cols; ++i) {
        if (i > 0) {
            tout << sep;
        }
        tout << quoteString(model->headerData(i, Qt::Horizontal));
    }
    if (excelCompat) {
        forceUtf8 = true;
        tout.setLocale(QLocale(QLocale::Vietnamese));
    }
    for (int i = 0; i < rows; ++i) {
        tout << '\n';
        for (int j = 0; j < cols; ++j) {
            if (j > 0) {
                tout << sep;
            }
            QVariant &&data = model->index(i, j).data();
            if (!data.isNull()) {
                switch (data.metaType().id()) {
                    case QMetaType::Float: {
                        tout << data.toFloat();
                        break;
                    }
                    case QMetaType::Double: {
                        tout << data.toDouble();
                        break;
                    }
                    case QMetaType::Int:
                    case QMetaType::Long:
                    case QMetaType::LongLong:
                    case QMetaType::UInt:
                    case QMetaType::ULong:
                    case QMetaType::ULongLong: {
                        tout << data.toString();
                        break;
                    }
                    case QMetaType::QString: {
                        tout << quoteString(data);
                        break;
                    }
                    default: {
                        tout << data.toString();
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
            QMessageBox::critical(widget, "Lỗi lưu tệp",
                                  data.errorString());
            return false;
        }
    } else {
        QMessageBox::critical(widget, "Lỗi lưu tệp", data.errorString());
        return false;
    }
    return true;
}

QString TableExporter::quoteString(const QVariant &data) {
    return _quotedString.arg(data.toString().replace('"', R"("")"));
}
