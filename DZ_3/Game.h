#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "Ball.h"
#include "Block.h"
#include "Bonus.h"
#include "Platform.h"

class Game {
 private:
  sf::RenderWindow window;
  Platform platform;
  std::vector<std::unique_ptr<Ball>> balls;
  std::vector<Block> blocks;
  std::vector<Bonus> bonuses;

  int score;
  int misses;
  bool ballStuck;
  bool oneTimeBottom;
  float deltaTime;
  sf::Clock clock;

  void initBlocks();
  void checkCollisions();
  void updateBonuses();
  void spawnBonus(float x, float y, BonusType type);
  void resetBall();

 public:
  Game();
  void run();
};