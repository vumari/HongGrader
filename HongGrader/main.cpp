#include "dangnhap.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto db = QSqlDatabase::addDatabase("QODBC");

    db.setConnectOptions();
    QString connectionString =
        "DRIVER={ODBC Driver 17 for SQL Server};SERVER=.\\SQLEXPRESS;DATABASE=QLDiemTHPT;Trusted_Connection=yes;";
    db.setDatabaseName(connectionString);
    if (db.open()) {
        db.close();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Lỗi kết nối CSDL");
        msgBox.setInformativeText(
            "Có lỗi khi kết nối với cơ sở dữ liệu: \n" + db.lastError().text());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

    dangnhap w;
    w.show();
    return a.exec();
}
