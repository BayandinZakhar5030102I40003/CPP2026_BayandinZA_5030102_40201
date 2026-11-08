#pragma once
#include <SFML/Graphics.hpp>

class Ball {
 private:
  sf::CircleShape shape;
  sf::Vector2f velocity;

 public:
  Ball(float startX, float startY);
  void update(float deltaTime);
  void draw(sf::RenderWindow& window);
  void setVelocity(float vx, float vy);
  void increaseSpeed(float factor);
  void randomizeTrajectory();
  sf::FloatRect getBounds() const;
  sf::Vector2f getPosition() const { return shape.getPosition(); }
  sf::Vector2f getVelocity() const { return velocity; }
  void setStuckToPlatform(bool stuck, const sf::Vector2f& platformPos);
};