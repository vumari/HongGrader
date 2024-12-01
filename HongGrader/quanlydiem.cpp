#include "quanlydiem.h"
#include "ui_quanlydiem.h"

#include "quanlygiaovien.h"
#include "quanlyhocsinh.h"
#include "quanlylophoc.h"
#include "quanlytaikhoan.h"
#include "thongkediem.h"
#include "thongkehocsinh.h"
#include "quanlynamhoc.h"
#include "dangnhap.h"
#include "helper.h"
#include "scoremodel.h"
#include "scoreitemdelegate.h"

#include <QTimer>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QItemSelectionModel>
#include <QMouseEvent>


quanlydiem::quanlydiem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::quanlydiem) {
    ui->setupUi(this);
    ui->BThuy->hide();
    ui->BTluu->hide();

    if (!QSqlDatabase::contains("main")) {
        QSqlDatabase::cloneDatabase(QSqlDatabase::defaultConnection, "main");
    }

    QSqlDatabase db = QSqlDatabase::database("main");

    if (db.open()) {
        ui->groupBox_HS->init(db);

        QSqlQueryModel *schoolYearModel = new QSqlQueryModel(this);
        schoolYearModel->setQuery("SELECT * FROM NamHoc", db);
        ui->CBnamhoc->setModel(schoolYearModel);

        QSqlQueryModel *subjectModel = new QSqlQueryModel(this);
        subjectModel->setQuery("SELECT * FROM Mon", db);
        ui->CBmonhoc->setModel(subjectModel);
        ui->CBmonhoc->setModelColumn(1);
    }

    model = new ScoreModel(this);
    model->select();
    ui->tablediem->setModel(model);
    ui->tablediem->setItemDelegate(new ScoreItemDelegate(ui->tablediem));
    ui->tablediem->viewport()->installEventFilter(this);
    setupTable();

    connect(ui->BTthem, &QPushButton::clicked, this, &quanlydiem::onAddRow);
    connect(ui->BTxoa, &QPushButton::clicked,
            this, &quanlydiem::onDeleteCurrentRow);

    QTimer::singleShot(1, this, &quanlydiem::login);
}

quanlydiem::~quanlydiem() {
    delete ui;
}

bool quanlydiem::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (obj->parent() != ui->tablediem) {
            return QObject::eventFilter(obj, event);
        }
        if (mouseEvent->button() != Qt::RightButton) {
            return QObject::eventFilter(obj, event);
        }
        if (ui->tablediem->editTriggers() ==
            QAbstractItemView::NoEditTriggers) {
            return QObject::eventFilter(obj, event);
        }
        QModelIndex index = ui->tablediem->indexAt(mouseEvent->pos());
        if (index.isValid()) {
            const int col = index.column();
            if (((col >= 7) && (col <= 7 + 5)) || (col == 14)) {
                model->setData(index, QVariant(QMetaType::fromType<float>()));
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

void quanlydiem::login() {
    dangnhap formDangNhap(this);
    int      result = formDangNhap.exec();

    if (result == QDialog::Accepted) {
        username = formDangNhap.username;
    } else {
        qApp->quit();
    }
}

void quanlydiem::on_actionLog_out_triggered() {
    login();
}


void quanlydiem::on_actiongiaovien_triggered() {
    (new quanlygiaovien(this))->show();
}


void quanlydiem::on_actionhocsinh_triggered() {
    (new quanlyhocsinh(this))->show();
}


void quanlydiem::on_actionlop_triggered() {
    (new quanlylophoc(this))->show();
}


void quanlydiem::on_actionEdit_triggered() {
    (new quanlytaikhoan(this))->show();
}


void quanlydiem::on_actionscoreboard_triggered() {
    (new thongkediem(this))->show();
}


void quanlydiem::on_actionstudentlist_triggered() {
    (new thongkehocsinh(this))->show();
}

void quanlydiem::on_actionnamhoc_2_triggered() {
    (new quanlynamhoc(this))->show();
}


void quanlydiem::on_BTsua_clicked() {
    ui->BTsua->hide();
    ui->BThuy->show();
    ui->BTluu->show();
    ui->BTthem->setDisabled(true);
    ui->groupBox->setDisabled(true);
    ui->BTxoa->setDisabled(true);
    ui->tablediem->setEditTriggers(
        QTableView::DoubleClicked | QTableView::SelectedClicked |
        QTableView::EditKeyPressed);
}


void quanlydiem::on_BThuy_clicked() {
    model->select();

    ui->BThuy->hide();
    ui->BTluu->hide();
    ui->BTsua->show();
    ui->BTthem->setDisabled(false);
    ui->groupBox->setDisabled(false);
    ui->BTxoa->setDisabled(false);
    ui->tablediem->setEditTriggers(QTableView::NoEditTriggers);
}


void quanlydiem::on_BTluu_clicked() {
    model->submitAll();

    ui->BTluu->hide();
    ui->BThuy->hide();
    ui->BTsua->show();
    ui->BTthem->setDisabled(false);
    ui->groupBox->setDisabled(false);
    ui->BTxoa->setDisabled(false);
    ui->tablediem->setEditTriggers(QTableView::NoEditTriggers);
}

void quanlydiem::setupTable() {
    ui->tablediem->horizontalHeader()->resizeSections(
        QHeaderView::ResizeToContents);
    ui->tablediem->hideColumn(2);
    ui->tablediem->hideColumn(4);
}

bool quanlydiem::checkValidInputs() {
    const int studentId = ui->SBmaHS->value();

    if (!Helper::ifStudentIdExists(model->database(), studentId, this)) {
        QMessageBox::critical(this, "Lỗi nhập liệu",
                              "Mã học sinh không tồn tại.");
        ui->SBmaHS->setFocus(Qt::OtherFocusReason);
        return false;
    }

    return true;
}

void quanlydiem::onAddRow() {
    if (!checkValidInputs()) {
        return;
    }

    model->appendRow(ui->SBmaHS->value(),
                     Helper::getCurrIdFromComboBox(ui->CBmonhoc).toString(),
                     ui->RBHK1->isChecked() ? 1 : 2,
                     ui->CBnamhoc->currentText());
    setupTable();
}

void quanlydiem::onDeleteCurrentRow() {
    auto *selectionModel = ui->tablediem->selectionModel();

    if (selectionModel->hasSelection()) {
        model->removeRow(selectionModel->currentIndex().row());
        setupTable();
    }
}
