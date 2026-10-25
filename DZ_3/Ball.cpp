#include "Ball.h"

#include <cmath>
#include <random>

Ball::Ball(float startX, float startY) {
  shape.setRadius(10.f);
  shape.setFillColor(sf::Color::Red);
  shape.setPosition({startX, startY});
  velocity = {0.f, -200.f};
}

void Ball::update(float deltaTime) { shape.move(velocity * deltaTime); }

void Ball::draw(sf::RenderWindow& window) { window.draw(shape); }

void Ball::setVelocity(float vx, float vy) { velocity = {vx, vy}; }

void Ball::increaseSpeed(float factor) {
  velocity.x *= factor;
  velocity.y *= factor;
}

void Ball::randomizeTrajectory() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> angelDis(-3.14 / 4, 3.14 / 4);

  float angel = angelDis(gen);
  float currentSpeed =
      std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

  velocity.x = currentSpeed * std::sin(angel);
  velocity.y = -currentSpeed * std::cos(angel);
}

sf::FloatRect Ball::getBounds() const { return shape.getGlobalBounds(); }

void Ball::setStuckToPlatform(bool stuck, const sf::Vector2f& platformPos) {
  if (stuck) {
    shape.setPosition(platformPos);
    velocity = {0, 0};
  }
}