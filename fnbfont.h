#ifndef FNBFONT_H
#define FNBFONT_H

#include <QPainter>
#include <QPoint>
#include <QString>


struct FnbGlyphInfo
{
    uint32_t charcode;
    uint16_t x,y,w,h;  // pixel level
    int16_t leftBearing,descent,horizontalAdvance;
};

struct FnbFont
{
    FnbFont();
    QImage texture;
    std::map<int, FnbGlyphInfo> glyphs;
    int16_t baseline;
    void Load(QString fnbFile, QString PNGFile);
    void SaveFNB(QString fnbFile);
    void ParseFNB(const QByteArray bytes);
    void DrawString(QPainter& p, QString text, QPoint pos);
    float DrawGlyph(QPainter& p, int glyph, QPoint pos);
    QRectF DebugGlyph(QPainter& p, const FnbGlyphInfo& g, QPoint offset, float scale, bool selected = false);
};

#endif // FNBFONT_H
