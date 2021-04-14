#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QPainter>
#include <QDebug>
#include <QFileDialog>
#include <set>
#include <QMessageBox>
#include <QInputDialog>
#include <cmath>
#include <QDesktopServices>

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowTitle("Spelunky 2 FNB Font Creator by PeterSvP");
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, true);

    unicodeSubrange = new UnicodeSubrange(this);
    unicodeSubrange->chars = &charsToAdd;
    unicodeSubrange->lineEdit = ui->charactersToInclude;

    tableEditor = new TableEditor(this);
    tableEditor->mainWindow = this;
    //tableEditor->show();

    ui->frame->installEventFilter(this);
    ui->frame->setFocusPolicy(Qt::ClickFocus);

    //setMouseTracking(true);
    //ui->frame->setMouseTracking(true);
    //ui->centralwidget->setMouseTracking(true);
    //ui->frame->setAttribute(Qt::WA_TransparentForMouseEvents);

    //font.Load("f1.fnb", "f1.png");
    tableEditor->RebuildUI();

    //font.Load("fontfirasans.fnb", "fontfirasans.png");
    //font.Load("fontyorkten.fnb", "fontyorkten.png");

    //font.Load("uc1.fnb", "uc1.png");

    QObjectList targets = {ui->centralwidget, ui->dwFontCreator};
    for(auto o: targets)
    {
        for(auto x: o->findChildren<QSpinBox*>())
            x->setValue( settings.value(x->objectName(), x->value()).toInt());

        for(auto x: o->findChildren<QToolButton*>())
            if(x->isCheckable())
                x->setChecked(settings.value(x->objectName(), x->isChecked()).toBool());

        for(auto x: o->findChildren<QLineEdit*>())
            x->setText(settings.value(x->objectName(), x->text()).toString());

        for(auto x: o->findChildren<QPlainTextEdit*>())
            x->setPlainText(settings.value(x->objectName(), x->toPlainText()).toString());

        for(auto x: o->findChildren<QFontComboBox*>())
        {
            x->setCurrentFont( QFont(settings.value(x->objectName(), x->currentFont().family()).toString()));
            x->setEditText(x->currentFont().family());
        }
    }

    //auto w= ui->dwGlyphInspector->findChildren<QSpinBox*>();
    //for(auto o:w)o->installEventFilter(this);

    restoreGeometry(settings.value("MainWindow-Geometry").toByteArray());
    restoreState(settings.value("MainWindow-State").toByteArray());

    SetCurrentGlyph(nullptr);

    if(font.glyphs.size()>0) ui->splash->hide();
}


void MainWindow::closeEvent(QCloseEvent *e)
{

    // Save all states
    QObjectList targets = {ui->centralwidget, ui->dwFontCreator};
    QStringList bans = {"qt_spinbox_lineedit"};
    for(auto o: targets)
    {
        for(auto x: o->findChildren<QToolButton*>())
            if(x->isCheckable())
                settings.setValue(x->objectName(), x->isChecked());

        for(auto x: o->findChildren<QLineEdit*>())
            settings.setValue(x->objectName(), x->text());

        for(auto x: o->findChildren<QSpinBox*>())
            settings.setValue(x->objectName(), x->value());

        for(auto x: o->findChildren<QPlainTextEdit*>())
            settings.setValue(x->objectName(), x->toPlainText());

        for(auto x: o->findChildren<QFontComboBox*>())
            settings.setValue(x->objectName(), x->currentFont().family());
    }
    for(const auto& b: bans) settings.remove(b);
    settings.setValue("MainWindow-Geometry", saveGeometry());
    settings.setValue("MainWindow-State", saveState());
}

