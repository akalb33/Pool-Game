#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>
#include "Polygon.h"

class TextureManager
{
public:
   enum Textures
   {
      background,
      blackBackground,
      table,
      player,
      cue,
      purple,
      orange,
      black,
      whiteSpot
   };

   TextureManager()
   {
      Init();
      
      AddTexture(background, "cement.jpg");

      AddTexture(blackBackground, "black background.png");

      AddTexture(player, "stick.png", true);
      SetOrigin(player, 962.0f, 14.0f);
      AddPoint(player, 962.0f, 14.0f);


      AddTexture(table, "Table No Holes.png");

      AddTexture(cue, "cue.png", true);
      AddPoint(cue, 90.0f, 3.0f);
      AddPoint(cue, 130.0f, 13.0f);
      AddPoint(cue, 168.0f, 50.0f);
      AddPoint(cue, 178.0f, 89.0f);
      AddPoint(cue, 166.0f, 133.0f);
      AddPoint(cue, 137.0f, 163.0f);
      AddPoint(cue, 90.0f, 177.0f);
      AddPoint(cue, 40.0f, 160.0f);
      AddPoint(cue, 14.0f, 132.0f);
      AddPoint(cue, 4.0f, 89.0f);
      AddPoint(cue, 19.0f, 40.0f);
      AddPoint(cue, 52.0f, 12.0f);

      AddTexture(purple, "purple.png", true);
      AddPoint(purple, 90.0f, 3.0f);
      AddPoint(purple, 130.0f, 13.0f);
      AddPoint(purple, 168.0f, 50.0f);
      AddPoint(purple, 178.0f, 89.0f);
      AddPoint(purple, 166.0f, 133.0f);
      AddPoint(purple, 137.0f, 163.0f);
      AddPoint(purple, 90.0f, 177.0f);
      AddPoint(purple, 40.0f, 160.0f);
      AddPoint(purple, 14.0f, 132.0f);
      AddPoint(purple, 4.0f, 89.0f);
      AddPoint(purple, 19.0f, 40.0f);
      AddPoint(purple, 52.0f, 12.0f);

      AddTexture(orange, "orange.png", true);
      AddPoint(orange, 90.0f, 3.0f);
      AddPoint(orange, 130.0f, 13.0f);
      AddPoint(orange, 168.0f, 50.0f);
      AddPoint(orange, 178.0f, 89.0f);
      AddPoint(orange, 166.0f, 133.0f);
      AddPoint(orange, 137.0f, 163.0f);
      AddPoint(orange, 90.0f, 177.0f);
      AddPoint(orange, 40.0f, 160.0f);
      AddPoint(orange, 14.0f, 132.0f);
      AddPoint(orange, 4.0f, 89.0f);
      AddPoint(orange, 19.0f, 40.0f);
      AddPoint(orange, 52.0f, 12.0f);

      AddTexture(black, "8ball.png", true);
      AddPoint(black, 90.0f, 3.0f);
      AddPoint(black, 130.0f, 13.0f);
      AddPoint(black, 168.0f, 50.0f);
      AddPoint(black, 178.0f, 89.0f);
      AddPoint(black, 166.0f, 133.0f);
      AddPoint(black, 137.0f, 163.0f);
      AddPoint(black, 90.0f, 177.0f);
      AddPoint(black, 40.0f, 160.0f);
      AddPoint(black, 14.0f, 132.0f);
      AddPoint(black, 4.0f, 89.0f);
      AddPoint(black, 19.0f, 40.0f);
      AddPoint(black, 52.0f, 12.0f);

      AddTexture(whiteSpot, "whiteSpot1transparent.png", true);

   }

   // This automatically sets default origin and dimensions based on texture
   //    size. Call SetOrigin() and AddPoint() to overwrite these values.
   void AddTexture(Textures textureId, std::string fileName, bool smooth = false);

   const sf::Texture& GetTexture(Textures textureId) const;

   // This will set the origin of the texture. Rotation, movement and scaling
   //    are relative to the origin. Default dimensions are automatically set
   //    based on the size of the texture. Call AddPoint() to overwrite these.
   void SetOrigin(Textures textureId, float x, float y);

   // The first time this is called, the dimensions will be a point.
   // The second time this is called, the dimensions will be a line.
   // Three or more calls will define a polygon with an automatic line drawn
   //    from the last point added to the first point that was added.
   void AddPoint(Textures textureId, float x, float y);

   const Polygon& GetPolygon(Textures textureId);

private:
   enum State
   {
      noTexture,
      textureWithDefaultDimensions,
      originSetDefaultDimensions,
      pointsAdded
   };

   void Init();
   void SetDefaultDimensions(Textures textureId);

   static const unsigned int MAX_TEXTURES = 100;
   sf::Texture mTexture[MAX_TEXTURES];
   Polygon mPolygon[MAX_TEXTURES];
   State mState[MAX_TEXTURES];
};
