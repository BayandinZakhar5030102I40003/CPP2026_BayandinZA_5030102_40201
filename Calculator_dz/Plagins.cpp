#include "Plagins.h"

#include <iostream>
#include <stdexcept>

using NamePtr = char const* (*)();
using InfoPtr = int (*)();
using OneFuncPtr = double (*)(double);
using BiFuncPtr = double (*)(double, double);

Plagins::Plagins() {
  BiFuncI("+", 1, [](double a, double b) { return a + b; });
  BiFuncI("-", 1, [](double a, double b) { return a - b; });
  BiFuncI("*", 2, [](double a, double b) { return a * b; });
  BiFuncI("/", 2, [](double a, double b) {
    if (b == 0.0) {
      throw std::runtime_error("Деление на ноль");
    }
    return a / b;
  });
}

void Plagins::OneFuncI(std::string const& name, int queue,
                       OneElFunc::OneFunc func) {
  smthf[name] = Function{
      queue, 1, [func]() { return std::make_unique<OneElFunc>(func); }};
}

void Plagins::BiFuncI(std::string const& name, int queue,
                      BiElFunc::BiFunc func) {
  smthf[name] =
      Function{queue, 2, [func]() { return std::make_unique<BiElFunc>(func); }};
}

bool Plagins::BoolFunc(std::string const& name) const {
  return smthf.find(name) != smthf.end();
}

Function const& Plagins::GetFunc(std::string const& name) const {
  auto it = smthf.find(name);
  if (it == smthf.end()) {
    throw std::runtime_error("Ошибка " + name);
  }
  return it->second;
}

void Plagins::LoadPlagins(std::string const& path) {
  std::string mask = path + "\\*.dll";
  WIN32_FIND_DATAA fileData;
  HANDLE hFind = FindFirstFileA(mask.c_str(), &fileData);

  if (hFind == INVALID_HANDLE_VALUE) {
    std::cout << "Ошибка, плагины не найдены\n";
    return;
  }

  using FindPtr = std::unique_ptr<void, std::function<void(HANDLE)>>;
  FindPtr findGuard(hFind, [](HANDLE h) {
    if (h != INVALID_HANDLE_VALUE) FindClose(h);
  });

  do {
    if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      continue;
    }

    std::string fileName = fileData.cFileName;
    std::string fullPath = path + "\\" + fileName;

    DllHandle dll(LoadLibraryA(fullPath.c_str()), [](HMODULE h) {
      if (h) FreeLibrary(h);
    });

    if (!dll) {
      std::cout << "Ошибка " << fileName << "\n";
      continue;
    }

    auto getName =
        reinterpret_cast<NamePtr>(GetProcAddress(dll.get(), "GetName"));
    auto getCount =
        reinterpret_cast<InfoPtr>(GetProcAddress(dll.get(), "GetCount"));
    auto getQueue =
        reinterpret_cast<InfoPtr>(GetProcAddress(dll.get(), "GetQueue"));

    if (!getName || !getCount || !getQueue) {
      std::cout << "Ошибка " << fileName << "\n";
      continue;
    }

    char const* rawName = getName();
    if (!rawName || std::string(rawName).empty()) {
      continue;
    }

    std::string name = rawName;
    int countEl = getCount();
    int queue = getQueue();

    if (countEl == 1) {
      auto oneCalc =
          reinterpret_cast<OneFuncPtr>(GetProcAddress(dll.get(), "OneCalc"));
      if (!oneCalc) continue;
      OneFuncI(name, queue, oneCalc);
    } else if (countEl == 2) {
      auto biCalc =
          reinterpret_cast<BiFuncPtr>(GetProcAddress(dll.get(), "BiCalc"));
      if (!biCalc) continue;
      BiFuncI(name, queue, biCalc);
    } else {
      continue;
    }

    loadDlls.emplace_back(std::move(dll));

  } while (FindNextFileA(hFind, &fileData) != 0);
}