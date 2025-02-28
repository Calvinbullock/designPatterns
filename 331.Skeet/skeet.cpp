/**********************************************************************
 * Skeet
 * No actual birds were killed in the making of this game.
 **********************************************************************/

#include <string>
#include <sstream>
#include "skeet.h"
using namespace std;


#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_18
#endif // __APPLE__

#ifdef __linux__
#include <GL/gl.h>        // Main OpenGL library
#include <GL/glut.h>      // Second OpenGL library
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif // __linux__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>         // OpenGL library we copied
#define _USE_MATH_DEFINES
#include <math.h>
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif // _WIN32

/************************
 * SKEET ANIMATE
 * move the gameplay by one unit of time
 ************************/
void SkeetLogic::animate()
{
   storage.getTime()++;

   // if status, then do not move the game
   if (storage.getTime().isStatus())
   {
      // get rid of the bullets and the birds without changing the score
      storage.getBirds().clear();
      storage.getBullets().clear();
      storage.getEffects().clear();
      storage.getPoints().clear();
      return;
   }

   // spawn
   spawn();

   // move the birds and the bullets
   for (auto element : storage.getBirds())
   {
      element->advance();
      storage.getHitRatio().adjust(element->isDead() ? -1 : 0);
   }
   for (auto bullet : storage.getBullets())
      bullet->move(storage.getEffects());
   for (auto effect : storage.getEffects())
      effect->fly();
   for (auto & pts : storage.getPoints())
      pts.update();

   // hit detection
   for (auto element : storage.getBirds())
      for (auto bullet : storage.getBullets())
         if (!element->isDead() && !bullet->isDead() &&
             element->getRadius() + bullet->getRadius() >
             minimumDistance(element->getPosition(), element->getVelocity(),
                             bullet->getPosition(),  bullet->getVelocity()))
         {
            for (int i = 0; i < 25; i++)
               storage.getEffects().push_back(new Fragment(bullet->getPosition(), bullet->getVelocity()));
            element->kill();
            bullet->kill();
            storage.getHitRatio().adjust(1);
            bullet->setValue(-(element->getPoints()));
            element->setPoints(0);
         }

   // remove the zombie birds
   for (auto it = storage.getBirds().begin(); it != storage.getBirds().end();)
      if ((*it)->isDead())
      {
         if ((*it)->getPoints())
            storage.getPoints().push_back(Points((*it)->getPosition(), (*it)->getPoints()));
         storage.getScore().adjust((*it)->getPoints());
         it = storage.getBirds().erase(it);
      }
      else
         ++it;

   // remove zombie bullets
   for (auto it = storage.getBullets() .begin(); it != storage.getBullets().end(); )
      if ((*it)->isDead())
      {
         (*it)->death(storage.getBullets());
         int value = -(*it)->getValue();
         storage.getPoints().push_back(Points((*it)->getPosition(), value));
         storage.getScore().adjust(value);
         it = storage.getBullets().erase(it);
      }
      else
         ++it;

   // remove zombie fragments
   for (auto it = storage.getEffects().begin(); it != storage.getEffects().end();)
      if ((*it)->isDead())
         it = storage.getEffects().erase(it);
      else
         ++it;

   // remove expired points
   for (auto it = storage.getPoints().begin(); it != storage.getPoints().end();)
      if ((*it).isDead())
         it = storage.getPoints().erase(it);
      else
         ++it;
}

/************************************************************************
 * DRAW Background
 * Fill in the background
 *  INPUT color   Background color
 *************************************************************************/
void SkeetLogic::drawBackground(double redBack, double greenBack, double blueBack) const
{
   glBegin(GL_TRIANGLE_FAN);

   // two rectangles is the fastest way to fill the screen.
   glColor3f((GLfloat)redBack /* red % */, (GLfloat)greenBack /* green % */, (GLfloat)blueBack /* blue % */);
   glVertex2f((GLfloat)0.0, (GLfloat)0.0);
   glVertex2f((GLfloat)storage.getDimensions().getX(), (GLfloat)0.0);
   glVertex2f((GLfloat)storage.getDimensions().getX(), (GLfloat)storage.getDimensions().getY());
   glVertex2f((GLfloat)0.0, (GLfloat)storage.getDimensions().getY());

   glEnd();
}