void MainWindow::MousePress(QMouseEvent *e)
{
    if(!ui->frame->geometry().contains(e->pos()))return;

    if(e->button() == Qt::LeftButton)
    {
        for(const auto& r: glyphHitZones)
        {
            QRectF rf = r.second;
            if(r.second.contains(e->pos()))
            {
                //qDebug() << rf << e->pos() << r.second.contains(e->pos());
                qDebug ()  << QChar(r.first->charcode);
                SetCurrentGlyph(r.first);
                update();
                return;
            }
        }
        SetCurrentGlyph(nullptr);
        update();
    }
    else if(e->button() == Qt::RightButton)
    {

            QString s =  QInputDialog::getText(this,"Character To add","Enter character to create/edit");
            if(s.length()==1)
            {
                auto it = font.glyphs.find(s[0].unicode());
                if(it!=font.glyphs.end())
                {
                    SetCurrentGlyph(&it->second);
                    return;
                }

                FnbGlyphInfo src = currentGlyph != nullptr ? *currentGlyph : FnbGlyphInfo{0,0,0,32,32,0,0,0};
                FnbGlyphInfo g;
                g.charcode = s[0].unicode();
                g.x = e->pos().x();
                g.y = e->pos().y();
                g.w = src.w == 0 ? 32 : src.w;
                g.h = src.h == 0 ? 32 : src.h;
                font.glyphs[s[0].unicode()] = g;
                SetCurrentGlyph(&font.glyphs[s[0].unicode()]);
            }
    }
    else if(e->buttons().testFlag(Qt::MiddleButton))
    {
        panInit = e->pos();
        panning = true;
    }
}

void MainWindow::MouseMove(QMouseEvent *e)
{
    if(panning)
    {
        translate += e->pos()-panInit;
        panInit = e->pos();
        update();
    }
}

void MainWindow::MouseRelease(QMouseEvent *e)
{
    panning = false;
}

void MainWindow::KeyboardControls(QKeyEvent *e)
{
    if(currentGlyph)
    {
        bool shift = e->modifiers().testFlag(Qt::ShiftModifier);
        bool ctrl = e->modifiers().testFlag(Qt::ControlModifier);
        bool alt = e->modifiers().testFlag(Qt::AltModifier);
        int x = (e->key()==Qt::Key_Right) - (e->key()==Qt::Key_Left);
        int y = (e->key()==Qt::Key_Down) - (e->key()==Qt::Key_Up);
        qDebug() << "MainWindow::KeyboardControls" << x << y << int(e->key()) << int(Qt::Key_Left);


        auto g = currentGlyph;
        if(shift) {x*=4;y*=4;}
        if(ctrl && alt)
        {
            // Horz. Advancement
            g->horizontalAdvance += x;
            SetCurrentGlyph(g);
            update();
        }
        else if(ctrl)
        {
            //Rezise
            g->w += x;
            g->h += y;
            SetCurrentGlyph(g);
            update();
        }
        else if(alt)
        {
            //bearing/descent
            g->leftBearing += x;
            g->descent += y;
            SetCurrentGlyph(g);
            update();
        }
        else
        {
            //move
            g->x += x;
            g->y += y;
            if(x<0) x=0;
            if(y<0) y=0;
            if(x+g->w > font.texture.width()) x = font.texture.width()-g->w;
            if(y+g->h > font.texture.height()) y = font.texture.height()-g->h;
            SetCurrentGlyph(g);
            update();
        }
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->frame)
    {
        if(event->type() == QEvent::Paint) DrawMainUI((QPaintEvent*)event);
        else if(event->type() == QEvent::MouseButtonPress) MousePress((QMouseEvent*)event);
        else if(event->type() == QEvent::MouseButtonRelease) MouseRelease((QMouseEvent*)event);
        else if(event->type() == QEvent::MouseMove) MouseMove((QMouseEvent*)event);
        else if(event->type() == QEvent::KeyPress) KeyboardControls((QKeyEvent*)event);
        return true;
    }
    return false;
}

