#include <Wire.h>
#include <TM1637Display.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <avr/wdt.h>

#define RELAY_PIN   2
#define TEMP_PULLUP 3
#define TEMP_PIN    4
#define TEMP_GND    5

#define DISPLAY_CLK 6
#define DISPLAY_DIO 7
#define DISPLAY_5V  8
#define DISPLAY_GND 9

#define TEMP_MAX   -17
#define TEMP_MIN   -20
#define TEMP_ERROR  40

OneWire onewire(TEMP_PIN);
DallasTemperature dallas(&onewire);
DeviceAddress addr;

TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);

int temp = 0;
bool relay_on = false;


//     A
//    ---
// F |   | B
//    -G-
// E |   | C
//    ---
//     D

static const uint8_t segment_F = SEG_A | SEG_F | SEG_G | SEG_E;
static const uint8_t segment_E = SEG_A | SEG_F | SEG_G | SEG_E | SEG_D;
static const uint8_t segment_L = SEG_F | SEG_E | SEG_D;
static const uint8_t segment_r = SEG_E | SEG_G;
static const uint8_t segment_n = SEG_C | SEG_E | SEG_G;
static const uint8_t segment_o = SEG_C | SEG_E | SEG_G | SEG_D;
static const uint8_t segment_degree = SEG_A | SEG_F | SEG_B | SEG_G;
static const uint8_t segment_minus = SEG_G;
static const uint8_t segment_empty = 0;

static const uint8_t segment_err[] = { segment_F, segment_E, segment_L };

void setup()
{
  pinMode(TEMP_PULLUP, OUTPUT);
  digitalWrite(TEMP_PULLUP, HIGH);
  pinMode(TEMP_GND, OUTPUT);
  digitalWrite(TEMP_GND, LOW);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(DISPLAY_GND, OUTPUT);
  digitalWrite(DISPLAY_GND, LOW);
  pinMode(DISPLAY_5V, OUTPUT);
  digitalWrite(DISPLAY_5V, HIGH);

  display.setBrightness(2);

  while (true) {
    dallas.begin();
    if (dallas.getAddress(addr, 0))
      break;

    display.showNumberDec(2, false);
    display.setSegments(segment_err, 3, 0);
    delay(1000);
  }
  dallas.setResolution(TEMP_9_BIT);

  wdt_enable(WDTO_4S); /* enable watchdog, 4s timeout */
}

void relay(bool on)
{
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);
  relay_on = on;
}

void loop()
{
  dallas.requestTemperatures();
  int temp = (int)dallas.getTempC(addr);

  if (temp != DEVICE_DISCONNECTED_C) {
    if (relay_on) {
      if (temp < TEMP_MIN)
        relay(false);
    }
    else {
      if (temp > TEMP_ERROR)
        delay(10000); // watchdog, reset pls!
      else
        if (temp > TEMP_MAX)
          relay(true);
    }

    display.showNumberDec(abs(temp), false, 2, 1);

    if (temp < 0)
      display.setSegments(&segment_minus, 1, 0);
    else
      display.setSegments(&segment_empty, 1, 0);

    if (relay_on)
      display.setSegments(&segment_degree, 1, 3);
    else
      display.setSegments(&segment_empty, 1, 3);

  }
  else {
    display.showNumberDec(3, false);
    display.setSegments(segment_err, 3, 0);
    relay(false);
  }

  delay(1000);
  wdt_reset(); /* pat watchdog */
}
