#include "stdafx.h"
#include "Util.h"
#include "BlackBackground.h"
#include "Character.h"

BlackBackground::BlackBackground()
   : Character(typeid(BlackBackground))
{
   SetTexture(TextureManager::blackBackground);
   SetDisplayOrder(2);
   mScale = 0.65f;
}

BlackBackground::~BlackBackground()
{
}

void BlackBackground::Process(float deltaTime)
{
  
}