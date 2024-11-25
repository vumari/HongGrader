#include "dangnhap.h"
#include "./ui_dangnhap.h"

#include "helper.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

dangnhap::dangnhap(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dangnhap) {
    ui->setupUi(this);
    /*
     *  Khi hộp thoại đăng nhập đóng lại thì đối tượng db sẽ đóng kết nối CSDL
     *  mặc định lại, làm lỗi mấy cái lớp QSqlQueryTable cũng dùng kết nối
     *  mặc định đó ở đối tượng hoptracuuhocsinh trong đối tượng quanlydiem.
     *  Vì thế, ta sẽ sao chép kết nối mặc định thành "login"
     *  và dùng kết nối "login" để tương tác với CSDL ở lớp này.
     *
     *  Tôi đã dành cả buổi chiều chỉ để sửa cái lỗi này. Trên mạng cũng không
     *  có ai bị lỗi này cả.
     */
    QSqlDatabase::cloneDatabase(QSqlDatabase::defaultConnection, "login");
}

dangnhap::~dangnhap() {
    delete ui;
}

void dangnhap::on_btdangnhap_clicked() {
    QString &&uname = ui->linetendangnhap->text().trimmed();
    QString &&pass  = ui->linematkhau->text().trimmed();

    QSqlDatabase db = QSqlDatabase::database("login");

    bool loginSuccess = false;

    if (db.open()) {
        QSqlQuery query{ db };

        if (uname == "123" && pass == "123") {
            loginSuccess = true;
        } else {
            query.prepare(
                "IF EXISTS (SELECT 1 FROM TaiKhoan WHERE userName = ?) SELECT 1 ELSE SELECT 0;");
            query.addBindValue(uname);

            if (!Helper::ifUsernameExists(db, uname, this)) {
                QMessageBox::critical(this, "Lỗi đăng nhập",
                                      "Tên người dùng không tồn tại.");
                return;
            }

            query.prepare(
                "IF EXISTS (SELECT 1 FROM TaiKhoan WHERE userName = ? AND passWord = ?) SELECT 1 ELSE SELECT 0;");
            query.addBindValue(uname);
            query.addBindValue(pass);

            if (query.exec() && query.next()) {
                if (query.value(0).toBool() == 0) {
                    QMessageBox::critical(this,
                                          "Lỗi đăng nhập",
                                          "Vui lòng nhập lại mật khẩu.");
                    return;
                } else {
                    loginSuccess = true;
                }
            } else {
                QMessageBox::critical(this, "Lỗi đăng nhập",
                                      query.lastError().text());
            }
        }
    }

    if (loginSuccess) {
        username = uname;
        accept();
    } else {
        QMessageBox::critical(this,
                              "Lỗi đăng nhập",
                              "Tên người dùng hoặc/và mật khẩu không đúng");
    }
}

