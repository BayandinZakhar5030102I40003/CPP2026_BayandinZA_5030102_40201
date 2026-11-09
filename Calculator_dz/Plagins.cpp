#include "Plagins.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace fs = std::filesystem;

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
  if (!fs::exists(path) || !fs::is_directory(path)) {
    std::cout << "Ошибка, папка плагинов не найдена\n";
    return;
  }

#ifdef _WIN32
  std::string ext = ".dll";
#else
  std::string ext = ".so";
#endif

  for (const auto& entry : fs::directory_iterator(path)) {
    if (!entry.is_regular_file() || entry.path().extension() != ext) {
      continue;
    }

    std::string fullPath = entry.path().string();
    std::string fileName = entry.path().filename().string();

#ifdef _WIN32
    void* hLib = LoadLibraryA(fullPath.c_str());
    auto freeLib = [](void* h) {
      if (h) FreeLibrary(static_cast<HMODULE>(h));
    };
    auto getProc = [hLib](const char* name) {
      return reinterpret_cast<void*>(
          GetProcAddress(static_cast<HMODULE>(hLib), name));
    };
#else
    void* hLib = dlopen(fullPath.c_str(), RTLD_LAZY);
    auto freeLib = [](void* h) {
      if (h) dlclose(h);
    };
    auto getProc = [hLib](const char* name) { return dlsym(hLib, name); };
#endif

    DllHandle dll(hLib, freeLib);

    if (!dll) {
      std::cout << "Ошибка загрузки библиотеки " << fileName << "\n";
      continue;
    }

    auto getName = reinterpret_cast<NamePtr>(getProc("GetName"));
    auto getCount = reinterpret_cast<InfoPtr>(getProc("GetCount"));
    auto getQueue = reinterpret_cast<InfoPtr>(getProc("GetQueue"));

    if (!getName || !getCount || !getQueue) {
      std::cout << "Ошибка формата плагина " << fileName << "\n";
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
      auto oneCalc = reinterpret_cast<OneFuncPtr>(getProc("OneCalc"));
      if (!oneCalc) continue;
      OneFuncI(name, queue, oneCalc);
    } else if (countEl == 2) {
      auto biCalc = reinterpret_cast<BiFuncPtr>(getProc("BiCalc"));
      if (!biCalc) continue;
      BiFuncI(name, queue, biCalc);
    } else {
      continue;
    }

    loadDlls.emplace_back(std::move(dll));
  }
}