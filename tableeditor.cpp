#include "tableeditor.h"
#include "ui_tableeditor.h"
#include "mainwindow.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>

TableEditor::TableEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableEditor)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(0);
    prototypeItem = new QTableWidgetItem();
    prototypeItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItemPrototype(prototypeItem);
    restoreGeometry(settings.value("TableEditor-Geometry").toByteArray());
}

TableEditor::~TableEditor()
{
    delete ui;
}

void TableEditor::RebuildUI()
{
    QSignalBlocker blocker(ui->tableWidget);

    ui->tableWidget->setRowCount(mainWindow->font.glyphs.size());
    int row=0;
    //ui->tableWidget->setSortingEnabled(false);
    for(const auto& x: mainWindow->font.glyphs)
    {
        int col = 0;
        itemAt(row, col++)->setText(QChar(x.second.charcode));
        itemAt(row, col++)->setText(QString::number(x.second.x));
        itemAt(row, col++)->setText(QString::number(x.second.y));
        itemAt(row, col++)->setText(QString::number(x.second.w));
        itemAt(row, col++)->setText(QString::number(x.second.h));
        itemAt(row, col++)->setText(QString::number(x.second.leftBearing));
        itemAt(row, col++)->setText(QString::number(x.second.descent));
        itemAt(row, col++)->setText(QString::number(x.second.horizontalAdvance));
        row++;
    }
}

void TableEditor::UpdateGlyph(FnbGlyphInfo *glyph, bool select)
{    
    if(!glyph) return;
    QSignalBlocker blocker(ui->tableWidget);

    int row = -1;
    int items = ui->tableWidget->rowCount();
    for(int i=0; i<items;i++)
    {
        if(itemAt(i,0)->text().at(0).unicode() == glyph->charcode)
        {
            row = i;
        }
    }
    if(row!=-1)
    {
        qDebug() << "TableEditor::UpdateGlyph" << itemAt(row,0)->text();
        int col = 0;
        itemAt(row, col)->setData(Qt::UserRole, QChar(glyph->charcode));
        itemAt(row, col++)->setText(QChar(glyph->charcode));
        itemAt(row, col++)->setText(QString::number(glyph->x));
        itemAt(row, col++)->setText(QString::number(glyph->y));
        itemAt(row, col++)->setText(QString::number(glyph->w));
        itemAt(row, col++)->setText(QString::number(glyph->h));
        itemAt(row, col++)->setText(QString::number(glyph->leftBearing));
        itemAt(row, col++)->setText(QString::number(glyph->descent));
        itemAt(row, col++)->setText(QString::number(glyph->horizontalAdvance));

        int cc = ui->tableWidget->currentColumn();
        if(select) ui->tableWidget->setCurrentCell(row, cc==-1?0:cc);
    }
}

void TableEditor::on_tableWidget_cellChanged(int row, int column)
{
    if(!mainWindow->currentGlyph) return;
    auto item = itemAt(row, column);
    auto val = item->text().toInt();
    switch (column) {
    case 0:
    {
        QChar to = item->text()[0];
        auto g = mainWindow->font[to.unicode()];
        uint32_t from = item->data(Qt::UserRole).toChar().unicode();
        if(from!=to)
        {
            if(g!=nullptr)
            {
                QMessageBox::information(this, "Glyph already exists", "Cannot change glyph's code point, because such code point already exists in the font");
            }
            else
            {
                FnbGlyphInfo fg = *mainWindow->font[from];
                mainWindow->font.glyphs.erase(from);
                fg.charcode = to.unicode();
                mainWindow->font.glyphs[to.unicode()] = fg;
                RebuildUI();
                mainWindow->SetCurrentGlyph(mainWindow->font[to.unicode()]);
                QTimer::singleShot(0, [to, this]{
                    auto nto = mainWindow->font[to.unicode()];
                    UpdateGlyph(nto, true);
                    mainWindow->update();
                });
                return;
            }
        }
        break;
    }
    case 1: mainWindow->currentGlyph->x = val; break;
    case 2: mainWindow->currentGlyph->y = val; break;
    case 3: mainWindow->currentGlyph->w = val; break;
    case 4: mainWindow->currentGlyph->h = val; break;
    case 5: mainWindow->currentGlyph->leftBearing = val; break;
    case 6: mainWindow->currentGlyph->descent = val; break;
    case 7: mainWindow->currentGlyph->horizontalAdvance = val; break;
    }
    mainWindow->SetCurrentGlyph(mainWindow->currentGlyph);
}

QTableWidgetItem *TableEditor::itemAt(int row, int col)
{
    auto item = ui->tableWidget->item(row, col);
    if(!item)
    {
        auto item = prototypeItem->clone();
        if(col == 0)
        {
            item->setBackgroundColor(QColor(0,0,0,50));
        }
        ui->tableWidget->setItem(row, col, item);
        return item;
    }
    else return item;
}

void TableEditor::closeEvent(QCloseEvent *e)
{
    //settings.setValue("TableEditor-Geometry", saveGeometry());
}

void TableEditor::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    int cp = itemAt(currentRow,0)->text()[0].unicode();
    for(auto& g: mainWindow->font.glyphs)
    {
        if(g.second.charcode == cp)
            mainWindow->SetCurrentGlyph(&g.second);
    }

}

void TableEditor::on_scaleAllGlyphs_clicked()
{
    bool ok;
    float f = QInputDialog::getDouble(this, "Scale Font by percentage", "Input the percentage to scale the font", 200, 1, 10000, 0, &ok);
    f/=100;
    if(ok)
    {
        for(auto& g: mainWindow->font.glyphs)
        {
            FnbGlyphInfo& i = g.second;
            i.x *= f;
            i.y *= f;
            i.w *= f;
            i.h *= f;
            i.horizontalAdvance *= f;
            i.descent *= f;
            i.leftBearing *= f;
        }
        RebuildUI();
        mainWindow->update();
        mainWindow->SetCurrentGlyph(mainWindow->currentGlyph);
    }
}

void TableEditor::on_bearingDec_clicked()
{
    for(auto& g: mainWindow->font.glyphs)
    {
        g.second.leftBearing--;
    }
    RebuildUI();
    mainWindow->update();
    mainWindow->SetCurrentGlyph(mainWindow->currentGlyph);

}

void TableEditor::on_bearingInc_clicked()
{
    for(auto& g: mainWindow->font.glyphs)
    {
        g.second.leftBearing++;
    }
    RebuildUI();
    mainWindow->update();
    mainWindow->SetCurrentGlyph(mainWindow->currentGlyph);

}

void TableEditor::on_advanceDec_clicked()
{
    for(auto& g: mainWindow->font.glyphs)
    {
        g.second.horizontalAdvance--;
    }
    RebuildUI();
    mainWindow->update();
    mainWindow->SetCurrentGlyph(mainWindow->currentGlyph);

}

void TableEditor::on_advanceInc_clicked()
{
    for(auto& g: mainWindow->font.glyphs)
    {
        g.second.horizontalAdvance++;
    }
    RebuildUI();
    mainWindow->update();
    mainWindow->SetCurrentGlyph(mainWindow->currentGlyph);
}

void TableEditor::on_autoFillAdvancement_clicked()
{
    for(auto& g: mainWindow->font.glyphs)
    {
        if(g.second.horizontalAdvance == 0)
            g.second.horizontalAdvance = g.second.w;
    }
    RebuildUI();
    mainWindow->update();
    mainWindow->SetCurrentGlyph(mainWindow->currentGlyph);

}
