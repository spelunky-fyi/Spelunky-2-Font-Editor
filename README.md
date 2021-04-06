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
