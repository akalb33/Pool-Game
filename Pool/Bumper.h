#pragma once

#include "Character.h"

class Bumper : public Character
{
public:
   Bumper();
   virtual ~Bumper();
   virtual void Process(float deltaTime);
   void Init(float x1, float y1, float x2, float y2);
   float GetAngleDoubled() { return mAngleDoubled; }

private:
   float mAngleDoubled;
};
