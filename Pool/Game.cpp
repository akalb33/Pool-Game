#include "stdafx.h"
#include "Game.h"
#include "Table.h"
#include "Background.h"
#include "BlackBackground.h"
#include "Bumper.h"
#include "Corner.h"
#include "Pocket.h"
#include "WhiteSpot.h"
#include "Util.h"
#include <assert.h>
#include <SFML/System/Time.hpp>
#include <random>

Game::Game()
   : mScreen(0.0f, 0.0f, 1920.0f, 1080.0f, false, "Pool")
   , mTextureManager()
   , mSoundManager()
   , mMenu()
   , mpCueBall(NULL)
   , mpPlayer(NULL)
{
   static bool allBallsStopped = true;

   Util::InitRandomizer();
   mScreen.setVerticalSyncEnabled(false);
   mScreen.SetMouseCursorVisible(false);
   Character::SetTextureManager(&mTextureManager);
   Character::SetSoundManager(&mSoundManager);
   Character::SetScreen(&mScreen);
   SetMaxCharacters();

   sf::Font font;
   assert(font.loadFromFile("arial.ttf"));
   sf::Text text;
   text.setFont(font);


   mMenu.SetMenuTitle("Pool");
   mMenu.SetText(text);
   mMenu.AddSetting("Start New Game");
   mMenu.AddSetting("Exit");
   mMenu.Load();

   bool goToMenu = true;

   sf::Clock clock;
   const float displayFramesPerSecond = 60.0f;
   const float maxProcessFramesPerSecond = 3000.0f;
   const float minProcessFramesPerSecond = 200.0f;
   float timeUntilNextNewDisplayFrame = 0.0f;
   while (mScreen.isOpen())
   {
      sf::Event event;
      while (mScreen.pollEvent(event) || goToMenu)
      {
         if (event.type == sf::Event::Closed)
            mScreen.close();
         else if (event.type == sf::Event::Resized)
            mScreen.HandleResizeEvent();
         else if (event.type == sf::Event::KeyReleased || goToMenu)
         {
            if (event.key.code == sf::Keyboard::Escape || goToMenu)
            {
               std::string selection = mMenu.ShowMenu(mScreen);
               if((selection == "Exit") || (selection == "WINDOW_CLOSED_EVENT"))
                  mScreen.close();
               else if(selection == "Start New Game")
                  StartNewGame();

               goToMenu = false;

               clock.restart();
            }
         }
      }

      while (clock.getElapsedTime().asSeconds() < 1.0f / maxProcessFramesPerSecond)
      {
      }

      float deltaTime = clock.restart().asSeconds();

      if (deltaTime > 1.0f / minProcessFramesPerSecond)
      {
         deltaTime = 1.0f / minProcessFramesPerSecond;
      }
      Character::ProcessAll(deltaTime);
      timeUntilNextNewDisplayFrame -= deltaTime;
      if (timeUntilNextNewDisplayFrame <= 0.0f)
      {
         mScreen.clear();
         Character::DisplayAll();
         mpPlayer->DrawHelperLine();
         mScreen.DisplayNewScreen();
         timeUntilNextNewDisplayFrame = 1.0f / displayFramesPerSecond;         
      }
      mScreen.OutputFramerate(10.0f);

      if (!allBallsStopped)
      {
         if (Ball::AllBallsStopped())
         {
            Character::Resurrect<Player>();
            allBallsStopped = true;
            if (mpCueBall->GetStatus() == StatusManager::dead)
            {
               mpCueBall->SetStatus(StatusManager::alive); 
               mpCueBall->Reset();
               std::vector<Ball*> ballsContacted;
               do
               {    
                  mpCueBall->mX = Util::RandomFloat(806.0f, 1096.0f);
                  mpCueBall->mY = Util::RandomFloat(410.0f, 669.0f);     
               } while (mpCueBall->WithinProximity<Ball>(ballsContacted, Ball::GetBallDiameter()));
            }
         }
      }
      else
      {
         allBallsStopped = Ball::AllBallsStopped();
      }

   }

   mMenu.Save();
}

Game::~Game()
{

}

void Game::SetMaxCharacters()
{
   Character::SetMaxLimit<Player>(1);
   Character::SetMaxLimit<Table>(1);
   Character::SetMaxLimit<Background>(1);
   Character::SetMaxLimit<Ball>(16);
   Character::SetMaxLimit<Bumper>(18);
   Character::SetMaxLimit<Corner>(12);
   Character::SetMaxLimit<Pocket>(6);
   Character::SetMaxLimit<WhiteSpot>(16);
   Character::SetMaxLimit<BlackBackground>(1);
}

void Game::UpdateMenuOptions()
{
}

