#pragma once
#include "Screen.h"
#include "Player.h"
#include "Character.h"
#include "Ball.h"
#include "Menu.h"
#include "TextureManager.h"

class Game : public IMenuCallBack
{
public:
   Game();
   ~Game();

private:
   void SetMaxCharacters();
   virtual void UpdateMenuOptions();
   void StartNewGame();

   Screen mScreen;
   TextureManager mTextureManager;
   SoundManager mSoundManager;
   Menu mMenu;

   Ball* mpCueBall;
   Player* mpPlayer;

};
