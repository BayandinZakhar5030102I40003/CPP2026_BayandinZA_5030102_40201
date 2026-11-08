#pragma once
#include <SFML/Graphics.hpp>

enum class BlockType {
  Normal,
  Indestructible,
  BonusHidden,
  SpeedBoost,
  MultiHit
};

enum class BonusType {
  None,
  EnlargePlatform,
  ShrinkPlatform,
  SpeedUp,
  SpeedDown,
  StickToPlatform,
  OneTimeBottom,
  RandomTrajectory,
  MovingBlock,
  SecondBall
};

class Block {
 private:
  sf::RectangleShape shape;
  BlockType type;
  int health;
  bool hasBonus;
  BonusType bonusType;

 public:
  Block(float x, float y, BlockType t, int hp = 1);
  void draw(sf::RenderWindow& window);
  bool hit();
  BlockType getType() const { return type; }
  int getHealth() const { return health; }
  sf::FloatRect getBounds() const;
  bool hasHiddenBonus() const { return hasBonus; }
  BonusType getBonusType() const { return bonusType; }
  bool isDestroyed() const {
    return health <= 0 && type != BlockType::Indestructible;
  }
};