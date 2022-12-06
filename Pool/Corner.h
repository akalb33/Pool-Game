#pragma once

#include "Character.h"

class Corner : public Character
{
public:
   Corner();
   virtual ~Corner();
   virtual void Process(float deltaTime);
   void Init(float x, float y);

private:
};
