#include <cmath>

extern "C" {
__declspec(dllexport) char const* GetName() { return "sin"; }
__declspec(dllexport) int GetCount() { return 1; }
__declspec(dllexport) int GetQueue() { return 4; }

__declspec(dllexport) double OneCalc(double deg) {
  double const pi = 3.1415926535;
  return std::sin(deg * pi / 180.0);
}
}