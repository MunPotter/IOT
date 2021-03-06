#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

static const unsigned char PROGMEM image_data_Saraarray[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x03, 0xfe, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x0f, 0xf0, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x3f, 0xc0, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfd, 0xff, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x1f, 0x00, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x7f, 0xc0, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xc0, 0x7f, 0x80, 0x01, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x80, 0x0f, 0xc0, 0x1f, 0xe0, 0x07, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x00, 0x07, 0xc0, 0x07, 0xf8, 0x3f, 0xe0, 0xfe, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xfe, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x1e, 0x00, 0x1f, 0xf0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0x80, 0x3f, 0xc0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0xff, 0xc1, 0xff, 0x00, 0x7c, 0x01, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xfc, 0x00, 0x7e, 0x03, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xe3, 0xff, 0xf0, 0x00, 0x7c, 0x0f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0x80, 0xff, 0x80, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x0f, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 0xfc, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfc, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfc, 0x03, 0x80, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0xff, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x03, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9d, 0xff, 0xf8, 0x7f, 0xff,
  0xff, 0xfc, 0x03, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfc, 0x03, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x0f, 0xe7, 0x80, 0x03, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0x0c, 0x0f, 0x9c, 0x01, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x00, 0x01, 0xff, 0xff, 0xfc, 0x78, 0x7c, 0x0c, 0x1f, 0x9c, 0x00, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x02, 0x00, 0xff, 0x03, 0xe0, 0x60, 0xfc, 0x08, 0x3f, 0x81, 0xc0, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x03, 0x00, 0x7c, 0x00, 0xf0, 0x41, 0xfc, 0x08, 0x1f, 0x99, 0xc0, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x03, 0x80, 0x38, 0x78, 0x70, 0x40, 0xfc, 0x0c, 0x0f, 0x9d, 0xc0, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x03, 0xc0, 0x1c, 0x30, 0x70, 0x60, 0x3c, 0x0e, 0x07, 0x81, 0xc0, 0xf0, 0x3f, 0xff,
  0xff, 0xfc, 0x03, 0xe0, 0x0e, 0x01, 0xe0, 0x78, 0x1c, 0x0f, 0x01, 0x81, 0xc0, 0xf0, 0x3f, 0xff,
  0xff, 0xff, 0x83, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xf3, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfe, 0x3f, 0xf7, 0xfc, 0xe5, 0xfd, 0x7b, 0x7f, 0xff, 0x7f, 0xf7, 0xdf, 0xff, 0xff, 0xff,
  0xff, 0xfd, 0xda, 0x94, 0xe5, 0xed, 0xad, 0x6b, 0x52, 0x9e, 0xb7, 0x74, 0xdc, 0x4a, 0x7f, 0xff,
  0xff, 0xfc, 0xda, 0x95, 0xd4, 0xec, 0xad, 0x4b, 0x5a, 0xdd, 0x99, 0x97, 0x5d, 0x5a, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
class Display {
  public:

    void DisplaySetup() {
      display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
      if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
      }
    }

    void RokkhiBitMap() {
      display.clearDisplay();
      delay(200);
      display.drawBitmap(0, 0, image_data_Saraarray, 128, 64, 1);
      display.display();
    }
    void DisplayConfig() {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 7);
    }
    void TextDisplay(String text) {
      display.println(text);
    }
    void intDisplay(int valu) {
      display.print("Id :");
      display.println(valu);
    }
    void ClearDisplay() {
      display.clearDisplay();
    }
    void FinishDisplay() {
      display.display();
    }

};
Display dis;
