#include "stdafx.h"
#include "Util.h"
#include "Bumper.h"
#include "Character.h"
#include <cmath>

Bumper::Bumper()
   : Character(typeid(Bumper))
   , mAngleDoubled(0.0f)
{
   //SetTexture(TextureManager::background);
   SetDisplayOrder(8);
}

Bumper::~Bumper()
{
}

void Bumper::Process(float deltaTime)
{
  
}

void Bumper::Init(float x1, float y1, float x2, float y2)
{
   mPolygon.SetOrigin(0.0f, 0.0f);
   mPolygon.AddPoint(x1, y1);
   mPolygon.AddPoint(x2, y2);
   mPolygon.AdjustPoints(mX, mY, mAngle, mScale);
   mAngleDoubled = 2.0f * Util::GetAngleFromRectangle(x1 - x2, y1 - y2);
}