void MainWindow::SetCurrentGlyph(FnbGlyphInfo *g)
{
    auto w= ui->dwGlyphInspector->findChildren<QLineEdit*>();
    for(auto o:w)o->blockSignals(true);

    currentGlyph = g;

    ui->glyphInspectorContents->setEnabled(g);
    if(g)
    {
        ui->character->setMaxLength(1);
        ui->character->setText(QChar(g->charcode));
        ui->charPosX->setValue(g->x);
        ui->charPosY->setValue(g->y);
        ui->charWidth->setValue(g->w);
        ui->charHeight->setValue(g->h);
        ui->charLeftBearing->setValue(g->leftBearing);
        ui->charHorizontalAdvance->setValue(g->horizontalAdvance);
        ui->charDescent->setValue(g->descent);
        ui->glyphInspectorSW->setCurrentWidget(ui->pnGlyphInspector);
    }
    else
    {
        ui->glyphInspectorSW->setCurrentWidget(ui->pgNoGlyphSelected);
    }

    for(auto o:w)o->blockSignals(false);

    if(tableEditor->isVisible()) tableEditor->UpdateGlyph(g, true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DrawMainUI(QPaintEvent *e)
{
    QPainter p(ui->frame);
    p.fillRect(ui->frame->rect(),QColor(70,70,70));

    QPoint offset(translate.x(), translate.y());
    //QPoint offset(ui->frame->x(),ui->frame->y());
    p.drawImage(QRect(offset, font.texture.size()*scale) , font.texture);
    p.fillRect(QRect(offset.x(),offset.y(),font.texture.width()*scale,font.texture.height()*scale), QColor(40,40,40,150));

    glyphHitZones.clear();
    for(auto& g: font.glyphs)
    {
        if(translate.x()+(g.second.x+g.second.w)*scale < -20) continue;
        if(translate.x()+g.second.x*scale > ui->frame->width()+20) continue;
        if(translate.y()+(g.second.y+g.second.h)*scale < -20) continue;
        if(translate.y()+g.second.y*scale > ui->frame->height()+20) continue;
        auto r = font.DebugGlyph(p,g.second, offset, scale, currentGlyph == &g.second);
        glyphHitZones[&g.second] = r;
    }

    qDebug() << "MainWindow::DrawMainUI" << ui->previewDrawStringHeight->value();
    font.DrawString(p, ui->lineEdit->text(),
                    QPoint(
                        32,
                        ui->frame->height() * (1-(ui->previewDrawStringHeight->value() / 10000.0))));
}

void MainWindow::wheelEvent(QWheelEvent *e)
{
    ui->zoomPercent->setValue(ui->zoomPercent->value() + sign(e->angleDelta().y())*20);
}

void MainWindow::on_loadFont_clicked()
{
    QString fnb = QFileDialog::getOpenFileName(this,"Select Font FNB", ".", "FNB Font Definition (*.fnb)");
    QString png;
    if(fnb!="")
    {
        QFileInfo info(fnb);
        png = info.path() + "/" + info.completeBaseName() + ".png";
        if(!QFile::exists(png) && png.contains("Data/Fonts"))
            png = png.replace("Data/Fonts","Data/Textures");
        if(!QFile::exists(png))
        {
            png = QFileDialog::getOpenFileName(this,"Select Font Atlas PNG", ".", "PNG Font Atlas (*.png)");
        }
    }

    if(QFile::exists(fnb) && QFile::exists(png))
    {
        font.Load(fnb, png);
        tableEditor->RebuildUI();
        ui->splash->hide();
        SetCurrentGlyph(nullptr);
    }
}

void MainWindow::on_unicodeSubranges_clicked()
{
    unicodeSubrange->show();
}

void MainWindow::on_CreateFont_clicked()
{
    ui->splash->hide();

    // Init font and metrics
    QFont f(ui->fontComboBox->currentFont().family(), ui->fontSize->value());
    f.setBold(ui->fontBold->isChecked());
    f.setItalic(ui->fontItalic->isChecked());
    QFontMetrics m(f);

    // Init atlas and parameters
    int atlasWidth = ui->atlasWidth->value();
    int atlasHeight = ui->atlasHeight->value();
    QImage i(atlasWidth, atlasHeight, QImage::Format_ARGB32);
    QPainter p(&i);

    int spacing = ui->atlasSpacing->value();
    int mx = ui->atlasCharMarginX->value();
    int my = ui->atlasCharMarginY->value();

    // Clear atlas
    p.setCompositionMode(QPainter::CompositionMode_Clear);
    p.fillRect(i.rect(), Qt::red);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);

    // Make font
    font.glyphs.clear();
    p.setPen(Qt::white);
    p.setFont(f);
    QPoint pos(0,0);
    int maxh = 0, realmaxh = 0;

    // Filter and clear characters, remove dupes, sort by height then unicode
    QSet<QChar> chars;
    for(auto c: ui->charactersToInclude->toPlainText()) chars.insert(c);
    QList<QChar> allChars = chars.toList();
    std::sort(allChars.begin(), allChars.end(), [&m](QChar a, QChar b)
    {
        //return a < b;
        int ah = m.boundingRect(a).height();
        int bh = m.boundingRect(b).height();
        if(ah == bh) return a<b;
        return ah > bh;
    });

    // Render glyphs from chars to the atlas
    for(auto c: allChars)
    {
        auto r = m.boundingRect(c); // can go out of range
        r.adjust(-mx, -my, mx, my);
        auto nr = r.translated(-r.topLeft() + pos);
        qDebug () << nr;
        int w = r.width();
        int h = r.height();
        if(h>maxh) maxh=h;
        if(h>realmaxh) realmaxh=h;
        if(pos.rx()+r.width()+spacing > atlasWidth)
        {
            pos.rx()=0;
            pos.ry()+= maxh+spacing;
            maxh=h;
        }
        nr = r.translated(-r.topLeft() + pos);
        //if(pos.ry() + r.height() > atlasHeight)
        //{
        //    ui->atlasHeight->stepUp();
        //    on_CreateFont_clicked();
        //    return;
        //}

        //p.drawRect(nr);
        p.drawText(pos - r.topLeft(), c);
        pos.rx() += r.width()+spacing;
        FnbGlyphInfo g;
        g.charcode = c.unicode();
        g.x = nr.x();
        g.y = nr.y();
        g.w = nr.width();
        g.h = nr.height();
        g.leftBearing = m.leftBearing(c) + mx;
        g.horizontalAdvance = m.horizontalAdvance(c);
        g.descent = r.top() + m.descent() + ui->perCharacterAdditionalVoffset->value();
        font.glyphs[c.unicode()] = g;
        //font.baseline = realmaxh/2;
        //ui->fontBaseline->setValue(font.baseline);
    }
    font.texture = i;
    repaint();
}

void MainWindow::on_saveFont_clicked()
{
    QString fnb = QFileDialog::getSaveFileName(this,"Select Font FNB", ".", "FNB Font Definition (*.fnb)");
    QFileInfo info(fnb);
    QString png = info.path() + "/" + info.completeBaseName() + ".png";
    if(fnb.contains("Mods/Packs") && fnb.contains("Data/Fonts"))
        png.replace("Data/Fonts", "Data/Textures");
    font.baseline = ui->fontBaseline->value();
    font.SaveFNB(fnb);
    font.texture.save(png);
}

void MainWindow::UpdateFontPreview(QFont f)
{
    QFont f2(f); f2.setPointSize(12);
    f2.setBold(ui->fontBold->isChecked());
    f2.setItalic(ui->fontItalic->isChecked());
    ui->charactersToInclude->setFont(f2);
}
void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    UpdateFontPreview(f);
}

