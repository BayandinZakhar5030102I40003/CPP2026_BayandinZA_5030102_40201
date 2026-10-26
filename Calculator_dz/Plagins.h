#pragma once
#include <windows.h>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Command.h"

struct Function {
  int queue = 0;
  int countEl = 0;
  std::function<std::unique_ptr<Command>()> fabricCommand;
};

class Plagins final {
 public:
  Plagins();
  ~Plagins() {}
  Plagins(Plagins const&) = delete;
  Plagins& operator=(Plagins const&) = delete;
  Plagins(Plagins&&) = default;
  Plagins& operator=(Plagins&&) = default;

  void OneFuncI(std::string const& name, int queue, OneElFunc ::OneFunc func);

  void BiFuncI(std::string const& name, int queue, BiElFunc ::BiFunc func);

  void LoadPlagins(std::string const& path);

  bool BoolFunc(std::string const& name) const;
  Function const& GetFunc(std::string const& name) const;

 private:
  using DllHandle = std::unique_ptr<std::remove_pointer_t<HMODULE>,
                                    std::function<void(HMODULE)>>;

  std::vector<DllHandle> loadDlls;
  std::map<std::string, Function> smthf;
};
