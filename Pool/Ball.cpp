#include "stdafx.h"
#include "Util.h"
#include "Ball.h"
#include "Bumper.h"
#include "Corner.h"
#include "Character.h"
#include <iostream>
#include <assert.h>

int Ball::mNumberOfBallsMoving = 0;
const float Ball::mBallDiameter = 36.0f;
const float Ball::mVelocityTowardPocketBottom = 400.0f;
const float Ball::mVelocityTowardFinalPosition = 40.0f;

Ball::Ball()
   : Character(typeid(Ball))
   , mBallMoving(false)
   , mpWhiteSpot(NULL)
   , mpPocket(NULL)
   , mMovingTowardBottomOfPocket(false)
{
   SetDisplayOrder(6);
}

Ball::~Ball()
{
}

void Ball::Process(float deltaTime)
{
   if (mStatusManager.GetStatus() == StatusManager::dying)
   {
      if (mMovingTowardBottomOfPocket)
      {
         if (MoveTowardLocation(mpPocket->GetBottomX(), mpPocket->GetBottomY(), mVelocityTowardPocketBottom, deltaTime))
         {
            float angle = Util::GetAngleFromRectangle(mpPocket->GetFinalX() - mX, mpPocket->GetFinalY() - mY) * Util::RAD_PER_DEG;
            mVelocityX = mVelocityTowardFinalPosition * cos(angle);
            mVelocityY = mVelocityTowardFinalPosition * sin(angle);
            mSprite.setColor(sf::Color(200, 200, 200));
            mScale = 0.00555555f * GetPocketedBallDiameter();
            SetDisplayOrder(3);
            if (mpWhiteSpot != NULL)
            {
               mpWhiteSpot->mSprite.setColor(sf::Color(200, 200, 200));
               mpWhiteSpot->SetDisplayOrder(4);
            }
            mMovingTowardBottomOfPocket = false;
         }
      }
      else
      {
         if (MoveTowardLocation(mpPocket->GetFinalX(), mpPocket->GetFinalY(), mVelocityTowardFinalPosition, deltaTime))
         {
            mNumberOfBallsMoving--;
            mBallMoving = false;
            mStatusManager.SetStatus(StatusManager::dead);
            mVelocityX = 0.0f;
            mVelocityY = 0.0f;
         }
      }
      UpdateWhiteSpotPosition(deltaTime);
   }
   else
   {
      if (abs(mVelocityX) < 10.0f && abs(mVelocityY) < 10.0f)
      {
         mVelocityX = 0.0f;
         mVelocityY = 0.0f;
         if (mBallMoving)
         {
            mNumberOfBallsMoving--;
            mBallMoving = false;
         }
      }
      else
      {
         if (!mBallMoving)
         {
            mNumberOfBallsMoving++;
            mBallMoving = true;
         }

         mVelocityX *= (1.0f - (0.6f * deltaTime));
         mVelocityY *= (1.0f - (0.6f * deltaTime));

         float previousX = mX;
         float previousY = mY;

         ApplyInertia(deltaTime);

         std::vector<Pocket*> pocketsContacted;
         std::vector<Corner*> cornersContacted;
         std::vector<Bumper*> bumpersContacted;
         std::vector<Ball*> ballsContacted;
         if (Character::Collision<Pocket>(pocketsContacted))
         {
            mStatusManager.SetStatus(StatusManager::dying);
            std::vector<Pocket*>::iterator it = pocketsContacted.begin();
            mpPocket = *it;
            mMovingTowardBottomOfPocket = true;
            float angle = Util::GetAngleFromRectangle(mpPocket->GetBottomX() - mX, mpPocket->GetBottomY() - mY) * Util::RAD_PER_DEG;
            mVelocityX = mVelocityTowardPocketBottom * cos(angle);
            mVelocityY = mVelocityTowardPocketBottom * sin(angle);
            mpSoundManager->Play(SoundManager::pocketDrop);
         }
         else if (Character::Collision<Corner>(cornersContacted))
         {
            float angle = 0.0f;
            float magnitude = 0.0f;
            Util::RectangleToVector(mVelocityX, mVelocityY, magnitude, angle);
            std::vector<Corner*>::iterator it = cornersContacted.begin();
            Corner* pCorner = *it;
            float angleFromBallCenterToCorner = Util::GetAngleFromRectangle(mX - pCorner->mX, mY - pCorner->mY);
            float bumperAngleDoubled = 2.0f * (angleFromBallCenterToCorner + 90.0f);
            float newAngle = bumperAngleDoubled - angle;
            Util::VectorToRectangle(magnitude * 0.7f, newAngle, mVelocityX, mVelocityY);
            mX = previousX;
            mY = previousY;
            mpSoundManager->Play(SoundManager::cushion, magnitude / 144.0f);
         }
         else if (Character::Collision<Bumper>(bumpersContacted))
         {
            float angle = 0.0f;
            float magnitude = 0.0f;
            Util::RectangleToVector(mVelocityX, mVelocityY, magnitude, angle);
            std::vector<Bumper*>::iterator it = bumpersContacted.begin();
            Bumper* pBumper = *it;
            float newAngle = pBumper->GetAngleDoubled() - angle;
            Util::VectorToRectangle(magnitude * 0.7f, newAngle, mVelocityX, mVelocityY);
            mX = previousX;
            mY = previousY;
            mpSoundManager->Play(SoundManager::cushion, magnitude / 144.0f);
         }
         else if (Character::WithinProximity<Ball>(ballsContacted, GetBallDiameter()))
         //else if (Character::Collision<Ball>(ballsContacted))
         {
            std::vector<Ball*>::iterator it = ballsContacted.begin();
            Ball* pBall = *it;

            float angle = Util::GetAngleFromRectangle(pBall->mX - mX, pBall->mY - mY);
            float sinTheta = sinf(angle * Util::RAD_PER_DEG);
            float cosTheta = cosf(angle * Util::RAD_PER_DEG);
            float a = cosTheta * cosTheta + sinTheta * sinTheta;
            float b = cosTheta * (pBall->mVelocityX - mVelocityX) + sinTheta * (pBall->mVelocityY - mVelocityY);
            float force = fabs(b / a);
            mVelocityX -= force * cosTheta;
            mVelocityY -= force * sinTheta;
            pBall->mVelocityX += force * cosTheta;
            pBall->mVelocityY += force * sinTheta;

            mX = previousX;
            mY = previousY;
            mpSoundManager->Play(SoundManager::ballCollision, force / 72.0f);
         }
         UpdateWhiteSpotPosition(deltaTime);
      }
   }
}