void MainWindow::on_fontBold_clicked(bool checked)
{
    UpdateFontPreview(ui->fontComboBox->currentFont());
}

void MainWindow::on_fontItalic_clicked(bool checked)
{
    UpdateFontPreview(ui->fontComboBox->currentFont());
}

void MainWindow::on_character_textChanged(const QString &arg1)
{
    return;
    if(!currentGlyph) return;
    if(arg1.size()!=1) return;
    currentGlyph->charcode = arg1[0].unicode();
    update();
}

void MainWindow::on_charPosX_valueChanged(int arg1)
{
    if(!currentGlyph) return;
    currentGlyph->x = arg1;
    SetCurrentGlyph(currentGlyph);
    update();
}

void MainWindow::on_charPosY_valueChanged(int arg1)
{
    if(!currentGlyph) return;
    currentGlyph->y = arg1;
    SetCurrentGlyph(currentGlyph);
    update();
}

void MainWindow::on_charWidth_valueChanged(int arg1)
{
    if(!currentGlyph) return;
    currentGlyph->w = arg1;
    SetCurrentGlyph(currentGlyph);
    update();
}

void MainWindow::on_charHeight_valueChanged(int arg1)
{
    if(!currentGlyph) return;
    currentGlyph->h = arg1;
    SetCurrentGlyph(currentGlyph);
    update();
}

