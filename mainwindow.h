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

    float scale = 1, previewSliderPercent = 0.8;
    QPoint translate = QPoint(32,10), panInit;
    bool panning = false;
    FnbGlyphInfo* currentGlyph = nullptr;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void DrawMainUI(QPaintEvent* e);
    void wheelEvent(QWheelEvent* e);
    void closeEvent(QCloseEvent* e);

    void MousePress(QMouseEvent* e);
    void MouseMove(QMouseEvent* e);
    void MouseRelease(QMouseEvent* e);
    void KeyboardControls(QKeyEvent* e);

    bool eventFilter(QObject *watched, QEvent *event);

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

    void on_zoomPercent_valueChanged(int arg1);

    void on_zoom100_clicked();

    void on_previewDrawStringHeight_sliderMoved(int position);

    void on_delGlyph_clicked();

    void on_copyGlyph_clicked();

    void on_SplashLabel_linkActivated(const QString &link);

public:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