void Ball::UpdateWhiteSpotPosition(float time)
{
   if (mpWhiteSpot != NULL)
   {
      if (mStatusManager.GetStatus() != StatusManager::dead)
      {
          float previousDeltaX = mpWhiteSpot->mDeltaX;
         float previousDeltaY = mpWhiteSpot->mDeltaY;
         if (!mpWhiteSpot->mBackside)
         {
            mpWhiteSpot->mDeltaX += mVelocityX * time;
            mpWhiteSpot->mDeltaY += mVelocityY * time;
         }
         else
         {
            mpWhiteSpot->mDeltaX -= mVelocityX * time;
            mpWhiteSpot->mDeltaY -= mVelocityY * time;
         }
         float magnitude = 0.0f;
         Util::RectangleToVector(mpWhiteSpot->mDeltaX, mpWhiteSpot->mDeltaY, magnitude, mpWhiteSpot->mAngle);

         if (mpWhiteSpot->GetDisplayOrder() == 7 && (magnitude > GetBallDiameter() / 2.0f) ||
             mpWhiteSpot->GetDisplayOrder() == 4 && (magnitude > GetPocketedBallDiameter() / 2.0f))
         {
            mpWhiteSpot->mDeltaX = previousDeltaX;
            mpWhiteSpot->mDeltaY = previousDeltaY;
            mpWhiteSpot->mBackside = !mpWhiteSpot->mBackside;
         }

         if (mpWhiteSpot->mBackside)
         {
            mpWhiteSpot->mX = -10000.0f;
         }
         else
         {
            mpWhiteSpot->mX = mpWhiteSpot->mDeltaX + mX;
            mpWhiteSpot->mY = mpWhiteSpot->mDeltaY + mY;
            if (mpWhiteSpot->GetDisplayOrder() == 4)
            {
               mpWhiteSpot->mSprite.setScale((1.0f - (magnitude / (GetPocketedBallDiameter() / 2.0f)) * (magnitude / (GetPocketedBallDiameter() / 2.0f)))
                                             * GetPocketedBallDiameter() / 36.0f, GetPocketedBallDiameter() / 36.0f);
            }   
            else
            {
               mpWhiteSpot->mSprite.setScale((1.0f - (magnitude / (mBallDiameter / 2.0f)) * (magnitude / (mBallDiameter / 2.0f))) * mBallDiameter / 36.0f, mBallDiameter / 36.0f);
            }
         }
      }
      else
      {
         mpWhiteSpot->mX = -10000.0f;
      }
   }
}

void Ball::UseWhiteSpot()
{
   mpWhiteSpot = Character::Resurrect<WhiteSpot>();
   assert(mpWhiteSpot != 0);
   mpWhiteSpot->Reset();
   do
   {
      mpWhiteSpot->mDeltaX = Util::RandomFloat(-mBallDiameter / 2.0f, mBallDiameter / 2.0f);
      mpWhiteSpot->mDeltaY = Util::RandomFloat(-mBallDiameter / 2.0f, mBallDiameter / 2.0f);
   } while (sqrt(mpWhiteSpot->mDeltaX * mpWhiteSpot->mDeltaX + mpWhiteSpot->mDeltaY * mpWhiteSpot->mDeltaY) > mBallDiameter / 2.0f);
   mpWhiteSpot->mBackside = (Util::RandomInteger(0, 1) == 0);
}

void Ball::Reset()
{
   mSprite.setColor(sf::Color(255, 255, 255));
   mVelocityX = 0.0f;
   mVelocityY = 0.0f;
   mScale = 0.005555555f * GetBallDiameter();     
   SetDisplayOrder(6);
}