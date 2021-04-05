# Spelunky 2 Font Editor
A desktop app used to create Spelunky 2 sprite fonts.

The tool operates on the FNB font files from Spelunky 2 and their respective texture atlasses, however only as PNG. Wth this tool you can either create font using any system-installed font (bitmap, TTF, OTF, whatever Qt supports on your OS), or load manually designed font atlas and manually define the font glyphs.

## UNDER HEAVY DEVELOPMENT
The tool is still not completed in full, thus no proper documentation is available at this point. Play with the tool yourself for now.

If the font looks badly offsetted in game, play with the Baseline setting.

It's better to do some sharpen filter on the generated atlas because by default Spelunky 2 text rendering does not play super well with anti-aliasing. By default only the Red channel is used from the sprite atlas, however this tool renders transparent PNG sheet.

## How to use?
Since currently the tool is incomplete, there are no available binary builds. You must use the source code and build the app yourself.

## How to build?
1. [Get Qt open-source and Qt Creator](https://www.qt.io/download-open-source) 
1. Open the `FNBTool.pro` in Qt Creator.
1. Enjoy! So far, only `qmake` is supported.
