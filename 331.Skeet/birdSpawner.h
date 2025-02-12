#pragma once

#include "bird.h"

#include <list>
#include <vector>
#include <cassert>


/******************************************************************
 * RANDOM
 * This function generates a random number.
 *
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
inline int random(int min, int max)
{
   assert(min < max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);

   return num;
}


//
//// base
class BirdSpawner {
public:
   BirdSpawner() {}
   virtual bool spawn(int &level, std::list<Bird*>& birds) = 0;
};


class Level1Spawner : public BirdSpawner
{
public:
   Level1Spawner() : BirdSpawner() {}

   bool spawn(int &level, std::list<Bird*>& birds)
   {
      if (level != 1)
         return false;

      double size = 30.0;
      // spawns when there is nothing on the screen
      if (birds.size() == 0 && random(0, 15) == 1)
         birds.push_back(new Standard(size, 7.0));

      // spawn every 4 seconds
      if (random(0, 4 * 30) == 1)
         birds.push_back(new Standard(size, 7.0));

      std::cout << "here" << birds.size() << std::endl;
      return true;
   }
};


class Level2Spawner : public BirdSpawner
{
public:
   Level2Spawner() : BirdSpawner() {}

   bool spawn(int &level, std::list<Bird*>& birds)
   {
      if (level != 2)
         return false;

      double size = 25.0;
      // spawns when there is nothing on the screen
      if (birds.size() == 0 && random(0, 15) == 1)
         birds.push_back(new Standard(size, 7.0, 12));

      // spawn every 4 seconds
      if (random(0, 4 * 30) == 1)
         birds.push_back(new Standard(size, 5.0, 12));
      // spawn every 3 seconds
      if (random(0, 3 * 30) == 1)
         birds.push_back(new Sinker(size));

      return true;
   }
};


class Level3Spawner : public BirdSpawner
{
public:
   Level3Spawner() : BirdSpawner() {}

   bool spawn(int &level, std::list<Bird*>& birds)
   {
      if (level != 3)
         return false;

      double size = 20.0;
      // spawns when there is nothing on the screen
      if (birds.size() == 0 && random(0, 15) == 1)
         birds.push_back(new Standard(size, 5.0, 15));

      // spawn every 4 seconds
      if (random(0, 4 * 30) == 1)
         birds.push_back(new Standard(size, 5.0, 15));
      // spawn every 4 seconds
      if (random(0, 4 * 30) == 1)
         birds.push_back(new Sinker(size, 4.0, 22));
      // spawn every 4 seconds
      if (random(0, 4 * 30) == 1)
         birds.push_back(new Floater(size));

      return true;
   }
};


class Level4Spawner : public BirdSpawner
{
public:
   Level4Spawner() : BirdSpawner() {}

   bool spawn(int &level, std::list<Bird*>& birds)
   {
      if (level != 4)
         return false;

      double size = 15.0;
      // spawns when there is nothing on the screen
      if (birds.size() == 0 && random(0, 15) == 1)
         birds.push_back(new Standard(size, 4.0, 18));

      // spawn every 4 seconds
      if (random(0, 4 * 30) == 1)
         birds.push_back(new Standard(size, 4.0, 18));
      // spawn every 4 seconds
      if (random(0, 4 * 30) == 1)
         birds.push_back(new Sinker(size, 3.5, 25));
      // spawn every 4 seconds
      if (random(0, 4 * 30) == 1)
         birds.push_back(new Floater(size, 4.0, 25));
      // spawn every 4 seconds
      if (random(0, 4 * 30) == 1)
         birds.push_back(new Crazy(size));

      return true;
   }
};

class SpawnHandler
{

public:
   //
   // Constructors
   SpawnHandler() : spawners()
   {
      spawners.push_back(new Level1Spawner());
      spawners.push_back(new Level2Spawner());
      spawners.push_back(new Level3Spawner());
      spawners.push_back(new Level4Spawner());
   }

   void handleSpawnRequest(int level, std::list<Bird*>& birds)
   {
      for (const auto& spawner : spawners) {
         if (spawner->spawn(level, birds))
            return;
      }
   }

private:
   std::vector<BirdSpawner*> spawners;
};
