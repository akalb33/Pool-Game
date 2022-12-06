#pragma once

#include "Character.h"

class WhiteSpot : public Character
{
public:
   WhiteSpot();
   virtual ~WhiteSpot();
   virtual void Process(float deltaTime);
   void Reset();

   float mDeltaX;
   float mDeltaY;
   bool mBackside;
private:

};
