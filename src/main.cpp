#include <Arduino.h>
#include <FastLED.h>
#include <rainbow.h>
#include <ripple.h>
/* HARDWARE DEFINITIONS */
#define NUM_PANELS 10
#define LED_PIN 3
#define NUM_LEDS 89
#define BRIGHTNESS 255
#define LED_TYPE WS2811
#define COLOR_ORDER GRB

/* LIGHT-SPECIFIC DEFS */
#define UPDATES_PER_SECOND 60 // define the speed
#define HUE_DELTA 3

CRGBArray<NUM_LEDS> leds;
CHSV hsvArray[NUM_PANELS];

CRGBSet panelArray[] = {
    leds(26, 34),
    leds(0, 7),
    leds(8, 16),
    leds(17, 25),
    leds(35, 43),
    leds(44, 52),
    leds(53, 61),
    leds(62, 70),
    leds(71, 79),
    leds(80, 87),
};

int adj[10][3] = {
    {3, -1, -1},
    {2, -1, -1},
    {1, 3, -1},
    {0, 4, 2},
    {3, 5, -1},
    {4, 6, -1},
    {5, 7, 9},
    {6, 8, -1},
    {7, -1, -1},
    {6, -1, -1}};

CHSV selected;
CHSV neighbour;

unsigned long elapsedSinceRipple;

void setup()
{
  delay(3000); // power-up safety delay
  selected.hue = 100;
  selected.value = 255;
  selected.sat = 240;
  neighbour.hue = HUE_RED;
  neighbour.value = 255;
  neighbour.sat = 240;
  Serial.begin(9600);
  Serial.println("starting...");
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < NUM_PANELS; i++)
  {
    hsvArray[i].hue = i * 5;
    hsvArray[i].sat = 240;
    hsvArray[i].val = BRIGHTNESS;
  }
  elapsedSinceRipple = millis();
}

void loop()
{
  rainbowCycle(panelArray, 1, hsvArray, NUM_PANELS);
  ripple2(panelArray, hsvArray, adj, elapsedSinceRipple);
  // for (int i = 0; i < NUM_PANELS; i++)
  // {

  //   panelArray[i] = selected;
  //   for (int j = 0; j < 3; j++)
  //   {
  //     if (adj[i][j] != -1) panelArray[adj[i][j]] = neighbour;
  //   }
  //   FastLED.show();
  //   delay(2000);
  //   FastLED.clear();
  //   FastLED.show();
  // }
  // int selectedPanel = (int) random(NUM_PANELS+1);
  // ripple(panelArray, hsvArray[0], selectedPanel, NUM_PANELS, adj);
  delay(1000 / UPDATES_PER_SECOND);
}

// Ripple effect
// Decrease brightness of each panel by x each iteration, to a certain minimum
// If the queue doesn't have any neighbours in it, then have a random chance of a panel being added to the queue
// On every iteration of a loop, set brightness of panels in queue, clear the panels in the queue, add them to the done array and replace them with their neighbours