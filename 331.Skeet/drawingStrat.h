#pragma once

//
// NOTE: Base
class DrawingStratagy
{

public:
   // Constructors
   DrawingStratagy() {}

   // Draw
   virtual void Draw() = 0;
};


//
// NOTE: Standard
class StandardStratagy
{

public:
   //
   // Constructors
   StandardStratagy() {}

   // Draw
   virtual void Draw() = 0;
};


//
// NOTE: Floater
class FloaterStatagy
{

public:
   //
   // Constructors
   FloaterStatagy() {}

   // Draw
   virtual void Draw() = 0;
};


//
// NOTE: Crazy
class CrazyStratagy
{

public:
   //
   // Constructors
   CrazyStratagy() {}

   // Draw
   virtual void Draw() = 0;
};


//
// NOTE: Sinker
class SinkerStatagy
{

public:
   //
   // Constructors
   SinkerStatagy() {}

   // Draw
   virtual void Draw() = 0;
};
