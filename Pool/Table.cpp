#include "stdafx.h"
#include "Util.h"
#include "Character.h"
#include "Table.h"

Table::Table()
   : Character(typeid(Table))
{
   SetTexture(TextureManager::table);
   SetDisplayOrder(5);
   mScale = 0.65f;
}

Table::~Table()
{
}

void Table::Process(float deltaTime)
{  
}