#pragma once

#include "Character.h"
#include "WhiteSpot.h"
#include "Pocket.h"

class Ball : public Character
{
public:
   Ball();
   virtual ~Ball();
   virtual void Process(float deltaTime);
   static bool AllBallsStopped() { return (mNumberOfBallsMoving == 0); }
   void UpdateWhiteSpotPosition(float time);
   void UseWhiteSpot();
   static float GetBallDiameter() { return mBallDiameter; }
   static float GetPocketedBallDiameter() { return mBallDiameter * 0.9f; }
   void Reset();

private:
   static int mNumberOfBallsMoving;
   static const float mBallDiameter;
   bool mBallMoving;            
   WhiteSpot* mpWhiteSpot;
   Pocket* mpPocket;  
   bool mMovingTowardBottomOfPocket;
   static const float mVelocityTowardPocketBottom;
   static const float mVelocityTowardFinalPosition;
};
