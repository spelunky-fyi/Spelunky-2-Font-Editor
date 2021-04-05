#ifndef UNICODESUBRANGE_H
#define UNICODESUBRANGE_H

#include <QDialog>
#include <QPlainTextEdit>
#include <set>

namespace Ui {
class UnicodeSubrange;
}

class UnicodeSubrange : public QDialog
{
    Q_OBJECT


public:
    std::set<QChar>* chars = nullptr;
    QPlainTextEdit* lineEdit;
    explicit UnicodeSubrange(QWidget *parent = nullptr);
    ~UnicodeSubrange();
    Ui::UnicodeSubrange *ui;
};

#endif // UNICODESUBRANGE_H
