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
    void drawBird(Bird* bird);

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




//
//// Delagate
//
class DrawOrders
{
public:
   static void drawStandard(Bird* bird) {
      if (!bird->isDead())
      {
         drawDisk(bird->getPoints(), bird->getRadius() - 0.0, 1.0, 1.0, 1.0); // white outline
         drawDisk(bird->getPoints(), bird->getRadius() - 3.0, 0.0, 0.0, 1.0); // blue center
      }
   }

   static void drawFloater(Bird* bird) {
      if (!bird->isDead())
      {
         drawDisk(bird->getPoints(), bird->getRadius() - 0.0, 0.0, 0.0, 1.0); // blue outline
         drawDisk(bird->getPoints(), bird->getRadius() - 4.0, 1.0, 1.0, 1.0); // white center
      }
   }

   static void drawCrazy(Bird* bird) {
      if (!isDead())
      {
         drawDisk(bird->getPoints(), bird->getRadius() * 1.0, 0.0, 0.0, 1.0); // bright blue outside
         drawDisk(bird->getPoints(), bird->getRadius() * 0.8, 0.2, 0.2, 1.0);
         drawDisk(bird->getPoints(), bird->getRadius() * 0.6, 0.4, 0.4, 1.0);
         drawDisk(bird->getPoints(), bird->getRadius() * 0.4, 0.6, 0.6, 1.0);
         drawDisk(bird->getPoints(), bird->getRadius() * 0.2, 0.8, 0.8, 1.0); // almost white inside
      }
   }

   static void drawSinker(Bird* bird) {
      if (!isDead())
      {
         drawDisk(bird->getPoints(), bird->getRadius() - 0.0, 0.0, 0.0, 0.8);
         drawDisk(bird->getPoints(), bird->getRadius() - 4.0, 0.0, 0.0, 0.0);
      }
   }

   void drawDisk(const Position& center, double radius,
                 double red, double green, double blue)
   {
      assert(radius > 1.0);
      const double increment = M_PI / radius;  // bigger the circle, the more increments

      // begin drawing
      glBegin(GL_TRIANGLES);
      glColor3f((GLfloat)red /* red % */, (GLfloat)green /* green % */, (GLfloat)blue /* blue % */);

      // three points: center, pt1, pt2
      Position pt1;
      pt1.setX(center.getX() + (radius * cos(0.0)));
      pt1.setY(center.getY() + (radius * sin(0.0)));
      Position pt2(pt1);

      // go around the circle
      for (double radians = increment;
      radians <= M_PI * 2.0 + .5;
      radians += increment)
      {
         pt2.setX(center.getX() + (radius * cos(radians)));
         pt2.setY(center.getY() + (radius * sin(radians)));

         glVertex2f((GLfloat)center.getX(), (GLfloat)center.getY());
         glVertex2f((GLfloat)pt1.getX(), (GLfloat)pt1.getY());
         glVertex2f((GLfloat)pt2.getX(), (GLfloat)pt2.getY());

         pt1 = pt2;
      }

      // complete drawing
      glEnd();
   }

};


