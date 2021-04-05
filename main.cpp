#include "mainwindow.h"
#include "unicodesubrange.h"

#include <QApplication>
#include "ui_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Spelunky 2 FNB Font Creator");
    a.setOrganizationName("spelunky.fyi");
    MainWindow w;
    w.show();
    UnicodeSubrange u(&w);
    u.chars = &w.charsToAdd;
    w.subrangePopup = &u;
    u.lineEdit = w.ui->charactersToInclude;
    //u.show();
    return a.exec();
}
