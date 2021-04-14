#include "mainwindow.h"
#include "unicodesubrange.h"

#include <QApplication>
#include "ui_mainwindow.h"
#include "tableeditor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Spelunky 2 FNB Font Creator");
    a.setOrganizationName("spelunky.fyi");
    MainWindow w;
    w.show();

    return a.exec();
}
