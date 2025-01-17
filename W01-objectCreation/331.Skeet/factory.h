
#include <cassert>
#include <string>
#include <sstream>

#include "position.h"
#include "uiInteract.h"
#include "bird.h"
#include "bullet.h"
#include "effect.h"
#include "gun.h"
#include "time.h"
#include "score.h"
#include "points.h"

using namespace std;

enum Birdtype {
   STANDARD,
   SINKER,
   FLOATER,
   CRAZY
};

inline Bird* birdFactory(int lv, Birdtype type, int birdCount)
{
   double size = 0;
   switch (lv) {
      case 1: // lv 1
         size = 30.0;
         if (type == STANDARD)
            return new Standard(size, 7.0);
         break;

      case 2: // lv 2
         size = 25.0;
         // no birds on screen
         if (type == STANDARD && birdCount == 0)
            return new Standard(size, 7.0, 12);

         else if (type == STANDARD && birdCount != 0)
            return new Standard(size, 5.0, 12);
         else if (type == SINKER)
            return new Sinker(size);
         break;

      case 3: // lv 3
         size = 20.0;
         if (type == STANDARD)
            return new Standard(size, 5.0, 15);
         else if (type == SINKER)
            return new Sinker(size, 4.0, 22);
         else if (type == FLOATER)
            return new Floater(size);
         break;

      case 4: // lv 4
         size = 15.0;
         if (type == STANDARD)
            return new Standard(size, 4.0, 18);
         else if (type == SINKER)
            return new Sinker(size, 3.5, 25);
         else if (type == FLOATER)
            return new Floater(size, 4.0, 25);
         else if (type == CRAZY)
            return new Crazy(size);
         break;
   }

   assert(true);
   return nullptr;
}
