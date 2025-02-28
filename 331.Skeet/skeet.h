/***********************************************************************
 * Header File:
 *    SKEET
 * Author:
 *    Br. Helfrich
 * Summary:
 *    The game class
 ************************************************************************/

#pragma once

#include "position.h"
#include "uiInteract.h"
#include "bird.h"
#include "bullet.h"
#include "effect.h"
#include "gun.h"
#include "time.h"
#include "score.h"
#include "points.h"

#include <list>

/*************************************************************************
 * Skeet
 * The game class
 *************************************************************************/
class Skeet
{
public:
   Skeet(Position & dimensions) : dimensions(dimensions),
      gun(Position(800.0, 0.0)), time(), score(), hitRatio(), bullseye(false) {}

   // handle all user input
   void interact(const UserInput& ui);

   // move the gameplay by one unit of time
   void animate();

   // output everything on the screen
   void drawLevel()  const;    // output the game
   void drawStatus() const;    // output the status information

   // is the game currently playing right now?
   bool isPlaying() const { return time.isPlaying();  }
private:
   // generate new birds
   void spawn();
   void drawBackground(double redBack, double greenBack, double blueBack) const;
   void drawTimer(double percent,
                  double redFore, double greenFore, double blueFore,
                  double redBack, double greenBack, double blueBack) const;
   void drawBullseye(double angle) const;

   Gun gun;                       // the gun
   std::list<Bird*> birds;        // all the shootable birds
   std::list<Bullet*> bullets;    // the bullets
   std::list<Effect*> effects;    // the fragments of a dead bird.
   std::list<Points>  points;     // point values;
   Time time;                     // how many frames have transpired since the beginning
   Score score;                   // the player's score
   HitRatio hitRatio;             // the hit ratio for the birds
   Position dimensions;           // size of the screen
   bool bullseye;
};


/* ========================================================================== \\
|| ========================================================================== ||
\\ ========================================================================== */

//
//// STORAGE
class SkeetStorage
{

public:
   //
   // Constructors
   SkeetStorage() : gun(Position(800.0, 0.0)), dimensions(),
         time(), score(), hitRatio(), bullseye(false) { }

   SkeetStorage(Position & dimensions) : gun(Position(800.0, 0.0)), dimensions(dimensions),
         time(), score(), hitRatio(), bullseye(false) { }

   //
   // Getters / Setters
   //

   // Gun Getters and Setters
   Gun & getGun() { return gun; }
   Gun getGun() const { return gun; }
   void setGun(const Gun& gun) { this->gun = gun; }

   // Birds Getters and Setters
   std::list<Bird*>& getBirds() { return birds; }
   const std::list<Bird*>& getBirds() const { return birds; }
   void setBirds(const std::list<Bird*>& birds) { this->birds = birds; }

   // Bullets Getters and Setters
   std::list<Bullet*>& getBullets() { return bullets; }
   const std::list<Bullet*>& getBullets() const { return bullets; }
   void setBullets(const std::list<Bullet*>& bullets) { this->bullets = bullets; }

   // Effects Getters and Setters
   std::list<Effect*>& getEffects() { return effects; }
   const std::list<Effect*>& getEffects() const { return effects; }
   void setEffects(const std::list<Effect*>& effects) { this->effects = effects; }

   // Points Getters and Setters
   std::list<Points>& getPoints() { return points; }
   const std::list<Points>& getPoints() const { return points; }
   void setPoints(const std::list<Points>& points) { this->points = points; }

   // Time Getters and Setters
   Time & getTime() { return time; }
   Time getTime() const { return time; }
   void setTime(const Time& time) { this->time = time; }

   // Score Getters and Setters
   Score getScore() const { return score; }
   void setScore(const Score& score) { this->score = score; }

   // HitRatio Getters and Setters
   HitRatio & getHitRatio() { return hitRatio; }
   HitRatio getHitRatio() const { return hitRatio; }
   void setHitRatio(const HitRatio& hitRatio) { this->hitRatio = hitRatio; }

   // Dimensions Getters and Setters
   Position getDimensions() const { return dimensions; }
   void setDimensions(const Position& dimensions) { this->dimensions = dimensions; }

   // Bullseye Getters and Setters
   bool isBullseye() const { return bullseye; }
   bool & isBullseye() { return bullseye; }
   void setBullseye(bool bullseye) { this->bullseye = bullseye; }

private:
   Gun gun;                       // the gun
   std::list<Bird*> birds;        // all the shootable birds
   std::list<Bullet*> bullets;    // the bullets
   std::list<Effect*> effects;    // the fragments of a dead bird.
   std::list<Points>  points;     // point values;
   Time time;                     // how many frames have transpired since the beginning
   Score score;                   // the player's score
   HitRatio hitRatio;             // the hit ratio for the birds
   Position dimensions;           // size of the screen
   bool bullseye;
};


//
//// LOGIC
class SkeetLogic
{

public:
   //
   // Constructors
   SkeetLogic() : storage() {}
   SkeetLogic(Position & dimensions) : storage() {}

   //
   // Getters / Setters

   //
   // Other
   void animate();
   void spawn();  // generate new birds
   bool isPLaying();
   void interact(const UserInput& ui);

   // output everything on the screen
   void drawLevel()  const;    // output the game
   void drawStatus() const;    // output the status information

   // is the game currently playing right now?
   bool isPlaying() const { return storage.getTime().isPlaying(); }

private:
   void drawBackground(double redBack, double greenBack, double blueBack) const;
   void drawTimer(double percent,
                  double redFore, double greenFore, double blueFore,
                  double redBack, double greenBack, double blueBack) const;
   void drawBullseye(double angle) const;

   SkeetStorage storage;
};


//
//// INTERFACE
class SkeetInterface
{

public:
   //
   // Constructors
   SkeetInterface(Position & dimensions) : logic()
   {
      logic = SkeetLogic(dimensions);
   }

   // handle all user input
   void interact(const UserInput& ui) { logic.interact(ui); }

   // move the gameplay by one unit of time
   void animate() { logic.animate(); }

   // output everything on the screen
   void drawLevel()  const { logic.drawLevel(); };
   void drawStatus() const { logic.drawStatus(); }

   // is the game currently playing right now?
   bool isPlaying() const { return logic.isPlaying(); }

private:
   SkeetLogic logic;
};

