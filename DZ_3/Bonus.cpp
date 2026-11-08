#include "Bonus.h"

Bonus::Bonus(float x, float y, BonusType t) : type(t) {
  shape.setSize({20, 20});
  shape.setPosition({x, y});
  velocity = {0, 100.f};

  switch (type) {
    case BonusType::EnlargePlatform:
      shape.setFillColor(sf::Color::Magenta);
      break;
    case BonusType::ShrinkPlatform:
      shape.setFillColor(sf::Color::Green);
      break;
    case BonusType::SpeedUp:
      shape.setFillColor(sf::Color::Red);
      break;
    case BonusType::SpeedDown:
      shape.setFillColor(sf::Color::Blue);
      break;
    case BonusType::StickToPlatform:
      shape.setFillColor(sf::Color(255, 255, 255));
      break;
    case BonusType::OneTimeBottom:
      shape.setFillColor(sf::Color::Yellow);
      break;
    case BonusType::RandomTrajectory:
      shape.setFillColor(sf::Color(255, 128, 0));
      break;
    default:
      shape.setFillColor(sf::Color::White);
      break;
  }
}

void Bonus::update(float deltaTime) { shape.move(velocity * deltaTime); }

void Bonus::draw(sf::RenderWindow& window) { window.draw(shape); }

sf::FloatRect Bonus::getBounds() const { return shape.getGlobalBounds(); }