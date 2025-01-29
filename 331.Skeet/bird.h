/***********************************************************************
 * Header File:
 *    Bird : Everything that can be shot
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Stuff that moves across the screen to be shot
 ************************************************************************/

#pragma once
#include "drawingStrat.h"
#include "position.h"
#include <array>

/**********************
 * BIRD
 * Everything that can be shot
 **********************/
class Bird
{
protected:
   static Position dimensions; // size of the screen
   Position pt;                  // position of the flyer
   Velocity v;                // velocity of the flyer
   double radius;             // the size (radius) of the flyer
   bool dead;                 // is this flyer dead?
   int points;                // how many points is this worth?
   DrawingStratagy* drawStrats[4];
   int drawImplementaion;

public:
   Bird() : dead(false), points(0), radius(1.0), drawImplementaion(0)
   {
      drawStrats[0] = new StandardStratagy();
      drawStrats[1] = new FloaterStatagy();
      drawStrats[2] = new CrazyStratagy();
      drawStrats[3] = new SinkerStatagy();
   }
   ~Bird()
   {
      for (int i = 0; i < 4; ++i) {
         delete drawStrats[i];
      }
   }

   // setters
   void operator=(const Position    & rhs) { pt = rhs;    }
   void operator=(const Velocity & rhs) { v = rhs;     }
   void kill()                          { dead = true; }
   void setPoints(int pts)              { points = pts;}

   // getters
   bool isDead()           const { return dead;   }
   Position getPosition()     const { return pt;     }
   Velocity getVelocity()  const { return v;      }
   double getRadius()      const { return radius; }
   int getPoints() const { return points; }
   bool isOutOfBounds() const
   {
      return (pt.getX() < -radius || pt.getX() >= dimensions.getX() + radius ||
      pt.getY() < -radius || pt.getY() >= dimensions.getY() + radius);
   }

   // special functions
   virtual void draw()
   {
      if (!isDead())
      {
         drawStrats[drawImplementaion]->draw(pt, radius);
      }
   }
   virtual void advance() = 0;
};

/*********************************************
 * STANDARD
 * A standard bird: slows down, flies in a straight line
 *********************************************/
class Standard : public Bird
{
public:
   Standard(double radius = 25.0, double speed = 5.0, int points = 10);
   void advance();
};

/*********************************************
 * FLOATER
 * A bird that floats like a balloon: flies up and really slows down
 *********************************************/
class Floater : public Bird
{
public:
   Floater(double radius = 30.0, double speed = 5.0, int points = 15);
   void advance();
};

/*********************************************
 * CRAZY
 * A crazy flying object: randomly changes direction
 *********************************************/
class Crazy : public Bird
{
public:
   Crazy(double radius = 30.0, double speed = 4.5, int points = 30);
   void advance();
};

/*********************************************
 * SINKER
 * A sinker bird: honors gravity
 *********************************************/
class Sinker : public Bird
{
public:
   Sinker(double radius = 30.0, double speed = 4.5, int points = 20);
   void advance();
};
