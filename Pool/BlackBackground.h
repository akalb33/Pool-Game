#pragma once

#include "Character.h"

class BlackBackground : public Character
{
public:
   BlackBackground();
   virtual ~BlackBackground();
   virtual void Process(float deltaTime);
public:

private:
};
