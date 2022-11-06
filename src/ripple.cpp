#include <FastLED.h>
#include <cppQueue.h>

// int adj[10][3] = {
//     {3, -1, -1},
//     {2, -1, -1},
//     {1, 3, -1},
//     {0, 4, 2},
//     {3, 5, -1},
//     {4, 6, -1},
//     {5, 7, 9},
//     {6, 8, -1},
//     {7, -1, -1},
//     {6, -1, -1}};

int NUM_PANELS = 10;
int maxBrightness = 250;
int minBrightness = 200;
int dimmingIncrement = 5;
bool visited[10] = {false};
int rippleChance = 300;
// bool queue[10] = {false};

int minMillisBeforeRipple = 5000;
int maxMillisBeforeRipple = 15000;

cppQueue queue(sizeof(int), 10, LIFO);

bool shouldRipple(int chance)
{
  int rand = random(chance);
  return rand == (int)rand / 2;
}

void ripple2(CRGBSet panels[], CHSV hsvs[], const int adj[10][3], unsigned long &elapsed)
{
  // On every iteration of a loop, set brightness of panels in queue, clear the panels in the queue, add them to the done array and replace them with their neighbours
  if (!queue.isEmpty())
  {
    cppQueue temp(sizeof(int), 10, LIFO);
    while (!queue.isEmpty())
    {
      int rec;
      queue.pop(&rec);
      visited[rec] = true;
      temp.push(&rec);
      hsvs[rec].value = maxBrightness;
    }
    while (!temp.isEmpty())
    {
      int p;
      temp.pop(&p);
      for (int i = 0; i < 3; i++)
      {
        if (adj[p][i] != -1 && !visited[adj[p][i]])
          queue.push(&adj[p][i]);
      }
    }
  }

  // If the queue doesn't have any neighbours in it, then have a random chance of a panel being added to the queue
  int randomPanel = random(10);
  if (queue.isEmpty() && millis() - elapsed > minMillisBeforeRipple && (millis() - elapsed > maxMillisBeforeRipple || shouldRipple(rippleChance)))
  {
    queue.push(&randomPanel);
    for (int i = 0; i < NUM_PANELS; i++)
    {
      visited[i] = false;
    }
    elapsed = millis();
  }

  // Decrease brightness of each panel by x each iteration, to a certain minimum
  for (int i = 0; i < NUM_PANELS; i++)
  {
    if (hsvs[i].value > minBrightness)
      hsvs[i].value -= dimmingIncrement; // TODO implement logic to make sure it always approaches same value
    panels[i] = hsvs[i];
  }
  FastLED.show();
}

// void computeDistances(const int adj[10][3], int index, int res[10], bool visited[],int level = 0) {
//   if (visited[index]) return;
//   res[index] = level;
//   visited[index]= true;
//   for (int i = 0; i < 3; i++)
//   {
//     if(adj[index][i] != -1) {
//       computeDistances(adj, adj[index][i], res, visited, level + 1);
//     }
//   }
// }

// void ripple(CRGBSet panels[], CHSV startingHue, int startingIndex, int NUM_PANELS, int adj[10][3]) {
//   bool visited[NUM_PANELS] = {false};
//   panels[startingIndex] = startingHue;
//   int res[10] = {0};
//   computeDistances(adj, startingIndex, res, visited, 0);
//   for (int i = 0; i < 20; i++)
//   {
//     for (int j = 0; j < 10; j++)
//     {
//       if (res[j] <= i){
//         startingHue.val -= 10;
//         panels[j] = startingHue;
//         startingHue.val += 10;
//       }
//     }
//     FastLED.show();
//     delay(50);
//     startingHue.val -= 10;
//   }

// }
