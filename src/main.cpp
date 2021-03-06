#include "lcdgfx.h"

// https://www.maxbotix.com/documents/LV-MaxSonar-EZ_Datasheet.pdf
const int PW_US_PER_INCH = 147;
const int ULTRASONIC_PWM_PIN = 3;
const uint8_t *DISPLAY_FONT = ssd1306xled_font6x8;
const uint8_t DISPLAY_FONT_WIDTH = DISPLAY_FONT[1];
const uint8_t DISPLAY_FONT_HEIGHT = DISPLAY_FONT[2];
const uint8_t DISPLAY_TITLE_FONT_WIDTH = 2 * DISPLAY_FONT_WIDTH;
const uint8_t DISPLAY_TITLE_FONT_HEIGHT = 2 * DISPLAY_FONT_HEIGHT;
const uint8_t DISPLAY_ALTITUDE_FONT_WIDTH = 4 * DISPLAY_FONT_WIDTH;

DisplaySSD1306_128x64_I2C display(-1);
String displayed_altitude = "";

double read_altitude_in()
{
  // Convert pulse width (µs) to inches
  return (double)pulseIn(ULTRASONIC_PWM_PIN, HIGH) / PW_US_PER_INCH;
}

double read_altitude_ft()
{
  return read_altitude_in() / 12;
}

void print_title(const char *title)
{
  // Calculate the horizontal position the title should be printed so that it's centered
  lcduint_t title_len_px = strlen(title) * DISPLAY_TITLE_FONT_WIDTH;
  lcduint_t title_start_px = display.width() > title_len_px ? (display.width() - title_len_px) / 2 : 0;

  display.printFixedN(title_start_px, 0, title, EFontStyle::STYLE_NORMAL, FONT_SIZE_2X);
}

void print_altitude(double altitude)
{
  String altitude_to_display(altitude, 1);

  // Don't print the altitude if it's already being displayed
  if (altitude_to_display == displayed_altitude)
  {
    return;
  }

  // Clear the previously printed altitude
  display.setColor(BLACK);
  display.fillRect(0, DISPLAY_TITLE_FONT_HEIGHT, display.width(), display.height());

  // Calculate the horizontal position the altitude should be printed so that it's centered
  lcduint_t altitude_len_px = altitude_to_display.length() * DISPLAY_ALTITUDE_FONT_WIDTH;
  lcduint_t altitude_start_px = display.width() > altitude_len_px ? (display.width() - altitude_len_px) / 2 : 0;

  display.printFixedN(altitude_start_px, display.height() / 2, altitude_to_display.c_str(), EFontStyle::STYLE_NORMAL, FONT_SIZE_4X);
  displayed_altitude = altitude_to_display;
}

void setup()
{
  display.setFixedFont(DISPLAY_FONT);
  display.begin();
  display.clear();

  print_title("ALTITUDE");
}

void loop()
{
  print_altitude(read_altitude_ft());

  delay(500);
}
