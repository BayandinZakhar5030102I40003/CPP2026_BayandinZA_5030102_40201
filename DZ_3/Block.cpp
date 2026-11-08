#include "Block.h"

#include <cstdlib>

Block::Block(float x, float y, BlockType t, int hp)
    : type(t), health(hp), hasBonus(false), bonusType(BonusType::None) {
  shape.setSize({60, 20});
  shape.setPosition({x, y});

  switch (type) {
    case BlockType::Normal:
      shape.setFillColor(sf::Color::Green);
      hasBonus = (rand() % 10 < 3);
      break;
    case BlockType::Indestructible:
      shape.setFillColor(sf::Color::White);
      health = -1;
      break;
    case BlockType::BonusHidden:
      shape.setFillColor(sf::Color::Yellow);
      hasBonus = true;
      break;
    case BlockType::SpeedBoost:
      shape.setFillColor(sf::Color::Cyan);
      break;
    case BlockType::MultiHit:
      shape.setFillColor(sf::Color(255, 165, 0));
      break;
  }

  if (hasBonus) {
    std::vector<BonusType> bonuses = {
        BonusType::EnlargePlatform, BonusType::ShrinkPlatform,
        BonusType::SpeedUp,         BonusType::SpeedDown,
        BonusType::StickToPlatform, BonusType::OneTimeBottom,
        BonusType::RandomTrajectory};
    bonusType = bonuses[rand() % bonuses.size()];
  }
}

void Block::draw(sf::RenderWindow& window) {
  if (health > 0 || type == BlockType::Indestructible) {
    window.draw(shape);
  }
}

bool Block::hit() {
  if (type == BlockType::Indestructible) return false;

  health--;
  return true;
}

sf::FloatRect Block::getBounds() const { return shape.getGlobalBounds(); }