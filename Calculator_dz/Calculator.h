#pragma once
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Command.h"

class Calculator final {
 public:
  Calculator() = default;

  double Evaluate(std::vector<std::unique_ptr<Command>> const& commands) const {
    std::vector<double> stack;
    stack.reserve(commands.size());

    std::for_each(commands.cbegin(), commands.cend(),[&stack](std::unique_ptr<Command> const& cmd) {if (cmd) {cmd->execute(stack);}});

    if (stack.size() != 1) {
      throw std::runtime_error("Ошибка");
    }

    return stack.back();
  }
};