void MainWindow::on_charLeftBearing_valueChanged(int arg1)
{
    if(!currentGlyph) return;
    currentGlyph->leftBearing = arg1;
    SetCurrentGlyph(currentGlyph);
    update();
}

void MainWindow::on_charHorizontalAdvance_valueChanged(int arg1)
{
    if(!currentGlyph) return;
    currentGlyph->horizontalAdvance = arg1;
    SetCurrentGlyph(currentGlyph);
    update();
}

void MainWindow::on_charDescent_valueChanged(int arg1)
{
    if(!currentGlyph) return;
    currentGlyph->descent = arg1;
    SetCurrentGlyph(currentGlyph);
    update();
}

void MainWindow::on_newFont_clicked()
{
    if(QMessageBox::question(this, "Instant Restart?",
                             "Sure? Just to avoid loss of work!",
                             QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::Yes)
    {
        font = FnbFont();
        ui->splash->show();
        translate = QPoint(32,32);
        update();
        tableEditor->RebuildUI();
    }
}

void MainWindow::on_loadPNGAtlas_clicked()
{
    QString png = QFileDialog::getOpenFileName(this,"Select Font Atlas PNG", ".", "PNG Font Atlas (*.png)");
    if(QFile::exists(png))
    {
        font.texture = QImage(png);
        ui->splash->hide();
    }
}

void MainWindow::on_zoomPercent_valueChanged(int arg1)
{
    scale = arg1/100.0;
    update();
}

void MainWindow::on_zoom100_clicked()
{
    ui->zoomPercent->setValue(100);
    update();
}

void MainWindow::on_previewDrawStringHeight_sliderMoved(int position)
{
    update();
}

void MainWindow::on_delGlyph_clicked()
{
    if(QMessageBox::question(this, "Del Glyph?",
                             "Sure? Just to avoid loss of work!\n"
                             "(If you touch DEL, I won't ask again)",
                             QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::Yes)
    {
        font.glyphs.erase(currentGlyph->charcode);
        SetCurrentGlyph(nullptr);
    }
}

void MainWindow::on_copyGlyph_clicked()
{
    QString s =  QInputDialog::getText(this,"Character To add","Enter character to create/edit");
    if(s.length()==1)
    {
        auto it = font.glyphs.find(s[0].unicode());
        if(it!=font.glyphs.end())
        {
            QMessageBox::information(this, "Glyph already exists", "Cannot copy over existing glyph!");
            return;
        }
        font.glyphs[s[0].unicode()] = *currentGlyph;
        font.glyphs[s[0].unicode()].charcode = s[0].unicode();
        SetCurrentGlyph(&font.glyphs[s[0].unicode()]);
    }
}

void MainWindow::on_SplashLabel_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(link);
}

void MainWindow::on_tableEditor_clicked()
{
    tableEditor->show();
}
