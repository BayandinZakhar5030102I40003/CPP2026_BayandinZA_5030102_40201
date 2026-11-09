#pragma once
#include <functional>
#include <stdexcept>
#include <vector>

class Command {
 public:
  virtual ~Command() = default;
  virtual void execute(std::vector<double>& stack) const = 0;
};

class StackCommand final : public Command {
 public:
  explicit StackCommand(double val) : smth(val) {}

  void execute(std::vector<double>& stack) const override final {
    stack.push_back(smth);
  }

 private:
  double smth;
};

class OneElFunc final : public Command {
 public:
  using OneFunc = std::function<double(double)>;
  explicit OneElFunc(OneFunc func) : smthf(std::move(func)) {}
  void execute(std::vector<double>& stack) const override final {
    if (stack.empty()) {
      throw std::runtime_error("Ошибка");
    }
    double a = stack.back();
    stack.pop_back();
    stack.push_back(smthf(a));
  }

 private:
  OneFunc smthf;
};

class BiElFunc final : public Command {
 public:
  using BiFunc = std::function<double(double, double)>;
  explicit BiElFunc(BiFunc func) : smthf(std::move(func)) {}
  void execute(std::vector<double>& stack) const override final {
    if (stack.size() < 2) {
      throw std::runtime_error("Ошибка");
    }
    double right = stack.back();
    stack.pop_back();
    double left = stack.back();
    stack.pop_back();
    stack.push_back(smthf(left, right));
  }

 private:
  BiFunc smthf;
};