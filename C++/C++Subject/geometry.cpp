#include <iostream>
#include "geometry.h"
bool operator==(const double& num1, const double& num2) {
  return round(num1 * 1e9) == round(num2 * 1e9);
}
