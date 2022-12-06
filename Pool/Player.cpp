#include "stdafx.h"
#include "Util.h"
#include "Player.h"
#include <SFML/Window/Mouse.hpp>
#include "Character.h"
#include "Bumper.h"
#include "Pocket.h"
#include "Corner.h"
#include <cmath>
#include <iostream>

Player::Player()
   : Character(typeid(Player))
   , mSpacebarPreviouslyPressed(false)
   , mDistanceToCueBall(0.0f)
   , mPreviousDistanceToCueBall(0.0f)
   , mpCueBall(NULL)
   , mTimePassedAccumulator(0.0f)
   , mKilled(true)
{
   SetTexture(TextureManager::player);
   SetDisplayOrder(180);
   mScale = 0.6f;
}

Player::~Player()
{
}

void Player::Reset()
{ 
   mKilled = true;
}

void Player::Process(float deltaTime)
{
   if (mKilled)
   {
      sf::Mouse::setPosition(sf::Vector2i(400, 400), *mpScreen);
      mDistanceToCueBall = 50.0f;
      mTimePassedAccumulator = 0.0f;
      mAngle = 0.0f;
      mKilled = false;
   }

   mX = mpCueBall->mX - mDistanceToCueBall * cos(mAngle * (3.1415f / 180.0f));
   mY = mpCueBall->mY - mDistanceToCueBall * sin(mAngle * (3.1415f / 180.0f));     

   sf::Vector2i currentMousePosition = sf::Mouse::getPosition(*mpScreen);
   float mouseDeltaX = currentMousePosition.x - 400.0f;
   float mouseDeltaY = currentMousePosition.y - 400.0f;

   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
   {
      float mPreviousX = mpCueBall->mX;
      float mPreviousY = mpCueBall->mY;
      std::vector<Bumper*> bumpersContacted;
      std::vector<Ball*> ballsContacted;
      std::vector<Corner*> cornersContacted;
      std::vector<Pocket*> pocketsContacted;
      mpCueBall->mX += mouseDeltaX / 2.0f;
      mpCueBall->mY += mouseDeltaY / 2.0f;
      if (mpCueBall->Collision(bumpersContacted) || mpCueBall->WithinProximity(ballsContacted, Ball::GetBallDiameter()) || mpCueBall->Collision(cornersContacted) || mpCueBall->Collision(pocketsContacted))
         //if (mpCueBall->Collision(bumpersContacted) || mpCueBall->Collision(ballsContacted) || mpCueBall->Collision(cornersContacted) || mpCueBall->Collision(pocketsContacted))
         {
         mpCueBall->mX = mPreviousX;
         mpCueBall->mY = mPreviousY;
      }
      sf::Mouse::setPosition(sf::Vector2i(400, 400), *mpScreen);
   }
   else
   {
      mTimePassedAccumulator += deltaTime;
      if (mTimePassedAccumulator > 0.03f)
      {
         float mouseMagnitude = 0.0f;
         float mouseAngle = 0.0f;
         Util::RectangleToVector(mouseDeltaX, mouseDeltaY, mouseMagnitude, mouseAngle);
         float mouseAngleFromStick = mouseAngle - mAngle;
         float mouseDeltaParallel = mouseMagnitude * cosf(mouseAngleFromStick * Util::RAD_PER_DEG);
         float mouseDeltaPerpendicular = mouseMagnitude * sinf(mouseAngleFromStick * Util::RAD_PER_DEG);
         if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
         {
            mDistanceToCueBall = 50.0f;
            //UpdateCueAngle(mouseDeltaX, mouseDeltaY);
            mAngle -= mouseDeltaPerpendicular / 6.0f;
         }
         else
         {
            mDistanceToCueBall -= mouseDeltaParallel / 2.0f;
            if (mDistanceToCueBall > 300.0f)
            {
                mDistanceToCueBall = 300.0f;
            }
            else if (mDistanceToCueBall < Ball::GetBallDiameter() / 2.0f)
            {
               Kill();
               mKilled = true;
               float cueStickVelocity = (mPreviousDistanceToCueBall - mDistanceToCueBall) / mTimePassedAccumulator;
               if (cueStickVelocity < 20.0f)
               {
                  cueStickVelocity = 20.0f;
               }
               else if (cueStickVelocity > 3500.0f)
               {
                  cueStickVelocity = 3500.0f;
               }
               mpSoundManager->Play(SoundManager::stickCollision, cueStickVelocity / 50.0f);
               mpCueBall->mVelocityX = cueStickVelocity * cos(mAngle * (3.1415f / 180.0f));
               mpCueBall->mVelocityY = cueStickVelocity * sin(mAngle * (3.1415f / 180.0f));
            }
            mPreviousDistanceToCueBall = mDistanceToCueBall;
         }
         sf::Mouse::setPosition(sf::Vector2i(400, 400), *mpScreen);
         mTimePassedAccumulator = 0.0f;
      }
   }
}

void Player::DrawHelperLine()
{
   if (mStatusManager.GetStatus() == StatusManager::alive)
   {
      sf::VertexArray lines(sf::LinesStrip, 2);


      lines[0].position = sf::Vector2f(mpCueBall->mX, mpCueBall->mY);
      lines[0].color = sf::Color::Red;
      lines[1].position = sf::Vector2f(mpCueBall->mX + 800.0f * cosf(mAngle * Util::RAD_PER_DEG), mpCueBall->mY + 800.0f * sinf(mAngle * Util::RAD_PER_DEG));
      lines[1].color = sf::Color::Red;

      mpScreen->draw(lines);
   }
}

void Player::UpdateCueAngle(float mouseDeltaX, float mouseDeltaY)
{
   static const float deadZone = 1.0f;
   static const float sensitivity = 16.0f;
   float xDirection = 0.0f;
   float yDirection = 0.0f;
   if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
   {
      mAngle = mAngle;
   }
   if (mAngle > 270.0f)
   {
      xDirection = -1.0f;
      yDirection = -1.0f;
   }
   else if (mAngle > 180.0f)
   {
      xDirection = -1.0f;
      yDirection = 1.0f;
   }
   else if (mAngle > 90.0f)
   {
      xDirection = 1.0f;
      yDirection = 1.0f;
   }
   else
   {
      xDirection = 1.0f;
      yDirection = -1.0f;
   }
   if (mouseDeltaX > deadZone || mouseDeltaX < -deadZone)
   {
      mAngle += xDirection * mouseDeltaX / sensitivity;
   }
   if (mouseDeltaY > deadZone || mouseDeltaY < -deadZone)
   {
      mAngle += yDirection * mouseDeltaY / sensitivity;
   }
   while (mAngle > 360.0f)
   {
      mAngle -= 360.0f;
   }
   while (mAngle < 0.0f)
   {
      mAngle += 360.0f;
   }

}