void Game::StartNewGame()
{
   Character::KillAll();
   Background* pBackground = Character::Resurrect<Background>();
   assert(pBackground != 0);
   pBackground->mX = 960.0f;                    
   pBackground->mY = 540.0f;
   pBackground->mScale = 2.25f;
   BlackBackground* pBlackBackground = Character::Resurrect<BlackBackground>();
   assert(pBlackBackground != 0);
   pBlackBackground->mX = 960.0f;
   pBlackBackground->mY = 540.0f;
   Table* pTable = Character::Resurrect<Table>();
   assert(pTable != 0);
   pTable->mX = 960.0f;
   pTable->mY = 540.0f;
   mpCueBall = Character::Resurrect<Ball>();
   assert(mpCueBall != 0);
   mpCueBall->mX = 600.0f;
   mpCueBall->mY = Util::RandomFloat(520.0f, 560.0f);
   mpCueBall->Reset();
   mpCueBall->SetTexture(TextureManager::cue);
   static const float ballSpacing = Ball::GetBallDiameter() * 1.02f;
   static const float rackX = 1236.0f;
   static const float rackY = 540.0f;
   std::vector<bool> oranges = { false, true, false, false, true, true, false, true, false, false, true, true, false, true };
   for (int row = 0; row < 5; row++)
   {
      for (int column = 0; column < row + 1; column++)
      {                                                                                     
         Ball* pBall = Character::Resurrect<Ball>();
         assert(pBall != 0);
         pBall->Reset();
         pBall->mX = rackX + ballSpacing * row;
         pBall->mY = rackY + ballSpacing * row * 0.5f - ballSpacing * column;
         if (column == 1 && row == 2)
         {
            pBall->SetTexture(TextureManager::black);
         }
         else 
         {
            if (*(oranges.begin()))
            {
               pBall->SetTexture(TextureManager::orange);
            }
            else
            {
               pBall->SetTexture(TextureManager::purple);
            }
            pBall->UseWhiteSpot();
            pBall->UpdateWhiteSpotPosition(0.0f);
            oranges.erase(oranges.begin());
         }
      }
   }


	mpPlayer = Character::Resurrect<Player>();
   assert(mpPlayer != 0);
   mpPlayer->Reset();
   mpPlayer->AssociateCueBall(mpCueBall);

   Bumper* pTopLeftBumper1 = Character::Resurrect<Bumper>();
   assert(pTopLeftBumper1 != 0);
   pTopLeftBumper1->Init(372.0f, 286.0f, 386.0f, 301.0f);

   Bumper* pTopLeftBumper2 = Character::Resurrect<Bumper>();
   assert(pTopLeftBumper2 != 0);
   pTopLeftBumper2->Init(408.0f, 245.0f, 428.0f, 263.0f);

   Bumper* pTopBumper1 = Character::Resurrect<Bumper>();
   assert(pTopBumper1 != 0);
   pTopBumper1->Init(428.0f, 263.0f, 917.0f, 263.0f);

   Bumper* pTopMiddleBumper1 = Character::Resurrect<Bumper>();
   assert(pTopMiddleBumper1 != 0);
   pTopMiddleBumper1->Init(917.0f, 263.0f, 924.0f, 239.0f);

   Bumper* pTopMiddleBumper2 = Character::Resurrect<Bumper>();
   assert(pTopMiddleBumper2 != 0);
   pTopMiddleBumper2->Init(981.0f, 242.0f, 990.0f, 263.0f);

   Bumper* pTopBumper2 = Character::Resurrect<Bumper>();
   assert(pTopBumper2 != 0);
   pTopBumper2->Init(990.0f, 263.0f, 1477.0f, 263.0f);

   Bumper* pTopRightBumper1 = Character::Resurrect<Bumper>();
   assert(pTopRightBumper1 != 0);
   pTopRightBumper1->Init(1477.0f, 263.0f, 1498.0f, 248.0f);

   Bumper* pTopRightBumper2 = Character::Resurrect<Bumper>();
   assert(pTopRightBumper2 != 0);
   pTopRightBumper2->Init(1534.0f, 286.0f, 1519.0f, 307.0f);

   Bumper* pRightBumper = Character::Resurrect<Bumper>();
   assert(pRightBumper != 0);
   pRightBumper->Init(1519.0f, 307.0f, 1519.0f, 761.0f);

   Bumper* pBottomRightBumper1 = Character::Resurrect<Bumper>();
   assert(pBottomRightBumper1 != 0);
   pBottomRightBumper1->Init(1519.0f, 761.0f, 1538.0f, 784.0f);

   Bumper* pBottomRightBumper2 = Character::Resurrect<Bumper>();
   assert(pBottomRightBumper2 != 0);
   pBottomRightBumper2->Init(1499.0f, 819.0f, 1476.0f, 804.0f);

   Bumper* pBottomBumper1 = Character::Resurrect<Bumper>();
   assert(pBottomBumper1 != 0);
   pBottomBumper1->Init(1476.0f, 804.0f, 988.0f, 804.0f);

   Bumper* pBottomMiddleBumper1 = Character::Resurrect<Bumper>();
   assert(pBottomMiddleBumper1 != 0);
   pBottomMiddleBumper1->Init(988.0f, 804.0f, 981.0f, 823.0f);

   Bumper* pBottomMiddleBumper2 = Character::Resurrect<Bumper>();
   assert(pBottomMiddleBumper2 != 0);
   pBottomMiddleBumper2->Init(923.0f, 825.0f, 915.0f, 804.0f);

   Bumper* pBottomBumper2 = Character::Resurrect<Bumper>();
   assert(pBottomBumper2 != 0);
   pBottomBumper2->Init(915.0f, 804.0f, 428.0f, 804.0f);

   Bumper* pBottomLeftBumper1 = Character::Resurrect<Bumper>();
   assert(pBottomLeftBumper1 != 0);
   pBottomLeftBumper1->Init(428.0f, 804.0f, 408.0f, 819.0f);

   Bumper* pBottomLeftBumper2 = Character::Resurrect<Bumper>();
   assert(pBottomLeftBumper2 != 0);
   pBottomLeftBumper2->Init(367.0f, 783.0f, 386.0f, 762.0f);

   Bumper* pLeftBumper = Character::Resurrect<Bumper>();
   assert(pLeftBumper != 0);
   pLeftBumper->Init(386.2f, 301.0f, 386.0f, 762.0f);

   Corner* pTopLeftCorner1 = Character::Resurrect<Corner>();
   assert(pTopLeftCorner1 != 0);
   pTopLeftCorner1->Init(386.0f, 301.0f);

   Corner* pTopLeftCorner2 = Character::Resurrect<Corner>();
   assert(pTopLeftCorner2 != 0);
   pTopLeftCorner2->Init(428.0f, 263.0f);

   Corner* pTopMiddleCorner1 = Character::Resurrect<Corner>();
   assert(pTopMiddleCorner1 != 0);
   pTopMiddleCorner1->Init(917.0f, 263.0f);

   Corner* pTopMiddleCorner2 = Character::Resurrect<Corner>();
   assert(pTopMiddleCorner2 != 0);
   pTopMiddleCorner2->Init(990.0f, 263.0f);

   Corner* pTopRightCorner1 = Character::Resurrect<Corner>();
   assert(pTopRightCorner1 != 0);
   pTopRightCorner1->Init(1477.0f, 263.0f);

   Corner* pTopRightCorner2 = Character::Resurrect<Corner>();
   assert(pTopRightCorner2 != 0);
   pTopRightCorner2->Init(1519.0f, 307.0f);

   Corner* pBottomRightCorner1 = Character::Resurrect<Corner>();
   assert(pBottomRightCorner1 != 0);
   pBottomRightCorner1->Init(1519.0f, 761.0f);

   Corner* pBottomRightCorner2 = Character::Resurrect<Corner>();
   assert(pBottomRightCorner2 != 0);
   pBottomRightCorner2->Init(1476.0f, 804.0f);

   Corner* pBottomMiddleCorner1 = Character::Resurrect<Corner>();
   assert(pBottomMiddleCorner1 != 0);
   pBottomMiddleCorner1->Init(988.0f, 804.0f);

   Corner* pBottomMiddleCorner2 = Character::Resurrect<Corner>();
   assert(pBottomMiddleCorner2 != 0);
   pBottomMiddleCorner2->Init(915.0f, 804.0f);

   Corner* pBottomLeftCorner1 = Character::Resurrect<Corner>();
   assert(pBottomLeftCorner1 != 0);
   pBottomLeftCorner1->Init(428.0f, 804.0f);

   Corner* pBottomLeftCorner2 = Character::Resurrect<Corner>();
   assert(pBottomLeftCorner2 != 0);
   pBottomLeftCorner2->Init(386.0f, 762.0f);

   Pocket* pTopLeftPocket = Character::Resurrect<Pocket>();
   assert(pTopLeftPocket != 0);
   pTopLeftPocket->Init(392.0f, 270.0f, 375.0f, 255.0f, 500.0f, 255.0f);

   Pocket* pTopMiddlePocket = Character::Resurrect<Pocket>();
   assert(pTopMiddlePocket != 0);
   pTopMiddlePocket->Init(950.0f, 246.0f, 950.0f, 231.0f, 1050.0f, 231.0f);

   Pocket* pTopRightPocket = Character::Resurrect<Pocket>();
   assert(pTopRightPocket != 0);
   pTopRightPocket->Init(1513.0f, 269.0f, 1532.0f, 254.0f, 1532.0f, 350.0f);

   Pocket* pBottomLeftPocket = Character::Resurrect<Pocket>();
   assert(pBottomLeftPocket != 0);
   pBottomLeftPocket->Init(385.0f, 803.0f, 385.0f - 19.0f, 818.0f, 500.0f, 818.0f);

   Pocket* pBottomMiddlePocket = Character::Resurrect<Pocket>();
   assert(pBottomMiddlePocket != 0);
   pBottomMiddlePocket->Init(946.0f, 824.0f, 946.0f, 839.0f, 1050.0f, 839.0f);

   Pocket* pBottomRightPocket = Character::Resurrect<Pocket>();
   assert(pBottomRightPocket != 0);
   pBottomRightPocket->Init(1516.0f, 804.0f, 1535.0f, 819.0f, 1535.0f, 650.0f);

   
}
