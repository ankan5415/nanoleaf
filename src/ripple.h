#include <FastLED.h>

void ripple2(CRGBSet panels[], CHSV hsvs[], const int adj[10][3], unsigned long &elapsed);
bool shouldRipple(int chance);
// void ripple(CRGBSet panels[], CHSV startingHue, int startingIndex, int NUM_PANELS, int adj[10][3]);
// void computeDistances(const int adj[10][3], int index, int res[10], bool visited[], int level = 0);