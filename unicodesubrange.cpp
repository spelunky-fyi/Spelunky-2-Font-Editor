#include "unicodesubrange.h"
#include "ui_unicodesubrange.h"

#include <QCheckBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>

UnicodeSubrange::UnicodeSubrange(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UnicodeSubrange)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    QStringList lines;

    QFile f("Subranges.txt");
    if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream s(&f);
        s.setCodec("UTF-8");
        while(!s.atEnd())
            lines.append(s.readLine());
    }
    for(const auto& l:lines)
    {
        auto cb = new QCheckBox();
        cb->setText(l);
        connect(cb, &QCheckBox::clicked, [this,l,cb]{
            QStringList parts = l.split("\t");
            parts = parts[0].split("-");
            int begin, end;
            begin = parts[0].toUInt(nullptr, 16);
            end = parts[1].toUInt(nullptr, 16);
            //qDebug() << begin <<" - " << end;

            for(int i=begin; i<=end;i++)
                if(cb->isChecked())
                    this->chars->insert(i);
                else this->chars->erase(i);
            QString r = "";
            for(QChar c: *this->chars) r+=c;
            this->lineEdit->setPlainText(r);
        });
        ui->scrollAreaWidgetContents->layout()->addWidget(cb);
    }
}

UnicodeSubrange::~UnicodeSubrange()
{
    delete ui;
}
