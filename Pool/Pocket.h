#pragma once

#include "Character.h"

class Pocket : public Character
{
public:
   Pocket();
   virtual ~Pocket();
   virtual void Process(float deltaTime);
   void Init(float x, float y, float bottomX, float bottomY, float finalX, float finalY);
   float GetBottomX() { return mBottomX; }
   float GetBottomY() { return mBottomY; }
   float GetFinalX() { return mFinalX; }
   float GetFinalY() { return mFinalY; }


private:
   float mBottomX;
   float mBottomY;
   float mFinalX;
   float mFinalY;
};
