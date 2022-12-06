#include "stdafx.h"
#include "Util.h"
#include "Pocket.h"
#include "Character.h"

Pocket::Pocket()
   : Character(typeid(Pocket))
   , mBottomX(0.0f)
   , mBottomY(0.0f)
   , mFinalX(0.0f)
   , mFinalY(0.0f)
{
   SetDisplayOrder(8);
}

Pocket::~Pocket()
{
}

void Pocket::Process(float deltaTime)
{
  
}

void Pocket::Init(float x, float y, float bottomX, float bottomY, float finalX, float finalY)
{
   mBottomX = bottomX;
   mBottomY = bottomY;
   mFinalX = finalX;
   mFinalY = finalY;
   mX = x; 
   mY = y; 
   mPolygon.SetOrigin(x, y);
   mPolygon.AddPoint(x, y);
   mPolygon.AdjustPoints(mX, mY, mAngle, mScale);
}