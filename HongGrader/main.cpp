#include "dangnhap.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dangnhap w;
    w.show();
    return a.exec();
}
