#include "stdafx.h"
#include "Util.h"
#include "Corner.h"
#include "Character.h"

Corner::Corner()
   : Character(typeid(Corner))
{
   SetDisplayOrder(8);
}

Corner::~Corner()
{
}

void Corner::Process(float deltaTime)
{
  
}

void Corner::Init(float x, float y)
{
   mX = x; 
   mY = y; 
   mPolygon.SetOrigin(x, y);
   mPolygon.AddPoint(x, y);
   mPolygon.AdjustPoints(mX, mY, mAngle, mScale);
}