/************************************************************************
 * DRAW Timer
 * Draw a large timer on the screen
 *  INPUT percent     Amount of time left
 *        Foreground  Foreground color
 *        Background  Background color
 *************************************************************************/
void SkeetLogic::drawTimer(double percent,
                     double redFore, double greenFore, double blueFore,
                     double redBack, double greenBack, double blueBack) const
{
   double radians;

   GLfloat length = (GLfloat)storage.getDimensions().getX();
   GLfloat half = length / (GLfloat)2.0;

   // do the background stuff
   drawBackground(redBack, greenBack, blueBack);

   // foreground stuff
   radians = percent * M_PI * 2.0;
   GLfloat x_extent = half + length * (GLfloat)sin(radians);
   GLfloat y_extent = half + length * (GLfloat)cos(radians);

   // get read to draw the triangles
   glBegin(GL_TRIANGLE_FAN);
   glColor3f((GLfloat)redFore /* red % */, (GLfloat)greenFore /* green % */, (GLfloat)blueFore /* blue % */);
   glVertex2f(half, half);

   // fill in the triangles, one eight at a time
   switch ((int)(percent * 8.0))
   {
   case 7: // 315 - 360
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      glVertex2f(0.0, length);
      break;
   case 6: // 270 - 315
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      glVertex2f(0.0, half);
      break;
   case 5: // 225 - 270
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      break;
   case 4: // 180 - 225
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(half, 0.0);
      break;
   case 3: // 135 - 180
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, half);
      glVertex2f(length, 0.0);
      break;
   case 2: // 90 - 135 degrees
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, half);
      break;
   case 1: // 45 - 90 degrees
      glVertex2f(half, length);
      glVertex2f(length, length);
      break;
   case 0: // 0 - 45 degrees
      glVertex2f(half, length);
      break;
   }
   glVertex2f(x_extent, y_extent);

   // complete drawing
   glEnd();

   // draw the red line now
   glBegin(GL_LINES);
   glColor3f((GLfloat)0.6, (GLfloat)0.0, (GLfloat)0.0);
   glVertex2f(half, half);
   glVertex2f(x_extent, y_extent);
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}

/*************************************************************************
 * DRAW TEXT
 * Draw text using a simple bitmap font
 *   INPUT  topLeft   The top left corner of the text
 *          text      The text to be displayed
 ************************************************************************/
void drawText(const Position& topLeft, const char* text)
{
   void* pFont = GLUT_TEXT;
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);

   // prepare to output the text from the top-left corner
   glRasterPos2f((GLfloat)topLeft.getX(), (GLfloat)topLeft.getY());

   // loop through the text
   for (const char* p = text; *p; p++)
      glutBitmapCharacter(pFont, *p);
}
void drawText(const Position & topLeft, const string & text)
{
   drawText(topLeft, text.c_str());
}

/************************
 * DRAW BULLSEYE
 * Put a bullseye on the screen
 ************************/
void SkeetLogic::drawBullseye(double angle) const
{
   // find where we are pointing
   double distance = storage.getDimensions().getX();
   GLfloat x = storage.getDimensions().getX() - distance * cos(angle);
   GLfloat y = distance * sin(angle);

   // draw the crosshairs
   glBegin(GL_LINES);
   glColor3f((GLfloat)0.6, (GLfloat)0.6, (GLfloat)0.6);

   // Draw the actual lines
   glVertex2f(x - 10.0, y);
   glVertex2f(x + 10.0, y);

   glVertex2f(x, y - 10.0);
   glVertex2f(x, y + 10.0);

   glColor3f((GLfloat)0.2, (GLfloat)0.2, (GLfloat)0.2);
   glVertex2f(storage.getDimensions().getX(), 0.0);
   glVertex2f(x, y);

   // Complete drawing
   glEnd();
}

/************************
 * SKEET DRAW LEVEL
 * output everything that will be on the screen
 ************************/
