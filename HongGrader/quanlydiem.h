#ifndef QUANLYDIEM_H
#define QUANLYDIEM_H

#include <QMainWindow>

class ScoreModel;

namespace Ui {
    class quanlydiem;
}
class dangnhap;
class quanlydiem : public QMainWindow
{
    Q_OBJECT

public:
    explicit quanlydiem(QWidget *parent = nullptr);
    ~quanlydiem();

private slots:
    void login();

    void on_actionLog_out_triggered();
    void on_actiongiaovien_triggered();
    void on_actionhocsinh_triggered();
    void on_actionlop_triggered();
    void on_actionEdit_triggered();
    void on_actionscoreboard_triggered();
    void on_actionstudentlist_triggered();
    void on_actionnamhoc_2_triggered();

    void on_BTsua_clicked();
    void on_BThuy_clicked();
    void on_BTluu_clicked();

    void setupTable();
    bool checkValidInputs();
    void onAddRow();
    void onDeleteCurrentRow();

private:
    Ui::quanlydiem *ui;

    ScoreModel *model = nullptr;
    QString username;
};

#endif // QUANLYDIEM_H
