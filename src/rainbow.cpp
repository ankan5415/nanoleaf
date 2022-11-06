#include <FastLED.h>


void rainbowCycle(CRGBSet panels[], int hueDelta, CHSV hsvs[], int NUM_PANELS) {
  for (int i = 0; i < NUM_PANELS; i++)
  {
    panels[i] = hsvs[i];
    hsvs[i].hue+=hueDelta;
    FastLED.show();
  }
}