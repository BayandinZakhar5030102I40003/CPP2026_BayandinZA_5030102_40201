#include <cmath>
#include <stdexcept>

#if defined(_WIN32)
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {
PLUGIN_EXPORT char const* GetName() { return "exp"; }
PLUGIN_EXPORT int GetCount() { return 1; }
PLUGIN_EXPORT int GetQueue() { return 4; }

PLUGIN_EXPORT double OneCalc(double x) {
  double res = std::exp(x);
  if (std::isinf(res) || std::isnan(res)) {
    throw std::runtime_error("Ошибка exp");
  }
  return res;
}
}