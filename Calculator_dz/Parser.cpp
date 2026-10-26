#include "Parser.h"

#include <cctype>
#include <sstream>
#include <stdexcept>

#include "Command.h"

std::vector<std::unique_ptr<Command>> Parser::Parse(
    std::string const& expr) const {
  std::vector<std::unique_ptr<Command>> commands;
  std::vector<std::string> stackOp;

  bool needOperand = true;
  size_t i = 0;

  while (i < expr.length()) {
    if (std::isspace(static_cast<unsigned char>(expr[i]))) {
      i++;
      continue;
    }
    if (std::isdigit(static_cast<unsigned char>(expr[i])) || expr[i] == '.') {
      size_t start = i;
      while (i < expr.length() &&
             (std::isdigit(static_cast<unsigned char>(expr[i])) ||
              expr[i] == '.')) {
        i++;
      }
      std::stringstream ss(expr.substr(start, i - start));
      double val = 0.0;
      ss >> val;

      commands.emplace_back(std::make_unique<StackCommand>(val));
      needOperand = false;
      continue;
    }

    if (expr[i] == '(') {
      stackOp.push_back("(");
      needOperand = true;
      i++;
      continue;
    }

    if (expr[i] == ')') {
      bool foundOpenParen = false;
      while (!stackOp.empty()) {
        std::string top = stackOp.back();
        stackOp.pop_back();
        if (top == "(") {
          foundOpenParen = true;
          break;
        }
        if (top == "~") {
          commands.emplace_back(
              std::make_unique<OneElFunc>([](double x) { return -x; }));
        } else {
          commands.emplace_back(m_plagins.GetFunc(top).fabricCommand());
        }
      }
      if (!foundOpenParen) {
        throw std::runtime_error("Ошибка");
      }

      if (!stackOp.empty() && stackOp.back() != "(" && stackOp.back() != "~") {
        Function const& funcInfo = m_plagins.GetFunc(stackOp.back());
        if (funcInfo.countEl == 1) {
          commands.emplace_back(funcInfo.fabricCommand());
          stackOp.pop_back();
        }
      }
      needOperand = false;
      i++;
      continue;
    }

    std::string token;
    if (std::isalpha(static_cast<unsigned char>(expr[i]))) {
      size_t start = i;
      while (i < expr.length() &&
             std::isalnum(static_cast<unsigned char>(expr[i]))) {
        i++;
      }
      token = expr.substr(start, i - start);
    } else {
      token = std::string(1, expr[i]);
      i++;
    }

    if (token == "-" && needOperand) {
      stackOp.push_back("~");
      needOperand = true;
      continue;
    }

    if (!m_plagins.BoolFunc(token)) {
      throw std::runtime_error("Неизвестная функция " + token);
    }

    Function const& currFunc = m_plagins.GetFunc(token);

    if (currFunc.countEl == 1) {
      stackOp.push_back(token);
      needOperand = true;
    } else {
      if (needOperand) {
        throw std::runtime_error("Ошибка1 " + token);
      }
      while (!stackOp.empty() && stackOp.back() != "(") {
        std::string top = stackOp.back();
        int topQueue = (top == "~") ? 3 : m_plagins.GetFunc(top).queue;

        if (topQueue >= currFunc.queue) {
          stackOp.pop_back();
          if (top == "~") {
            commands.emplace_back(
                std::make_unique<OneElFunc>([](double x) { return -x; }));
          } else {
            commands.emplace_back(m_plagins.GetFunc(top).fabricCommand());
          }
        } else {
          break;
        }
      }
      stackOp.push_back(token);
      needOperand = true;
    }
  }

  while (!stackOp.empty()) {
    std::string top = stackOp.back();
    stackOp.pop_back();
    if (top == "(") {
      throw std::runtime_error("Ошибка2");
    }
    if (top == "~") {
      commands.emplace_back(
          std::make_unique<OneElFunc>([](double x) { return -x; }));
    } else {
      commands.emplace_back(m_plagins.GetFunc(top).fabricCommand());
    }
  }

  return commands;
}