#if FRONTEND_LED
#include "led/led-frontend.h"

LedFrontend::LedFrontend() {
  this->strip = new Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
}

void LedFrontend::init() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  strip->begin();
  strip->show();
  strip->setBrightness(BRIGHTNESS);

}

void LedFrontend::connected(Config config, DataSource *dataSource) {
 strip->setPixelColor(0, 0x00ff00);
 strip->show();
}

void LedFrontend::doLoop() {
  if ((millis() - lastBlinkTime) > 100) {
    strip->setPixelColor(currentLedIndex, blinkStatus ? currentLedColor : 0);
    strip->show();

    lastBlinkTime = millis();
    blinkStatus = blinkStatus;
  }
}

void LedFrontend::updateDisplayModel(DisplayModel *displayModel) {
  uint32_t colors[] = {0x008200, 0x00ff00, 0xffff00, 0xffae29, 0xff2d19, 0x940000};
  float j;
  for (int i = 1; i < strip->numPixels(); i++) {
    j = 5.0 * ((float) i) / strip->numPixels();
    uint32_t c = mixColors(colors[(int) j], colors[(int) j + 1], j - ((int) j));
    strip->setPixelColor(i, c);
  }
  strip->show();

  currentLedIndex = strip->numPixels() * displayModel->level;
  currentLedColor = strip->getPixelColor(currentLedIndex);
}

void LedFrontend::updateDataSourceStatus(DataSourceStatus status) {
  uint32_t color = 0x0;
  switch (status) {
    case SUCCESS:
    color = 0x00ff00;
    break;

    case FAILURE:
    color = 0xff0000;
    break;

    case IN_PROGRESS:
    color = 0xffff00;
    break;
 }
 strip->setPixelColor(0, color);
 strip->show();
}

void LedFrontend::print(String message) {
  Serial.print(message);
}

void LedFrontend::println(String message) {
  Serial.println(message);
}

void LedFrontend::unpack(uint32_t color, uint8_t *colors) {
  colors[0] = (color >> 16) & 0xff;
  colors[1] = (color >> 8) & 0xff;
  colors[2] = (color >> 0) & 0xff;
}

uint32_t LedFrontend::pack(uint8_t *colors) {
  uint16_t r = colors[0];
  uint16_t g = colors[1];
  uint16_t b = colors[2];
  return (r << 16) | (g << 8) | b;
}

uint32_t LedFrontend::mixColors(uint32_t c1, uint32_t c2, float f) {
  uint8_t rgb1[3];
  uint8_t rgb2[3];
  unpack(c1, rgb1);
  unpack(c2, rgb2);

  uint8_t rgb[3];
  for (int i = 0; i < 3; i++) {
    rgb[i] = (1 - f) * rgb1[i] + f * rgb2[i];
  }

  return pack(rgb);
}

#endif