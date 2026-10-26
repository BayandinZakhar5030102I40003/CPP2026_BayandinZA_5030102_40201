#include <cmath>
#include <stdexcept>

extern "C" {
__declspec(dllexport) char const* GetName() { return "ln"; }
__declspec(dllexport) int GetCount() { return 1; }
__declspec(dllexport) int GetQueue() { return 4; }

__declspec(dllexport) double OneCalc(double x) {
  if (x <= 0.0) {
    throw std::runtime_error("Îøèáêà ln");
  }
  return std::log(x);
}
}