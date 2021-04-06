# Spelunky 2 Font Editor
A desktop app used to create Spelunky 2 sprite fonts.

The tool operates on the FNB font files from Spelunky 2 and their respective texture atlasses, however only as PNG. Wth this tool you can either create font using any system-installed font (bitmap, TTF, OTF, whatever Qt supports on your OS), or load manually designed font atlas and manually define the font glyphs.

## Download for Windows
To downlaod pre-built package for Windows [go to the Releases page](https://github.com/spelunky-fyi/Spelunky-2-Font-Editor/releases). No pre-built package available for Linux. 

## How to use?
The app should be very straignt-forward. You have two options:
1. Automatically create sprite font from installed system font (TTF, OTF), or
1. Manually design your atlas and define your font by hand.

## Creating sprite font from TTF/OTF
1. Get the fonts you need, and **install** them to your system.
1. Start Spelunky 2 Font Editor. Focus your attention to the Font Creator tab.

	![](https://raw.githubusercontent.com/spelunky-fyi/Spelunky-2-Font-Editor/main/images/font-creator.png)

1. Fill in some sample text into the text field on the top toolbar of the app. This will be used for testing how your font behaves.
1. Select the font family and the font size. Recommended font size for Spelunky 2 is 40 to 60.
1. Use the `Unicode Subranges` button or manually type the characters to include in your font.
1. Hit `Create Font (F6)` and observe the results.
1. Tweak the *Atlas Settings* and *Per Character*  properties and hit `Create Font (F6)` until you are satisfied with the result. **Ensure** that all green rectangles are in the boundaries or your texture (the darker rectangle).

	![](https://raw.githubusercontent.com/spelunky-fyi/Spelunky-2-Font-Editor/main/images/font-glyphs.png)

## Manually creating font and editing glyphs
Spelunky 2 Font Editor allows you to manually define glyphs and edit glyphs as you like, or add new characters tofonts.

1. Draw yourself anatlas with glyphs. 
1. Start Spelunky 2 Font Editor. Hit `Load PNG` button and feed in your texture atlas.
1. *Right click* anywhere to search glyph or define new one. Popup will ask you to input a single character. If such character already exists in the font, it will be automatically selected for you. If such character does not exist, a character definition will be created for you.
1. Edit per-character properties into the Inspector

	![](https://raw.githubusercontent.com/spelunky-fyi/Spelunky-2-Font-Editor/main/images/slyph-inspector.png)


1. Use the *arrow keys* to move the bounding box, *Ctrl+arrows* to resize the bounding box, *Alt+arrows* to edit the *left bearing* and *descent* and Ctrl+Alt+Arrows to edit *horizontal advance*.
1. During editing, save your projects as FNB. I encourage you to **treat FNB files as project files**!


## Getting your font into the game
1. When you are ready, hit the `Save FNB`. If you select a folder within Modlunky 2 mod pack and save your FNB file into `Data/Fonts` of said pack, this app will automatically place the PNG file into `Data/Textures` for you. 
1. In order to make your fonts work in the game, they must be named `fontfirasans.fnb` for the Main Menu and Journal texts and `fontyorkten.fnb` for Journal labels and other places. The `fontmono.fnb` is used at the seed dialog. There is also unused font named `fontdebug.fnb`.
1. While the original textures are in DDS format, Playlunky and Modlunky 2 can load PNG files for convenience, and PNG files are usually easier to manipulate.
1. Feel free to edit your PNG texture in any way you like. You are encouraged to pass this png through Unsharp Mask filter, because Spelunky 2 does not natively render texts alpha-blended!

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
