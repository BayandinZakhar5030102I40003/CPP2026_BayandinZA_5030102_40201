#pragma once
#include <SFML/Graphics.hpp>

class Platform {
 private:
  sf::RectangleShape shape;
  float speed;
  float originalWidth;

 public:
  Platform();
  void update(float deltaTime);
  void draw(sf::RenderWindow& window);
  void setSize(float width);
  void resetSize();
  sf::FloatRect getBounds() const;
  float getX() const { return shape.getPosition().x; }
  float getWidth() const { return shape.getSize().x; }
};