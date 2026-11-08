#include "Game.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

Game::Game()
    : window(sf::VideoMode({1600, 600}), "Arkanoid"),
      platform(),
      score(0),
      misses(0),
      ballStuck(true),
      oneTimeBottom(false),
      deltaTime(0) {
  initBlocks();
  window.setFramerateLimit(120);
  balls.push_back(std::make_unique<Ball>(400, 530));
}

void Game::initBlocks() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, 100);

  int rows = 14;
  int cols = 21;
  float blockWidth = 50;
  float blockHeight = 20;
  float spacing = 5;

  float totalWidth = cols * (blockWidth + spacing) - spacing;
  float startX = (1600 - totalWidth) / 2;
  float startY = 50;

  int centerCol = cols / 2;

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col <= centerCol; col++) {
      bool createBlock = (dist(gen) < 40);

      if (createBlock) {
        float x = startX + col * (blockWidth + spacing);
        float y = startY + row * (blockHeight + spacing);

        BlockType type;
        int health = 1;
        int typeRand = dist(gen);

        if (row == 0 || row == 5 || row == 10) {
          type = BlockType::Indestructible;
        } else if (row == rows - 1) {
          if (typeRand < 30)
            type = BlockType::SpeedBoost;
          else if (typeRand < 60)
            type = BlockType::BonusHidden;
          else
            type = BlockType::Normal;
        } else {
          if (typeRand < 15) {
            type = BlockType::MultiHit;
            health = 3;
          } else if (typeRand < 30)
            type = BlockType::SpeedBoost;
          else if (typeRand < 45)
            type = BlockType::BonusHidden;
          else
            type = BlockType::Normal;
        }

        blocks.emplace_back(x, y, type, health);

        if (col != centerCol) {
          float rightX = startX + (cols - 1 - col) * (blockWidth + spacing);
          blocks.emplace_back(rightX, y, type, health);
        }
      }
    }
  }
}

void Game::checkCollisions() {
  deltaTime = clock.restart().asSeconds();
  if (deltaTime > 0.03f) deltaTime = 0.03f;

  for (auto& ball : balls) {
    sf::Vector2f pos = ball->getPosition();
    sf::Vector2f vel = ball->getVelocity();

    if (pos.x <= 0 || pos.x + 16 >= 1600) {
      vel.x = -vel.x;
      ball->setVelocity(vel.x, vel.y);
    }
    if (pos.y <= 0) {
      vel.y = -vel.y;
      ball->setVelocity(vel.x, vel.y);
    }

    if (pos.y + 16 >= 600) {
      if (oneTimeBottom) {
        oneTimeBottom = false;
        vel.y = -vel.y;
        ball->setVelocity(vel.x, vel.y);
      } else {
        misses++;
        ball->setVelocity(0, 0);
        ballStuck = true;

        if (misses >= 3) {
          std::cout << "Game Over! Score: " << score << std::endl;
          window.close();
        }
      }
    }

    if (ball->getBounds().findIntersection(platform.getBounds()).has_value()) {
      float hitPos = (ball->getPosition().x + 8 - platform.getX()) /
                     platform.getBounds().size.x;
      float angle = (hitPos - 0.5f) * 1.5f;
      sf::Vector2f vel2 = ball->getVelocity();
      float speed = std::sqrt(vel2.x * vel2.x + vel2.y * vel2.y);
      ball->setVelocity(speed * std::sin(angle), -speed * std::cos(angle));

      if (ballStuck) {
        ballStuck = false;
      }
    }
  }

  for (auto& ball : balls) {
    for (auto& block : blocks) {
      if (ball->getBounds().findIntersection(block.getBounds()).has_value()) {
        if (block.hit()) {
          score += 10;

          if (block.getType() == BlockType::SpeedBoost) {
            ball->increaseSpeed(1.2f);
          }

          if (block.hasHiddenBonus()) {
            spawnBonus(block.getBounds().position.x,
                       block.getBounds().position.y, block.getBonusType());
          }
        }

        sf::Vector2f vel = ball->getVelocity();
        vel.y = -vel.y;
        ball->setVelocity(vel.x, vel.y);
        break;
      }
    }
  }

  blocks.erase(std::remove_if(blocks.begin(), blocks.end(),
                              [](const Block& b) { return b.isDestroyed(); }),
               blocks.end());
}

void Game::updateBonuses() {
  for (auto& bonus : bonuses) {
    bonus.update(deltaTime);

    if (bonus.getBounds().findIntersection(platform.getBounds()).has_value()) {
      switch (bonus.getType()) {
        case BonusType::EnlargePlatform:
          platform.setSize(150);
          break;
        case BonusType::ShrinkPlatform:
          platform.setSize(50);
          break;
        case BonusType::SpeedUp:
          for (auto& ball : balls) ball->increaseSpeed(2.f);
          break;
        case BonusType::SpeedDown:
          for (auto& ball : balls) ball->increaseSpeed(1.f);
          break;
        case BonusType::StickToPlatform:
          ballStuck = true;
          break;
        case BonusType::OneTimeBottom:
          oneTimeBottom = true;
          break;
        case BonusType::RandomTrajectory:
          for (auto& ball : balls) ball->randomizeTrajectory();
          break;
        default:
          break;
      }
      bonus = bonuses.back();
      bonuses.pop_back();
      break;
    }
  }

  bonuses.erase(std::remove_if(bonuses.begin(), bonuses.end(),
                               [](const Bonus& b) {
                                 return b.getBounds().position.y > 600;
                               }),
                bonuses.end());
}

void Game::spawnBonus(float x, float y, BonusType type) {
  bonuses.emplace_back(x + 20, y, type);
}

void Game::resetBall() {
  if (ballStuck && !balls.empty()) {
    balls[0]->setVelocity(0, -200);
    ballStuck = false;
  }
}

void Game::run() {
  while (window.isOpen()) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto* keyPressed =
                     event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Space) {
          resetBall();
        }
      }
    }

    platform.update(deltaTime);

    if (ballStuck && !balls.empty()) {
      balls[0]->setStuckToPlatform(
          true,
          sf::Vector2f(platform.getX() + platform.getBounds().size.x / 2 - 8,
                       530));
      balls[0]->setVelocity(0, 0);
    } else {
      for (auto& ball : balls) {
        ball->update(deltaTime);
      }
    }

    checkCollisions();
    updateBonuses();

    window.clear(sf::Color::Black);
    platform.draw(window);
    for (auto& ball : balls) ball->draw(window);
    for (auto& block : blocks) block.draw(window);
    for (auto& bonus : bonuses) bonus.draw(window);
    window.display();

    bool allDestroyableBlocksGone = true;
    for (const auto& block : blocks) {
      if (block.getType() != BlockType::Indestructible &&
          !block.isDestroyed()) {
        allDestroyableBlocksGone = false;
        break;
      }
    }

    if (allDestroyableBlocksGone && blocks.size() > 0) {
      std::cout << "You win! Score: " << score << std::endl;
      window.close();
    } else if (blocks.empty()) {
      std::cout << "You win! Score: " << score << std::endl;
      window.close();
    }
  }
}