void SkeetLogic::drawLevel() const
{
   // output the background
   drawBackground(storage.getTime().level() * .1, 0.0, 0.0);

   // draw the bullseye
   if (storage.isBullseye())
      drawBullseye(storage.getGun().getAngle());

   // output the gun
   storage.getGun().display();

   // output the birds, bullets, and fragments
   for (auto& pts : storage.getPoints())
      pts.show();
   for (auto effect : storage.getEffects())
      effect->render();
   for (auto bullet : storage.getBullets())
      bullet->output();
   for (auto element : storage.getBirds())
      element->draw();

   // status
   drawText(Position(10,                                      storage.getDimensions().getY() - 30), storage.getScore().getText()  );
   drawText(Position(storage.getDimensions().getX() / 2 - 30, storage.getDimensions().getY() - 30), storage.getTime().getText()   );
   drawText(Position(storage.getDimensions().getX() - 110,    storage.getDimensions().getY() - 30), storage.getHitRatio().getText());
}

/************************
 * SKEET DRAW STATUS
 * place the status message on the center of the screen
 ************************/
void SkeetLogic::drawStatus() const
{
   // output the text information
   ostringstream sout;
   if (storage.getTime().isGameOver())
   {
      // draw the end of game message
      drawText(Position(storage.getDimensions().getX() / 2 - 30, storage.getDimensions().getY() / 2 + 10),
               "Game Over");

      // draw end of game status
      drawText(Position(storage.getDimensions().getX() / 2 - 30, storage.getDimensions().getY() / 2 - 10),
               storage.getTime().getText());
   }
   else
   {
      // output the status timer
      drawTimer(1.0 - storage.getTime().percentLeft(),
                     (storage.getTime().level() - 0.0) * .1, 0.0, 0.0,
                     (storage.getTime().level() - 1.0) * .1, 0.0, 0.0);

      // draw the message giving a countdown
      sout << "Level " << storage.getTime().level()
           << " begins in " << storage.getTime().secondsLeft() << " seconds";
      drawText(Position(storage.getDimensions().getX() / 2 - 110, storage.getDimensions().getY() / 2 - 10),
         sout.str());
   }
}

/************************
 * SKEET INTERACT
 * handle all user input
 ************************/
void SkeetLogic::interact(const UserInput & ui)
{
   Time time = storage.getTime();
   Gun gun = storage.getGun();

   // reset the game
   if (time.isGameOver() && ui.isSpace())
   {
      time.reset();
      storage.getScore().reset();
      storage.getHitRatio().reset();
      return;
   }

   // gather input from the interface
   gun.interact(ui.isUp() + ui.isRight(), ui.isDown() + ui.isLeft());
   Bullet *p = nullptr;

   // a pellet can be shot at any time
   if (ui.isSpace())
      p = new Pellet(gun.getAngle());
   // missiles can be shot at level 2 and higher
   else if (ui.isM() && time.level() > 1)
      p = new Missile(gun.getAngle());
   // bombs can be shot at level 3 and higher
   else if (ui.isB() && time.level() > 2)
      p = new Bomb(gun.getAngle());

   storage.isBullseye() = ui.isShift();

   // add something if something has been added
   if (nullptr != p)
      storage.getBullets().push_back(p);

   // send movement information to all the bullets. Only the missile cares.
   for (auto bullet : storage.getBullets())
      bullet->input(ui.isUp() + ui.isRight(), ui.isDown() + ui.isLeft(), ui.isB());
}

/******************************************************************
 * RANDOM
 * This function generates a random number.
 *
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
int random(int min, int max)
{
   assert(min < max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);

   return num;
}

/************************
 * SKEET SPAWN
 * lanuch new birds
 ************************/
void SkeetLogic::spawn()
{
   std::list<Bird*> birds = storage.getBirds();

   double size;
   switch (storage.getTime().level())
   {
      // in level 1 spawn big birds occasionally
      case 1:
         size = 30.0;
         // spawns when there is nothing on the screen
         if (birds.size() == 0 && random(0, 15) == 1)
            birds.push_back(new Standard(size, 7.0));

         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            birds.push_back(new Standard(size, 7.0));
         break;

      // two kinds of birds in level 2
      case 2:
         size = 25.0;
         // spawns when there is nothing on the screen
         if (birds.size() == 0 && random(0, 15) == 1)
            birds.push_back(new Standard(size, 7.0, 12));

         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            birds.push_back(new Standard(size, 5.0, 12));
         // spawn every 3 seconds
         if (random(0, 3 * 30) == 1)
            birds.push_back(new Sinker(size));
         break;

      // three kinds of birds in level 3
      case 3:
         size = 20.0;
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
         break;

      // three kinds of birds in level 4
      case 4:
         size = 15.0;
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
         break;

      default:
         break;
   }
}
