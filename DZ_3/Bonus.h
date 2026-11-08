#pragma once
#include <SFML/Graphics.hpp>

#include "Block.h"

class Bonus {
 private:
  sf::RectangleShape shape;
  BonusType type;
  sf::Vector2f velocity;

 public:
  Bonus(float x, float y, BonusType t);
  void update(float deltaTime);
  void draw(sf::RenderWindow& window);
  sf::FloatRect getBounds() const;
  BonusType getType() const { return type; }
};