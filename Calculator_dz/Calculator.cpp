#include "Calculator.h"

#include <windows.h>

#include <iostream>
#include <string>

#include "Parser.h"
#include "Plagins.h"

int main() {
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);

  Plagins plagins;

  plagins.LoadPlagins("./plagins");

  Parser parser(plagins);
  Calculator calc;

  std::cout << " Введите выражение:\n";

  std::string line;
  while (true) {
    if (!std::getline(std::cin, line) || line == "x") {
      break;
    }

    if (line.empty()) {
      continue;
    }

    try {
      auto commands = parser.Parse(line);

      double result = calc.Evaluate(commands);

      std::cout << "Результат: " << result << std::endl;
    } catch (std::exception const& e) {
      std::cout << "Ошибка: " << e.what() << std::endl;
    }
  }

  return 0;
}