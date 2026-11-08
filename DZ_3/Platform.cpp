#include "Platform.h"

Platform::Platform() {
  originalWidth = 100.f;
  shape.setSize(sf::Vector2f(originalWidth, 15.f));
  shape.setFillColor(sf::Color::White);
  shape.setPosition({750, 550});
  speed = 400.f;
}

void Platform::update(float deltaTime) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
    shape.move({-speed * deltaTime, 0});
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
    shape.move({speed * deltaTime, 0});
  }

  if (shape.getPosition().x < 0) {
    shape.setPosition({0, shape.getPosition().y});
  }
  if (shape.getPosition().x + shape.getSize().x > 1600) {
    shape.setPosition({1600 - shape.getSize().x, shape.getPosition().y});
  }
}

void Platform::draw(sf::RenderWindow& window) { window.draw(shape); }

void Platform::setSize(float width) {
  float currentX = shape.getPosition().x + shape.getSize().x / 2;
  shape.setSize(sf::Vector2f(width, 15.f));
  shape.setPosition({currentX - width / 2, shape.getPosition().y});
}

void Platform::resetSize() { setSize(originalWidth); }

sf::FloatRect Platform::getBounds() const { return shape.getGlobalBounds(); }