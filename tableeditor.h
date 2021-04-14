#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H

#include <QDialog>
#include <QSettings>
#include <QTableWidget>
#include "fnbfont.h"

class MainWindow;

namespace Ui {
class TableEditor;
}

class TableEditor : public QDialog
{
    Q_OBJECT

public:
    explicit TableEditor(QWidget *parent = nullptr);
    ~TableEditor();
    MainWindow* mainWindow;
    QTableWidgetItem* prototypeItem;

    void RebuildUI();
    void UpdateGlyph(FnbGlyphInfo* glyph, bool select);

private slots:
    void on_tableWidget_cellChanged(int row, int column);

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_scaleAllGlyphs_clicked();

    void on_bearingDec_clicked();

    void on_bearingInc_clicked();

    void on_advanceDec_clicked();

    void on_advanceInc_clicked();

    void on_autoFillAdvancement_clicked();

private:
    QTableWidgetItem* itemAt(int row, int col);
    QSettings settings;
    Ui::TableEditor *ui;

    void closeEvent(QCloseEvent* e);

};

#endif // TABLEEDITOR_H
