#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Command.h"
#include "Plagins.h"

class Parser final {
 public:
  explicit Parser(Plagins const& plagins) : m_plagins(plagins) {}

  std::vector<std::unique_ptr<Command>> Parse(std::string const& expr) const;

 private:
  Plagins const& m_plagins;
};