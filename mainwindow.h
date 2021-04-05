#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include <QSettings>
#include <unicodesubrange.h>
#include "fnbfont.h"
#include <unordered_map>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    FnbFont font;
    std::set<QChar> charsToAdd;
    UnicodeSubrange* subrangePopup;
    QSettings settings;
    std::unordered_map<FnbGlyphInfo*, QRectF> glyphHitZones;

    float scale = 1;
    QPoint translate;
    FnbGlyphInfo* currentGlyph = nullptr;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent* e);
    void wheelEvent(QWheelEvent* e);
    void closeEvent(QCloseEvent* e);

    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

    void SetCurrentGlyph(FnbGlyphInfo* g);

    void UpdateFontPreview(QFont f);

private slots:
    void on_loadFont_clicked();

    void on_unicodeSubranges_clicked();

    void on_CreateFont_clicked();

    void on_saveFont_clicked();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_fontBold_clicked(bool checked);
    void on_fontItalic_clicked(bool checked);

    void on_character_textChanged(const QString &arg1);

    void on_charPosX_valueChanged(int arg1);

    void on_charPosY_valueChanged(int arg1);

    void on_charWidth_valueChanged(int arg1);

    void on_charHeight_valueChanged(int arg1);

    void on_charLeftBearing_valueChanged(int arg1);

    void on_charHorizontalAdvance_valueChanged(int arg1);

    void on_charDescent_valueChanged(int arg1);

    void on_newFont_clicked();

    void on_loadPNGAtlas_clicked();

public:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
