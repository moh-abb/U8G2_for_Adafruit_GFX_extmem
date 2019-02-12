## U8G2_for_Adafruit_GFX_using_external_memory
A library (modified from @olikraus' [U8G2_for_Adafruit_GFX library](https://github.com/olikraus/U8g2_for_Adafruit_GFX)) that includes support for SD memory and SPI flash on Arduino boards.

The library should support:
* fs::File (SPIFFS files) on the ESP8266. I have tested it on the Wemos D1 Mini, so it should work on most boards that use the "fs.h" library.
* SD::File (files stored on SD cards).

#### How to use
In order to use this library, you should first choose whether to use SPIFFS or an SD card to store the font.
Then inside the header (.h) file after the license, uncomment the option that you want to use, and comment any other options.
If you are __not__ using an ESP8266-related board, uncomment "USE_ESP8266". This will ensure that any calls to "yield()"
When using the library in your code, be sure to use the function `setFontFile(File font)` in order to declare which font to use, instead of the typical `setFont(uint8_t* font)`.
Use the library as you would with the original U8G2_for_Adafruit_GFX library, but you should probably check if this library is suitable for your application beforehand.

#### Precautions
Be warned that using external memory to draw text is __much__ slower than using onboard memory. Draw functions that belong to the Adafruit_GFX library rather than this library will not be affected, but bear in mind that (especially for Unicode non-ASCII text) drawing will possibly take multiple seconds. I have not yet optimised this library for speed, but it is unlikely to be comparable to using fonts that come from onboard memory.
You should probably use this library if:
* you want to use large fonts infrequently, such as a font with a large pixel size, and you do not mind a very low refresh rate.
* you want to have a variety of fonts
