#include "fnbfont.h"

#include <QFile>
#include <QDebug>

QFont dbgFont;

FnbFont::FnbFont()
{
    // only for debugging
    dbgFont = QFont();
    dbgFont.setPointSize(12);
}

void FnbFont::Load(QString fnbFile, QString PNGFile)
{
    texture.load(PNGFile);
    QByteArray fnb;
    QFile f(fnbFile);
    f.open(QIODevice::ReadOnly);
    fnb = f.readAll();
    ParseFNB(fnb);
}

void FnbFont::SaveFNB(QString fnbFile)
{
    QFile f(fnbFile);
    f.open(QIODevice::WriteOnly);
    QDataStream d(&f);
    // Header 0x01, baseline[u16], 1/width[f32], 1/height[f32]
    d.writeRawData("\x01", 1);
    d.writeRawData((char*)&baseline, 2);

    float w = 1.0f/texture.width();
    float h = 1.0f/texture.height();
    d.writeRawData((char*)&w, 4);
    d.writeRawData((char*)&h, 4);

    for(const auto& e: glyphs)
    {
        d.writeRawData("\x03", 1);  // entry header 0x03
        d.writeRawData( (char*)&e.second.charcode, 4);
        d.writeRawData( (char*)&e.second.x, 2);
        d.writeRawData( (char*)&e.second.y, 2);
        d.writeRawData( (char*)&e.second.w, 2);
        d.writeRawData( (char*)&e.second.h, 2);
        d.writeRawData( (char*)&e.second.leftBearing, 2);
        d.writeRawData( (char*)&e.second.descent, 2);
        d.writeRawData( (char*)&e.second.horizontalAdvance, 2);
    }
}

void FnbFont::ParseFNB(const QByteArray bytes)
{
    glyphs.clear();

    const int entrySize = 19;
    const int firstEntryOffset = 11;

    QDataStream d(bytes);
    d.skipRawData(firstEntryOffset);

    int c = 0;
    while(entrySize *c + firstEntryOffset < bytes.size())
    {
        uint16_t temp;
        uint8_t rd;
        d.readRawData( (char*)&rd, 1); //?
        if(rd==4) break;
        FnbGlyphInfo e;
        e.w = 16;
        e.h = 16;
        d.readRawData( (char*)&e.charcode, 4);
        d.readRawData( (char*)&e.x, 2);
        d.readRawData( (char*)&e.y, 2);
        d.readRawData( (char*)&e.w, 2);
        d.readRawData( (char*)&e.h, 2);
        d.readRawData( (char*)&e.leftBearing, 2);
        d.readRawData( (char*)&e.descent, 2);
        d.readRawData( (char*)&e.horizontalAdvance, 2);

        glyphs[e.charcode] = e;
        qDebug() << e.charcode << (QChar) e.charcode << ": w=" << e.w <<" h=" << e.h;
        c++;
    }
    qDebug() << "Entries = " << c;
}

void FnbFont::DrawString(QPainter &p, QString text, QPoint pos)
{
    p.setCompositionMode(QPainter::CompositionMode_Screen);
    QPoint ps = pos;
    for(const auto& c: text)
    {
        ps.rx() = DrawGlyph(p, c.unicode(), ps);
    }
}

float FnbFont::DrawGlyph(QPainter &p, int codepoint, QPoint pos)
{
    int space = 12;
    if(codepoint==32) {pos.rx() += space;return pos.x();}
    auto it = glyphs.find(codepoint);
    if(it != glyphs.end() && it->second.w > 0)
    {
        const FnbGlyphInfo& g = it->second;
        p.drawImage(pos.x()+g.leftBearing, pos.y()+g.descent, texture, g.x, g.y, g.w, g.h);

        p.setPen(QPen(QColor(255,255,255,50), 5));
        p.drawPoint(pos.x(), pos.y());

        //p.setPen(Qt::green);
        //QRect r (pos.x()+g.horizontalOffset, pos.y()+g.verticalOffset, g.w, g.h);
        //p.drawRect(r);

        pos.rx() +=  g.horizontalAdvance;// g.w+g.r1;
    }
    else if(it != glyphs.end()) pos.rx() += space;
    return pos.x();
}

QRectF FnbFont::DebugGlyph(QPainter& p, const FnbGlyphInfo& g, QPoint offset, bool selected)
{
    QRectF rect(g.x + offset.x(), g.y + offset.y(), g.w, g.h);

    QRectF rect2 = rect.adjusted(g.leftBearing, 0,0,0);
    rect2.setWidth(g.horizontalAdvance);
    p.setPen(QColor(0,255,0,60));
    p.drawRect(rect2);

    // selection
    p.setPen(QPen(selected? Qt::yellow: Qt::green, selected?3:1));
    p.drawRect(rect);


    //QRectF rect2(g.x + offset.x() + g.horizontalOffset, g.y + offset.y(), g.w - g.horizontalOffset, g.h);

    //p.setPen(Qt::red);
    //float y = g.y + offset.y();
    //float x = g.x + offset.x() + g.horizontalOffset;
    //p.drawLine(x, y, x, y+g.h);
    //p.setPen(Qt::yellow);
    //p.drawLine(x+g.charWidth, y, x+g.charWidth, y+g.h);

    //p.setPen(Qt::yellow);
    //float y = offset.y() +g.y - g.verticalOffset;
    //float x = g.x+offset.x();
    //p.drawLine(x, y, x+g.w, y);


    p.setFont(dbgFont);
    p.drawText(rect, QString(g.charcode), Qt::AlignCenter | Qt::AlignHCenter);
    //p.setPen(QPen(Qt::yellow, 5));
    //p.drawPoint( g.x + offset.x(), g.y + offset.y() + g.horizontalOffset);

    return rect;
}
