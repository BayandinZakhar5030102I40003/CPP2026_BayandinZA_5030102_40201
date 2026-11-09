#include <cmath>

#if defined(_WIN32)
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {
PLUGIN_EXPORT char const* GetName() { return "sin"; }
PLUGIN_EXPORT int GetCount() { return 1; }
PLUGIN_EXPORT int GetQueue() { return 4; }

PLUGIN_EXPORT double OneCalc(double deg) {
  double const pi = 3.1415926535;
  return std::sin(deg * pi / 180.0);
}
}