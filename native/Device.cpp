#include "Device.h"

namespace fx_rack {
  FilterDevice::FilterDevice(
    double cutoff,
    int taps,
    bool hp
  ): cutoff(cutoff), taps(taps), hp(hp) {}

  int FilterDevice::getType() const {
    return 0;
  }
}
