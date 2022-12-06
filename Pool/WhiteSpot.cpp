#include "stdafx.h"
#include "Util.h"
#include "WhiteSpot.h"
#include "Character.h"

WhiteSpot::WhiteSpot()
   : Character(typeid(WhiteSpot))
   , mDeltaX(0.0f)
   , mDeltaY(0.0f)
   , mBackside(false)
{
   SetTexture(TextureManager::whiteSpot);
   SetDisplayOrder(7);
   UseSFMLScaling(); 
}

WhiteSpot::~WhiteSpot()
{
}

void WhiteSpot::Process(float deltaTime)
{
  
}

void WhiteSpot::Reset()
{
   mSprite.setColor(sf::Color(255, 255, 255));
   SetDisplayOrder(7);
   mSprite.setScale(1.0f, 1.0f);

}