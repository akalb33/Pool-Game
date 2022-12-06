#pragma once

#include "Character.h"
#include "Ball.h"

class Player : public Character
{
public:
   Player();
   virtual ~Player();
   void Reset();
   virtual void Process(float deltaTime);
   void AssociateCueBall(Ball* pBall) { mpCueBall = pBall; }
   void DrawHelperLine();
   void UpdateCueAngle(float mouseDeltaX, float mouseDeltaY);

private:
   Ball* mpCueBall;
   bool mSpacebarPreviouslyPressed;
   float mDistanceToCueBall;
   float mPreviousDistanceToCueBall;
   float mTimePassedAccumulator; 
   bool mKilled;
};
