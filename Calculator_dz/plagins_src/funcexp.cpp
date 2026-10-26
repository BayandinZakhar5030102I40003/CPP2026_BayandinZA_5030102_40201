#include <cmath>
#include <stdexcept>

extern "C" {
__declspec(dllexport) char const* GetName() { return "exp"; }
__declspec(dllexport) int GetCount() { return 1; }
__declspec(dllexport) int GetQueue() { return 4; }

__declspec(dllexport) double OneCalc(double x) {
  double res = std::exp(x);
  if (std::isinf(res) || std::isnan(res)) {
    throw std::runtime_error("Ошибка exp");
  }
  return res;
}
}